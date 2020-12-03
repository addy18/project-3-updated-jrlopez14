

	

	.text
	.global isPositive
isPositive:
	cmp #0, r12
	jge Positive

Negative:
	mov #0, r12
	jmp exit

Positive:
	mov #1, r12
exit:
	pop r0
