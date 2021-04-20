#ifndef CONF_CLOCK_H_INCLUDED
#define CONF_CLOCK_H_INCLUDED

// ===== System Clock (MCK) Source Options
//#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_SLCK_RC
//#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_SLCK_XTAL
//#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_SLCK_BYPASS
//#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_MAINCK_4M_RC
//#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_MAINCK_8M_RC
//#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_MAINCK_12M_RC
//#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_MAINCK_XTAL
//#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_MAINCK_BYPASS
//#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_PLLACK
#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_PLLBCK


// ===== System Clock (MCK) Prescaler Options   (Fmck = Fsys / (SYSCLK_PRES))
#define CONFIG_SYSCLK_PRES          SYSCLK_PRES_1
//#define CONFIG_SYSCLK_PRES          SYSCLK_PRES_2
//#define CONFIG_SYSCLK_PRES          SYSCLK_PRES_4
//#define CONFIG_SYSCLK_PRES          SYSCLK_PRES_8
//#define CONFIG_SYSCLK_PRES          SYSCLK_PRES_16
//#define CONFIG_SYSCLK_PRES          SYSCLK_PRES_32
//#define CONFIG_SYSCLK_PRES          SYSCLK_PRES_64
//#define CONFIG_SYSCLK_PRES          SYSCLK_PRES_3


// PLL0 = PLLA
// PLL1 = PLLB
#define CONFIG_PLL1_SOURCE          PLLB_SRC_MAINCK_XTAL
#define CONFIG_PLL1_MUL             2
#define CONFIG_PLL1_DIV             1

// ===== Coprocessor System Clock (CPMCK) Options
// Fcpmck = Fcpclk_source / CPCLK_PRES

// Note:
// CONFIG_CPCLK_ENABLE  MUST be defined if using peripherals on bus matrix 1.
#define CONFIG_CPCLK_ENABLE

// Coprocessor System Clock Source Options
//#define CONFIG_CPCLK_SOURCE         CPCLK_SRC_SLCK
//#define CONFIG_CPCLK_SOURCE         CPCLK_SRC_MAINCK
//#define CONFIG_CPCLK_SOURCE         CPCLK_SRC_PLLACK
//#define CONFIG_CPCLK_SOURCE         CPCLK_SRC_PLLBCK
//#define CONFIG_CPCLK_SOURCE         CPCLK_SRC_PLLBCK
#define CONFIG_CPCLK_SOURCE         CPCLK_SRC_MCK

// Coprocessor System Clock Prescaler Options (CPCLK_PRES may be 1 to 16).
#define CONFIG_CPCLK_PRES           1

#endif /* CONF_CLOCK_H_INCLUDED */
