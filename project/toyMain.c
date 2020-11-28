#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "buzzer.h"
#include "led.h"

short redrawScreen = 1;

void wdt_c_handler()
{
  static int secCount = 0;
  if (secCount == 250) {
    secCount = 0;
    redrawScreen = 1;
  }
}

void main()
{
  configureClocks();
  led_init();
  lcd_init();
  buzzer_init();
  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x08);	              /**< GIE (enable interrupts) */
  clearScreen(COLOR_BLACK);
}

    
    



