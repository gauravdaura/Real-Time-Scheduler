//----------------------------------------------------------------------------
// INCLUDE FILES
//----------------------------------------------------------------------------

#include <stdint.h>
#include "schedular.h"
#include "hwdefs.h"
#include "led.h"

//----------------------------------------------------------------------------
// GLOBAL VARIABLES
//----------------------------------------------------------------------------

uint8_t current_task = 1;

uint32_t g_tick_count = 0;

//----------------------------------------------------------------------------
// FUNCTION - TASK HANDLERS
// PURPOSE - to perform and executes a task
//----------------------------------------------------------------------------

void idle_handler(void)
{
    while (1)
    {
    }
}

void task1_handler(void)
{
    while (1)
    {
        LED_Control( LED_LED1, LED_ON );
        task_delay(1000);
        LED_Control( LED_LED1, LED_OFF );
        task_delay(1000);
    }
}

void task2_handler(void)
{
    while (1)
    {
        //LED_Control( LED_LED2, LED_ON );
        //task_delay(500);
        //LED_Control( LED_LED2, LED_OFF );
        //task_delay(500);
    }
}

void task3_handler(void)
{
    while (1)
    {
        //LED_Control( LED_LED3, LED_ON );
        //task_delay(250);
        //LED_Control( LED_LED3, LED_OFF );
        //task_delay(250);
    }
}

void task4_handler(void)
{
    while (1)
    {
    }
}

//----------------------------------------------------------------------------
// FUNCTION - Init_Schedular_STACK( void )
// PURPOSE - to  initialize Scheduler Stack
//----------------------------------------------------------------------------

__attribute__((naked)) Init_Schedular_Stack(uint32_t top_of_stack)
{
    __asm (" MSR PSP, R0");
    __asm (" BX LR");
}

void Init_Task_Stack(void)
{
    int i, j;

    user_tasks[0].current_state = TASK_READY_STATE;
    user_tasks[1].current_state = TASK_READY_STATE;
    user_tasks[2].current_state = TASK_READY_STATE;
    user_tasks[3].current_state = TASK_READY_STATE;
    user_tasks[4].current_state = TASK_READY_STATE;

    user_tasks[0].psp_value = IDLE_STACK_START;
    user_tasks[1].psp_value = T1_STACK_START;
    user_tasks[2].psp_value = T2_STACK_START;
    user_tasks[3].psp_value = T3_STACK_START;
    user_tasks[4].psp_value = T4_STACK_START;

    user_tasks[0].task_handlers = idle_handler;
    user_tasks[1].task_handlers = task1_handler;
    user_tasks[2].task_handlers = task2_handler;
    user_tasks[3].task_handlers = task3_handler;
    user_tasks[4].task_handlers = task4_handler;

    uint32_t *pPSP;     // Dummy Stack Initialization

    for (i = 0; i < MAX_TASKS; i++)
    {
        pPSP = (uint32_t*) user_tasks[i].psp_value;

        pPSP--;
        *pPSP = DUMMY_XPSR;     // 0x01000000

        pPSP--;
        *pPSP = (uint32_t) user_tasks[i].task_handlers;

        pPSP--;
        *pPSP = 0xFFFFFFFD;     // PC

        for (j = 0; j < 13; j++)
        {
            pPSP--;
            *pPSP = 0;
        }

        user_tasks[i].psp_value = (uint32_t) pPSP;
    }

}

//----------------------------------------------------------------------------
// FUNCTION - Switch_SP_To_PSP( void ) and other functions
// PURPOSE - to change Stack Pointer to PSP
//----------------------------------------------------------------------------

__attribute__((naked)) Switch_SP_To_PSP(void)
{
    // Initialize the PSP with task 1 stack start addr
    __asm (" PUSH {LR}");
    __asm (" BL Get_Current_PSP");
    __asm (" MSR PSP, R0");
    __asm (" POP {LR}");

    // change SP to PSP using the control register
    __asm (" MRS R0, CONTROL");
    __asm (" ORR R0, #0x02");
    __asm (" MSR CONTROL, R0");
    __asm (" BX LR");

}

