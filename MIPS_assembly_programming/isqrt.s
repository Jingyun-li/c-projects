# author:	Jingyun Li   
# UID number:	117242874 
# Directory ID:	jingyli 

isqrt:
	# PROLOGUE
	subu	$sp, $sp, 8		# expand the stack by 8 bytes
	sw	$ra, 8($sp)		# push $ra on stack
	sw	$fp, 4($sp)		# push $sp on stack
	addu	$fp, $sp, 8		# set $fp to the saved $ra

	# BODY
	subu    $sp, $sp, 4             # expand the stack by 4 bytes
        sw      $a0, 4($sp)             # push $a0 on stack

	move	$v0, $a0
	blt	$a0, 2, ret		# if n is less than 2, jump to return
	
	srl 	$a0, $a0, 2
	jal	isqrt
	sll	$t0, $v0, 1		# left shift n by 1 and save it in $t0
	add	$t1, $t0, 1		# represent large by $t1
	mul	$t2, $t1, $t1		# multiply large and save it in $t2
	lw	$a0, 4($sp)		# restore $a0 from stack
	bgt	$t2, $a0, rets		# branch to rets if $t2 is less than n
	j	retl
	
rets:
	move 	$v0, $t0
	j	ret			# set the return value to small

retl:
	move	$v0, $t1		# set the return value to large

ret:
	# EPILOGUE
	move	$sp, $fp           	# restore $sp
	lw	$ra, ($fp)		# restore saved $ra
	lw	$fp, -4($sp)		# restore saved $fp
	jr	$ra			# return to kernel
