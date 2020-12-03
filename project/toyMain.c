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

// if positive true, otherwise false
int isPositive(int num);

// draws a diamond centered at the col and row with the size and color given
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

void drawMenu(u_int COLOR)
{
  drawString5x7(0,0,"Project 3:", COLOR_WHITE, COLOR_BLACK);
  drawString5x7(0,(screenHeight/2 - 21), "SW1 Red Diamonds", COLOR, COLOR_BLACK);
  drawString5x7(0, screenHeight/2, "SW2 Blue Diamonds", COLOR, COLOR_BLACK);
  drawString5x7(0,(screenHeight/2 + 21), "SW4 Clear Diamonds", COLOR, COLOR_BLACK);
}	
// Interrupt handler from p2 + a counter that sets redraw screen to 1, every second
void wdt_c_handler()
{
  static int s1Count = 0;
  static int s2Count = 0;
  static int sCount = 0;
  static int secCount = 0;

  /*if (++ secCount == 250){
    secCount = 0;
    redrawScreen = 1;
    }*/
  if (previous_state != super_state) redrawScreen = 1;
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


// State 1: Red Diamonds, Red Jeremiah
// State 2: Blue Diamonds, Blue Jeremiah

// State 3: Same screen as previous state
// State 4: Erases diamonds and Jeremiah
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

  if (isPositive(0) != 0) {
    drawString5x7(7,screenHeight-20, "2 > 0 !!", COLOR_RED, COLOR_BLACK);
  }
  if (isPositive(-2) == 0) {
    drawString5x7(0, screenHeight-10, "-1 < 0 !!", COLOR_BLUE, COLOR_BLACK);
  }
  while(1) {

    if (redrawScreen = 1) {
      redrawScreen = 0;

      switch(super_state){
      case 0:
	drawString5x7(0,0,"Project 3:", COLOR_WHITE, COLOR_BLACK);
	drawMenu(COLOR_WHITE);
        break;
      case 1:
	if(previous_state == 0) drawMenu(COLOR_BLACK);
	lcd_state(COLOR_RED);
	previous_state = 1;
 	break;
      case 2:
	if(previous_state == 0) drawMenu(COLOR_BLACK);
        lcd_state(COLOR_BLUE);
	previous_state = 2;
	break;
      case 3:
	//lcd_state(COLOR_PURPLE);
	previous_state = 3;
	break;
      case 4:
	if (previous_state == 0) drawMenu(COLOR_BLACK);
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

    
    



