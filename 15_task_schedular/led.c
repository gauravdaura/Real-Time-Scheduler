#include<stdint.h>
#include "led.h"
#include "hwdefs.h"

//-------------------------------------------------------
// FUNCTION - delay(uint32_t count)
// PURPOSE - delay
//-------------------------------------------------------

void delay(uint32_t count)
{
  uint32_t i;                   // PROBLEM
  for (i = 0 ; i < count ; i++);
}

//-------------------------------------------------------
// FUNCTION - LED_Init( void )
// PURPOSE - Initialize LED
//-------------------------------------------------------

void LED_Init( void )
{
    HWREG( SYSCTL_RCGCGPIO ) |= 0x20;
    HWREG( GPIO_PORTF_BASE + GPIO_O_DEN ) |= ( LED_LED1 | LED_LED2 | LED_LED3 );
    HWREG( GPIO_PORTF_BASE + GPIO_O_DIR ) |= ( LED_LED1 | LED_LED2 | LED_LED3 );
}

void LED_Control (uint8_t uiLED, uint8_t uiState )
{
    switch( uiState )
    {
    case LED_ON:
        HWREG( GPIO_PORTF_BASE + GPIO_O_DATA + (uiLED << 2)) =  uiLED;
        break;
    case LED_OFF:
        HWREG( GPIO_PORTF_BASE + GPIO_O_DATA + (uiLED << 2)) =  0;
        break;
    default:
        HWREG( GPIO_PORTF_BASE + GPIO_O_DATA + (uiLED << 2)) ^= 0;
    }
}
