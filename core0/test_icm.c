#include "test_icm.h"

#include <icm.h>
#include <string.h>


COMPILER_ALIGNED(64)
struct icm_region_descriptor_main_list reg_descriptor[1];


// 512-bit block.
// Up to 65536 bits can be transferred,
// configurable with icm_region_descriptor_main_list::tran_size.
volatile uint32_t message_to_hash[] = {
   0x80636261,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x00000000,
   0x18000000
};


// It is a requirement (ds. 42.5.3)
// that the ICM Hash Area be 128-byte-aligned.
COMPILER_ALIGNED(128)
uint32_t output_sha[0x20];


static void reg_dig_mismatch_handler(uint8_t reg_num)
{
		asm volatile("nop");
	//while (1);

	// if(reg_num == ICM_REGION_NUM_0) {
	// 	printf( " Memory region0 is modified \n\r");
	// } else if(reg_num == ICM_REGION_NUM_1) {
	// 	printf( " Memory region1 is modified \n\r");
	// }
}


void test_icm(void) {
	// Clear output
	memset(output_sha, 0, 0x20);

	/* ICM configuration */
	struct icm_config icm_cfg;

	/* ICM initialization */

	// false = permitted.
	icm_cfg.is_write_back = false;

	// End of monitoring is:
	// - forbidden (true)
	// - allowed (false)
	icm_cfg.is_dis_end_mon = true;

	// Branching to secondary list is: forbidden (true)
	icm_cfg.is_sec_list_branch = true;

	// Bus Burden Control
	// 2^(bbc) = waiting cycles
	icm_cfg.bbc = 8;

	// ASCD
	// Automatic Switch to Compare Digest
	// WBDIS (CFG) and CDWBN (RCFG) have no effect.
	icm_cfg.is_auto_mode = false;

	// Dual buffering
	icm_cfg.is_dual_buf = true;

	// Do we provide the initial hash value?
	icm_cfg.is_user_hash = false;


	icm_cfg.ualgo = ICM_SHA_1;

	// HPROT region hash
	icm_cfg.hash_area_val = 0;

	// DAPROT region descriptor
	icm_cfg.des_area_val = 0;

	icm_init(ICM, &icm_cfg);


	/**
	 * Configure our sole region descriptor
	**/

	reg_descriptor[0].start_addr = (uint32_t)message_to_hash;
	reg_descriptor[0].cfg.is_compare_mode = false;
	reg_descriptor[0].cfg.is_wrap = true;
	reg_descriptor[0].cfg.is_end_mon = false;
	reg_descriptor[0].cfg.reg_hash_int_en = true;


	// Enable digest mismatch interrupt
	reg_descriptor[0].cfg.dig_mis_int_en = false;

	reg_descriptor[0].cfg.bus_err_int_en = false;
	reg_descriptor[0].cfg.wrap_con_int_en = false;
	reg_descriptor[0].cfg.ebit_con_int_en = false;
	reg_descriptor[0].cfg.status_upt_con_int_en = false;
	reg_descriptor[0].cfg.is_pro_dly = false;

	// HRPOT
	reg_descriptor[0].cfg.mem_reg_val = 0;

	reg_descriptor[0].cfg.algo = ICM_SHA_1;
	reg_descriptor[0].tran_size = 0;
	reg_descriptor[0].next_addr = 0;

	/* Set region descriptor start address */
	icm_set_reg_des_addr(ICM, (uint32_t)(reg_descriptor));

	/* Set hash area start address */
	icm_set_hash_area_addr(ICM, (uint32_t)output_sha);

	icm_set_callback(ICM, reg_dig_mismatch_handler, ICM_REGION_NUM_0,
			ICM_INTERRUPT_RDM, 1);

	
	icm_enable(ICM);


	// Wait a little to store the initial hash value
	// This could be improved with interrupt
	// handlers etc to have 0 busy waiting.
	for (uint32_t i = 0; i < (1 << 21); i++) {
		asm volatile("nop");
	}


	// Switch from storing to monitoring.
	reg_descriptor[0].cfg.is_compare_mode = true;


	// Wait a little for the module to change mode.
	// Not sure the busy wait can be avoided here.
	for (uint32_t i = 0; i < (1 << 21); i++) {
		asm volatile("nop");
	}

	// Modify the region data.
	// This should now trigger a digest mismatch interrupt.
	// See it with a breakpoint!
	message_to_hash[0] = 0x12345678;
}
