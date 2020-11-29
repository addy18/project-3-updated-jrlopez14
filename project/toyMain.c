#include <msp430.h>
#include <libTimer.h>
#include "switches.h"
#include "stateMachines.h"
#include "lcdutils.h"
#include "lcddraw.h"
#include "buzzer.h"
#include "led.h"

short redrawScreen = 1;
short previousState = 0;

u_char centerWidth = screenWidth/2 + 1;
u_char centerHeight = screenHeight/2 +1;

void drawDiamond(u_char col, u_char row, u_int color);

void wdt_c_handler()
{
  static int s1Count = 0;
  static int s2Count = 0;
  static int sCount = 0;
  static int secCount = 0;
  secCount ++;
  if(secCount == 250) {
    secCount = 0;
    redrawScreen = 1;
  }
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

  while(1) {

    if (redrawScreen = 1) {
      redrawScreen = 0;
      
      switch(super_state){
      case 0: drawString5x7(0,0,"Project 3:", COLOR_WHITE, COLOR_BLACK); break;
	
      case 1:
	for(u_char r = 0; r < 10; r++){
	  for(u_char c = 0; c <= r; c++){
	    drawDiamond(centerWidth, centerHeight, COLOR_RED);
	    // drawPixel(centerWidth-c,centerHeight-r-1,COLOR_RED);
	    // drawPixel(centerWidth+c,centerHeight-r-1,COLOR_RED);
	    // drawPixel(centerWidth-c,centerHeight+r-20,COLOR_RED);
	    // drawPixel(centerWidth+c,centerHeight+r-20,COLOR_RED);
	  }
	}
	previousState = 1;
 	break;
      case 2:
	drawString5x7(40,40,"Hello", COLOR_RED, COLOR_BLACK);
	previousState = 2;
	break;
      case 3:
	drawString5x7(40,40,"Hello", COLOR_YELLOW, COLOR_BLACK);
	previousState = 3;
	break;
      case 4:
	if (previousState == 1) 
	if (previousState == 3) drawString5x7(40,40,"Hello",COLOR_BLACK,COLOR_BLACK);
	
	
	drawString11x16(centerWidth-(4*11), centerHeight,"Goodbye!", COLOR_RED,COLOR_BLACK);
	previousState = 4;
	break;
      }
    }
		     
  
    P1OUT &= ~LED_GREEN;
    or_sr(0x10);
    P1OUT |= LED_GREEN;

  }
}

    
    



