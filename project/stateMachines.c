#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include "switches.h"
#include "stateMachines.h"
#include "led.h"
#include "buzzer.h"

short freq = 500; // Initial frequency of state 2.
short state2_status = 1; // Initial state for state 2.

void buzzer_advance();

char super_state = 0;
void drawDiamond(u_char col, u_char row, u_char size, u_int color);

// didnt use, but was would be for assembly division by 2
u_char halfNumber(u_char num){
  return num/2;
}

//draws an arrangement of diamonds and prints jeremiah on screen
void lcd_state(int COLOR){

  u_char centerWidth = screenWidth/2 + 1;
  u_char centerHeight = screenHeight/2 + 1;


  //drawDiamond(centerWidth, centerHeight-20, 10, COLOR);
  //drawDiamond(centerWidth-20, centerHeight, 10, COLOR);
  //drawDiamond(centerWidth, centerHeight+20, 10, COLOR);
  //drawDiamond(centerWidth+20, centerHeight, 10, COLOR);
  drawDiamond(centerWidth, centerHeight, 10, COLOR_WHITE);

  drawString8x12(centerWidth-(8*4), centerHeight+40, "Jeremiah", COLOR, COLOR_BLACK);
}

// draws black diamonds over colored diamonds to erase them
void clearLcd(){
  u_char centerWidth = screenWidth/2 + 1;
  u_char centerHeight = screenHeight/2 + 1;

  drawDiamond(centerWidth, centerHeight-20, 10, COLOR_BLACK);
  drawDiamond(centerWidth-20, centerHeight, 10, COLOR_BLACK);
  drawDiamond(centerWidth, centerHeight + 20, 10, COLOR_BLACK);
  drawDiamond(centerWidth+20, centerHeight, 10, COLOR_BLACK);
  drawDiamond(centerWidth, centerHeight, 10, COLOR_BLACK);

  drawString8x12(centerWidth-(8*4), centerHeight+40, "Jeremiah", COLOR_BLACK, COLOR_BLACK);
}

// Toggling red 

char state1()
{
  static char stateS1 = 1;

  switch (stateS1) {
  case 0:
    red_on = 1;
    buzzer_set_period(1000);
    stateS1 = 1;
    break;
  case 1:
    red_on = 0;
    buzzer_set_period(0);
    stateS1 = 0;
    break;
  }
  
  return 1;
}


/* Pitch rises from 500 Hz to 5000 Hz with red led on in 2s with an pitch increase intervals of 1every 1/10 of sec. Pitch decreases from 5000 Hz to 500 Hz with green led on in 1 sec, with pitch decrease intervals of 1/10 sec */

char state2()
{
  static short stateS2 = 0;

  switch(stateS2){
  case 0:
  case 1: up_state(); stateS2++; break;
  case 2: down_state(); stateS2 = 0;
  default: break;
  }
  return 1;
}

void up_state()
{
  state2_status = 1; red_on = 1; 
  leds_changed = 1; led_update();
}
void down_state()
{
  state2_status = 0; red_on = 0; 
  leds_changed= 1; led_update();
}

// Now in buzzer_advance.s
// If state2_status == 1, increase pitch. Otherwise, decrease pitch. 
/*void buzzer_advance(){
  if (state2_status) freq += 225;
  else freq -= 450;
  
  short cycles = 2000000/freq;  // 2000000 / frequency gives us the correct period.
  buzzer_set_period(cycles);
  }*/

short get_period(short freq){
  return 2000000/freq;
}

// Cycles from no dim, to 50% dim to 33% dim, to 25% dim, to 12.5% dim, to 6.25% dim.
// Frequency of buzzer decreases with each dim.
char state3()
{
  static short s3Counter = 0;
  static short stateS3 = 0;
  if (++s3Counter == 125) { stateS3++; s3Counter = 0;}
  switch (stateS3){
  case 0: dimLights(1); buzzer_set_period(0); break;
  case 1: dimLights(2); buzzer_set_period(500); break;
  case 2: dimLights(3); buzzer_set_period(1500); break;
  case 3: dimLights(4); buzzer_set_period(6000); break;
  case 4: dimLights(8); buzzer_set_period(24000); break;
  case 5: dimLights(16); buzzer_set_period(0); stateS3 = 0; break;
  }
}

char modByX(char x, char dimCount){
  return dimCount % x;
}

// Leds are on 1 of of x times.
void dimLights(char x){
  
  static char dimCount = 0;
  switch(dimCount % x){
  case 0: red_on = 1;  dimCount++; break;
  case 1: red_on = 0;  dimCount++; break;
  default: red_on = 0; dimCount++; break;
  }
  leds_changed = 1;
  led_update();
  }

// Leds and buzzer off.
char state4(){
  buzzer_set_period(0);
  red_on = 0;
  leds_changed = 1;
  led_update();
  return 1; 
}

// Now in state_advance.s
// Changes state when super_state is changed.
/* void state_advance()
{ 
  char changed = 0; 
  switch(super_state){    
  case 1:
    changed = state1(); // Toggling leds and buzzer.
    break;
  case 2:
    changed = state2(); // Siren
    break;
  case 3:
    changed = state3(); // Dimming Lights.
    break;
  case 4:
    changed = state4(); // Leds and Buzzer off.
    break;
  }
  leds_changed = changed;
  led_update();
  } */




