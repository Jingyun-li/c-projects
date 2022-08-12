# author:	Jingyun Li   
# UID number:	117242874 
# Directory ID:	jingyli 

strlen:
	li	$v0, 0			# initiate $v0 with 0

strloop:
	lb	$t1, 0($a0)		# load a byte each time
	beqz	$t1, end		# goto end if the byte is null
	add	$v0, $v0, 1		# increase the count of the length by one each time
	add	$a0, $a0, 1		# increase the address by one each time
	j	strloop

end:
        jr	$ra

is_palindrome:
	# PROLOGUE
	subu	$sp, $sp, 8		# expand stack by 8 bytes
	sw	$ra, 8($sp)		# push $ra
	sw 	$fp, 4($sp) 		# push $fp
	addu	$fp, $sp, 8		# set $fp to saved $ra

	# BODY
	subu	$sp, $sp, 4		# expand stack by 4 bytes
	sw	$a0, 4($sp)		# push $a0

	jal	strlen			# call function strlen
	lw	$t0, 4($sp)		# load the address of the first element from stack
	add	$t1, $t0, $v0		
	sub	$t1, $t1, 1		# get the address of the last element

loop:
	bge 	$t0, $t1, ret1		# the loop breakes if number count from the beginning is greater
	lb 	$t2, 0($t0)		# load byte from the beginning
	lb 	$t3, 0($t1)		# load byte from the end
	bne 	$t2, $t3, ret0		# if the value of both bytes are different, jump to ret0
	add 	$t0, $t0, 1		# add the element by one
	sub 	$t1, $t1, 1		# decrease the element by one

	j	loop

ret1:
	li 	$v0, 1			# set the return value to 1
	j 	ret

ret0:
	li 	$v0, 0			# set the return value to 0

ret:
	# EPILOGUE
	move 	$sp, $fp		# restore $sp
	lw   	$ra, ($fp)		# restore saved $ra
	lw   	$fp, -4($sp)		# restore saved $fp
	j    	$ra			# return to kernel	
