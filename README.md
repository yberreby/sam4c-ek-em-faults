# Workflow pour second coeur

Compilez le projet.
Les dépendances du Makefile sont cassées pour l'instant, donc le `make clean` est important.

```
$ make clean 
$ make
```


Lancer JLinkGDBServer (pour le coeur 0) :

```
$ make server
```

Dans un nouveau terminal, ouvrir une session GDB (qui va servir à reprogrammer la carte et à inspecter l'état du coeur 0) :

```
$ cd core0
$ make debug_flash
```

Dans un troisième terminal, ouvrir une instance `JLinkExe` pour le coeur 1.

Il est **essentiel** d'exécuter `exec CORESIGHT_SetIndexAHBAPToUse = 1` avant
`connect`, pour se connecter au second coeur.

```
$ JLinkExe -device AT91SAM4C16C -speed 4000 -if swd
SEGGER J-Link Commander V6.96 (Compiled Feb 19 2021 09:59:28)
DLL version V6.96, compiled Feb 19 2021 09:59:12

Connecting to J-Link via USB...O.K.
Firmware: J-Link V9 compiled Feb  2 2021 16:34:10
Hardware version: V9.30
S/N: 269301061
License(s): FlashBP, GDB
OEM: SEGGER-EDU
VTref=3.322V


Type "connect" to establish a target connection, '?' for help
J-Link>exec CORESIGHT_SetIndexAHBAPToUse = 1;
J-Link>connect
Device "ATSAM4C16C" selected.


Connecting to target via SWD
Found SW-DP with ID 0x2BA01477
DPIDR: 0x2BA01477
AP map detection skipped. Manually configured AP map found.
AP[0]: MEM-AP (IDR: Not set)
AP[1]: AHB-AP (IDR: Not set)
AP[1]: Core found
AP[1]: AHB-AP ROM base: 0xE00FF000
CPUID register: 0x410FC241. Implementer code: 0x41 (ARM)
Found Cortex-M4 r0p1, Little endian.
FPUnit: 6 code (BP) slots and 2 literal slots
CoreSight components:J-Link>halt
PC = 0000046A, CycleCnt = 035D0347
R0 = 11111111, R1 = 0001100F, R2 = 00033000, R3 = 00077000
R4 = 000FF000, R5 = 0AA00000, R6 = 09900000, R7 = 05A00000
R8 = 0000055F, R9 = 00000AA0, R10= 40000000, R11= FFFFFFFE
R12= E0001000
SP(R13)= 20100CB0, MSP= 20100CB0, PSP= 00000000, R14(LR) = 0000036D
XPSR = 01000000: APSR = nzcvq, EPSR = 01000000, IPSR = 000 (NoException)
CFBP = 00000000, CONTROL = 00, FAULTMASK = 00, BASEPRI = 00, PRIMASK = 00

FPS0 = 00000000, FPS1 = 00000000, FPS2 = 00000000, FPS3 = 00000000
FPS4 = 00000000, FPS5 = 00000000, FPS6 = 00000000, FPS7 = 00000000
FPS8 = 00000000, FPS9 = 00000000, FPS10= 00000000, FPS11= 00000000
FPS12= 00000000, FPS13= 00000000, FPS14= 00000000, FPS15= 00000000
FPS16= 00000000, FPS17= 00000000, FPS18= 00000000, FPS19= 00000000
FPS20= 00000000, FPS21= 00000000, FPS22= 00000000, FPS23= 00000000
FPS24= 00000000, FPS25= 00000000, FPS26= 00000000, FPS27= 00000000
FPS28= 00000000, FPS29= 00000000, FPS30= 00000000, FPS31= 00000000E00D, PID: 003BB001 ITM
ROMTbl[0][4]: E0040000, CID: B105900D, PID: 000BB9A1 TPIU
Cortex-M4 identified.
J-Link>
```

De là, on peut utiliser `halt`, `go`, etc.