uint32_t Get_Current_PSP(void)
{
    return user_tasks[current_task].psp_value;
}

void Save_PSP_Value(uint32_t current_psp_value)
{
    user_tasks[current_task].psp_value = current_psp_value;
}

void Update_Next_Task(void)
{
    uint32_t state = TASK_BLOCKED_STATE;
    uint8_t i;
    for ( i = 0; i < MAX_TASKS; i++)
    {
        current_task++;
        current_task %= MAX_TASKS;      // ROUND-ROBIN FASHION
        state = user_tasks[current_task].current_state;
        if ( (state == TASK_READY_STATE) && (current_task != 0) )
            break;
    }

    if ( state != TASK_READY_STATE)
            current_task = 0;

}

//----------------------------------------------------------------------------
// FUNCTION - Enable_Processor_Faults()
// PURPOSE - to get the details of the fault
//----------------------------------------------------------------------------

void Enable_Processor_Faults()
{
    HWREG(SYSHNDCTRL) = (1 << 16) | (1 << 17) | (1 << 18);
}

//-------------------------------------------------------
// FUNCTION - task_delay(uint32_t tick_count)
// PURPOSE - task delay
//-------------------------------------------------------

void task_delay(uint32_t tick_count)
{
    // Disable interrupt
    __asm (" MOV R0, #1");
    __asm (" MSR PRIMASK, R0");

    if (current_task)
    {
    user_tasks[current_task].block_count = g_tick_count + tick_count;
    user_tasks[current_task].current_state = TASK_BLOCKED_STATE;
    }
    //schedule();

    // Enable interrupt
    __asm (" MOV R0, #0");
    __asm (" MSR PRIMASK, R0");
}

//-------------------------------------------------------
// FUNCTION - __attribute__((naked)) void PendSV_Handler (void)
// PURPOSE - Context Switch
//-------------------------------------------------------

__attribute__((naked)) void PendSV_Handler(void)
{
    /*  Switch Task-1 to Task-2 */

    // Save the context of current task
    __asm (" MRS R0, PSP");
    __asm (" STMDB R0!, {R4-R11}");
    __asm (" PUSH {LR}");
    // because BL in next instruction
    __asm (" BL Save_PSP_Value");

    //Retrieve the context of next task
    __asm (" BL Update_Next_Task");
    //current task++ and %
    __asm (" BL Get_Current_PSP");
    __asm (" LDMIA R0!, {R4-R11}");
    __asm (" MSR PSP, R0");
    __asm (" POP {LR}");
    __asm (" BX LR");
}

//-------------------------------------------------------
// FUNCTION - void update_global_tick( void )
// PURPOSE - update global tick count
//-------------------------------------------------------

void update_global_tick(void)
{
    g_tick_count++;
}

void check_unblock_task(void)
{
    uint8_t i;
    for (i = 1; i < MAX_TASKS; i++)
    {
        if (user_tasks[i].current_state != TASK_READY_STATE)
        {
            if (user_tasks[i].block_count == g_tick_count)
            {
                user_tasks[i].current_state = TASK_READY_STATE;
            } } } }

void schedule (void)
{
    HWREG( ICSR ) = (1 << 28);      // PEND ( PendSV is always Enabled)
}

//-------------------------------------------------------
// FUNCTION - SYSTICK_Int_Handler (void) AND Other Handlers
// PURPOSE - handler Interrupt of SysTick
//-------------------------------------------------------

void SYSTICK_IntHandler(void)
{
    update_global_tick();       // update global tick count
    check_unblock_task();       // check for unblock task
    schedule();                 // to pend the PendSV

}

void MEM_IntHandler(void)
{
    while (1)
    {
    }
}
void BUS_IntHandler(void)
{
    while (1)
    {
    }
}
void USAGE_IntHandler(void)
{
    while (1)
    {
    }
}
