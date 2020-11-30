	.arch msp430g2553
	.p2align 2
	.text

	.data
	.extern freq
	.extern state2_status

	.text

	.global buzzer_advance

	; if state2_status is 0, down state. otherwise upstate
buzzer_advance:
	cmp #0, &state2_status
	jz else

if:
	add #225, &freq
	jmp exit
else:
	sub #450, &freq
	
exit:
	mov &freq, r12
	call #get_period 	; calls get period from c, which does 2000000 / r12
				; then calls buzzer_set_period with this value
	call #buzzer_set_period
	pop r0
