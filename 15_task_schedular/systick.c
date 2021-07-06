//-------------------------------------------------------
// INCLUDE FILES
//-------------------------------------------------------

#include "systick.h"
#include "hwdefs.h"

//-------------------------------------------------------
// FUNCTION - SYSTICK_Init( void )
// PURPOSE - Initialize SysTick
//-------------------------------------------------------

void SYSTICK_Init( void )
{
    // 1. Program reload value.
    HWREG( NVIC_RELOAD ) = 16000;

    // 2. Clear current value.
    HWREG( NVIC_CURRENT ) = 0;

    // 3. Program Control and Status register
    HWREG( NVIC_STCTRL ) |= 0x07;
}
