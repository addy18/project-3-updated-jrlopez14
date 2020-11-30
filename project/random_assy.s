


	.global isNegative
isNegative:
	cmp #0, r12
	jge exit

Negative:
	mov #0, r12	
exit:
	pop r0
