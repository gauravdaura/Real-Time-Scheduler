//----------------------------------------------------------
// INCLUSION LOCK
//----------------------------------------------------------

#ifndef HWDEFS_H_
#define HWDEFS_H_

//----------------------------------------------------------------------------
// MACROS
//----------------------------------------------------------------------------

#define HWREG( x ) ( *( ( volatile uint32_t * )( x ) ) )

#define SRAM_START      0x20000000U
#define SRAM_SIZE       ( (32) * (1024) )
#define SRAM_END        ((SRAM_START) + (SRAM_SIZE))

#define STACK_TASK_SIZE       1024U
//#define STACK_SCHDLR_SIZE   1024U

#define T1_STACK_START      SRAM_END
#define T2_STACK_START      ((SRAM_END) - (1 * STACK_TASK_SIZE))
#define T3_STACK_START      ((SRAM_END) - (2 * STACK_TASK_SIZE))
#define T4_STACK_START      ((SRAM_END) - (3 * STACK_TASK_SIZE))
#define IDLE_STACK_START    ((SRAM_END) - (4 * STACK_TASK_SIZE))
#define SCHDR_STACK_START   ((SRAM_END) - (5 * STACK_TASK_SIZE))

#define NVIC_STCTRL         0xE000E010
#define NVIC_RELOAD         0xE000E014
#define NVIC_CURRENT        0xE000E018
#define SYSHNDCTRL          0xE000ED24

#define MAX_TASKS           5
#define DUMMY_XPSR          0x01000000U

#define SYSCTL_RCGCGPIO     0x400FE608
#define GPIO_PORTF_BASE     0x40025000

#define GPIO_O_DIR          0x00000400
#define GPIO_O_DATA         0x00000000
#define GPIO_O_DEN          0x0000051C

#define TASK_BLOCKED_STATE  0x00
#define TASK_READY_STATE    0xFF

#define ICSR                0xE000ED04

//#define INTERRUPT_DISABLE   __asm( " MOV R0, 0x01");
//#define INTERRUPT_ENABLE    __asm( " MOV R0, 0x00");

#endif  //  HWDEFS_H_

//----------------------------------------------------------
// END HWDEFS.H
//----------------------------------------------------------
