	.arch msp430g2553
	.p2align 1,0
	.text



				; r12 = col
				; r13 = row
				; r14 = color
	

	.global drawDiamond
drawDiamond:
	sub #8, r1
	mov #0, 0(r1)		; r = 0
	mov #0, 2(r1) 		; c = 0

	mov r12, 4(r1)
	mov r13, 6(r1)

	
outerloop:
	cmp.b #10, 0(r1)
	jc exit 		; r >= 10 exit
	add.b #1, 0(r1)		; r++
	
innerloop:
	cmp.b 0(r1), 2(r1) 	; c <= r
	jc outerloop

	sub.b 2(r1), r12
	sub.b 0(r1), r13
	sub.b #1, r13
	call #drawPixel
	
	mov 6(r1), r13
	add.b 0(r1), r13
	sub.b #20, r13
	
	call #drawPixel

	mov 4(r1), r12
	add.b 2(r1), r12

	call #drawPixel

	mov 6(r1), r13
	sub.b 0(r1), r13
	sub.b #1, r13

	call #drawPixel

	mov 4(r1), r12
	mov 6(r1), r13
	
	add.b #1, 2(r1)
	jmp innerloop
exit:
	add #8, r1
	pop r0
