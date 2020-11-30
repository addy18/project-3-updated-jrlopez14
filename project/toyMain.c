#include <msp430.h>
#include <libTimer.h>
#include "switches.h"
#include "stateMachines.h"
#include "lcdutils.h"
#include "lcddraw.h"
#include "buzzer.h"
#include "led.h"

short redrawScreen = 1;
static short previous_state = 0;

u_char centerWidth = screenWidth/2 + 1;
u_char centerHeight = screenHeight/2 +1;

void drawDiamond(u_char col, u_char row, u_char size, u_int color){
  for (u_char r = 0; r < size; r++){
    for (u_char c = 0; c <= r; c++){
      drawPixel(col-c, row-r-1, color);
      drawPixel(col-c, row+r-(2*size), color);
      drawPixel(col+c, row-r-1,color);
      drawPixel(col+c, row+r-(2*size), color);
    }
  }
}

void wdt_c_handler()
{
  static int s1Count = 0;
  static int s2Count = 0;
  static int sCount = 0;
  static int secCount = 0;

  if (++ secCount == 250){
    secCount = 0;
    redrawScreen = 1;
  }
  if (super_state != 3 && previous_state != super_state) redrawScreen = 1;
  if(super_state == 1){
    if (++ s1Count == 125) {
      state_advance();
      s1Count = 0;
    }
  } else if (super_state == 2) {
    if ( (++ s2Count % 25) == 0) buzzer_advance();
    if (s2Count == 250){
      state_advance();
      s2Count = 0;
    }
  } else{
    state_advance();
  }
}

void main()
{
  led_init();
  P1OUT |= LED_GREEN;
  configureClocks();

  lcd_init();
  switch_init();
  buzzer_init();

  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x08);	              /**< GIE (enable interrupts) */
  clearScreen(COLOR_BLACK);

  if (isNegative(2)) drawString5x7(100,0, "2 is not negative", COLOR_RED, COLOR_BLACK);
  
  while(1) {

    if (redrawScreen = 1) {
      redrawScreen = 0;


      static char color_state = 0;
      u_int COLOR;
      
      switch(color_state){
      case 0: COLOR = COLOR_RED; color_state = 1;  break;
      case 1: COLOR = COLOR_BLUE; color_state = 0; break;
      }
      
      switch(super_state){
      case 0: drawString5x7(0,0,"Project 3:", COLOR_WHITE, COLOR_BLACK); break;
      case 1:
	lcd_state(COLOR_RED);
	previous_state = 1;
 	break;
      case 2:
	lcd_state(COLOR_BLUE);
	previous_state = 2;
	break;
      case 3:
	previous_state = 3;
	break;
      case 4:
	if (previous_state != 4) clearLcd();
	previous_state = 4;
	break;
      }
    }
		     
  
    P1OUT &= ~LED_GREEN;
    or_sr(0x10);
    P1OUT |= LED_GREEN;

  }
}

    
    



