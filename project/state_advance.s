		.arch msp430g2553
		.p2align 1,0
		.text
	
jt:
	.word exit
	.word case1
	.word case2
	.word case3
	.word case4

	.data
	.extern super_state
	.extern leds_changed

	.text
	.global state_advance
state_advance:
	sub #2, r1
	mov #0, 0(r1)		; changed = 0;
	cmp.b #5, &super_state
	jhs exit
	mov.b &super_state, r12
	add.b r12, r12
	mov jt(r12), r0
	
case1:  call #state1
	mov.b r12, 0(r1) 	; changed = state1()
        jmp exit
case2:  call #state2
	mov.b r12, 0(r1) 	; changed = state2()
        jmp exit
case3:  call #state3
	mov.b r12, 0(r1)	; changed = state3()
	jmp exit
case4:  call #state4
	mov.b r12, 0(r1)	; changed = state4()
	jmp exit
exit:
	mov.b 0(r1), &leds_changed ; leds_changed = changed
	add #2, r1		   ; move stack pointer
	call #led_update	   ; call led_update
	pop r0			   ; return to caller


	
