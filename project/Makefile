# makfile configuration
CPU             	= msp430g2553
CFLAGS          	= -mmcu=${CPU} -Os -I../h
LDFLAGS		= -L../lib -L/opt/ti/msp430_gcc/include/ 

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-gcc -mmcu=${CPU} -c

all:toyMain.elf

#additional rules for files
toyMain.elf: ${COMMON_OBJECTS} toyMain.o wdt_handler.o led.o switches.o buzzer.o p2_interrupt_handler.o stateMachines.o state_advance.o buzzer_advance.o random_assy.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd 

load: toyMain.elf
	msp430loader.sh $^

clean:
	rm -f *.o *.elf
