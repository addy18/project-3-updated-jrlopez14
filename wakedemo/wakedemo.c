#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

#define LED_GREEN BIT6             // P1.6


short redrawScreen = 1;
u_int fontFgColor = COLOR_GREEN;

long COLOR1;
long COLOR2;

void wdt_c_handler()
{
  static int secCount = 0;

  secCount ++;
  if (secCount == 250) {		/* once/sec */
    secCount = 0;
    fontFgColor = (fontFgColor == COLOR_GREEN) ? COLOR_BLACK : COLOR_GREEN;
    redrawScreen = 1;
  }
}
  

void main()
{
  P1DIR |= LED_GREEN;		/**< Green led on when CPU on */		
  P1OUT |= LED_GREEN;
  configureClocks();
  lcd_init();
  
  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */
  
  clearScreen(COLOR_BLACK);

  u_char centerWidth = (screenWidth/2) + 1;
  u_char centerHeight = (screenHeight/2) + 1;
  
  
  static u_char colorState = 0;
  
  while (1) {			/* forever */
    if (redrawScreen) {
      redrawScreen = 0;

      switch(colorState){
      case 0: COLOR1 = COLOR_RED;    COLOR2 = COLOR_WHITE; colorState = 1; break;
      case 1: COLOR1 = COLOR_WHITE;  COLOR2 = COLOR_RED;   colorState = 0; break;
      }
      
      for(u_char r = 0; r < 10; r++){
	for(u_char c = 0; c <= r; c++){
	  drawPixel(centerWidth-c, centerHeight-r-1, COLOR1);
	  drawPixel(centerWidth+c, centerHeight-r-1, COLOR1);
	  drawPixel(centerWidth-c, centerHeight+r-20, COLOR2);
	  drawPixel(centerWidth+c, centerHeight+r-20, COLOR2);
	}
      }

      drawString11x16(20,20,"Hello", COLOR1, COLOR2);


      
    }
      
    P1OUT &= ~LED_GREEN;	/* green off */
    or_sr(0x10);		/**< CPU OFF */
    P1OUT |= LED_GREEN;		/* green on */
    
  }
}

    
    



