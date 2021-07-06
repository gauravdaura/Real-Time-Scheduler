//----------------------------------------------------------
// INCLUSION LOCK
//----------------------------------------------------------

#ifndef SCHEDULAR_H_
#define SCHEDULAR_H_

//----------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------

#include <stdint.h>
#include "hwdefs.h"

//----------------------------------------------------------------------------
// GLOBAL VARIABLES
//----------------------------------------------------------------------------

uint32_t g_uiSYSFLAGS;  // System Flag

typedef struct
{
    uint32_t psp_value;
    uint32_t block_count;
    uint8_t current_state;
    void (*task_handlers)(void);
} TCB_t;

TCB_t user_tasks[MAX_TASKS];

//----------------------------------------------------------
// FUNCTION PROTOTYPES
//----------------------------------------------------------

void task1_handler(void);
void task2_handler(void);
void task3_handler(void);
void task4_handler(void);

__attribute__((naked)) Init_Schedular_Stack( uint32_t top_of_stack);
void Init_Task_Stack(void);
void Enable_Processor_Faults(void);
__attribute__((naked)) Switch_SP_To_PSP( void );
uint32_t Get_Current_PSP(void);
void Save_PSP_Value(uint32_t current_psp_value);
void Update_Next_Task(void);

void task_delay(uint32_t tick_count);
__attribute__((naked)) void PendSV_Handler(void);
void update_global_tick(void);
void check_unblock_task(void);
void schedule (void);

#endif

//----------------------------------------------------------
// END SCHEDULAR.H
//----------------------------------------------------------
