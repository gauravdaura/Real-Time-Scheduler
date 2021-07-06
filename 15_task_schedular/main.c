//----------------------------------------------------------
// HEADER FILES
//----------------------------------------------------------

#include <stdint.h>
#include "hwdefs.h"
#include "schedular.h"
#include "systick.h"
#include "led.h"

//----------------------------------------------------------------------------
// FUNCTION - Initialize( void );
// PURPOSE - Initialize all the modules
//----------------------------------------------------------------------------

void Initialize( void )
{
    LED_Init();                                   //7

    Enable_Processor_Faults();                  // 4

    Init_Schedular_Stack(SCHDR_STACK_START);    // 2

    Init_Task_Stack();                          // 3

    SYSTICK_Init();                             // 1
}

//----------------------------------------------------------
// FUNCTION - MAIN()
// PURPOSE - to run the processor in the thread mode
//----------------------------------------------------------

int main(void)
{
    Initialize();

    Switch_SP_To_PSP();                         // 5

    task1_handler();                            // 6

    return 0;
}
