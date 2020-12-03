#ifndef stateMachine_included
#define state_Machine_included

extern char lcdState;
extern short freq;
extern short state2_status;

void state_advance();
char toggle_red();
char toggle_green();
void up_state();
void down_state();
void buzzer_advance();
void dimLights(char x);
void lcd_update();
void lcd_state(int color);
void clearLcd();
#endif // included
