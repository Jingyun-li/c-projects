# author:	Jingyun Li   
# UID number:	117242874 
# Directory ID:	jingyli 

reverse_prefix_sum:
	# PROLOGUE
	subu	$sp, $sp, 8		# expand the stack by 8 bytes
	sw	$ra, 8($sp)		# push $ra on stack
	sw	$fp, 4($sp)		# push $sp on stack
	addu	$fp, $sp, 8		# set $fp to the saved $ra

	# BODY
  	subu    $sp, $sp, 8             # expand the stack by 4 bytes
	sw	$s0, 8($sp)
        sw      $a0, 4($sp)             # push $a0 on stack

	li	$v0, 0			# initiate $v0 with 0
	lw	$s0, 0($a0)
	beq	$s0, -1, ret		# branch to ret if $a0 is equal to -1
	add	$a0, $a0, 4		# get the next element of the array
	jal	reverse_prefix_sum
	addu	$v0, $v0, $s0		# put the n after reverse_prefix_sum and n together in $a0
	lw	$a0, 4($sp)
	sw	$v0, 0($a0)

ret:
	# EPILOGUE
	lw	$s0, 8($sp)
	move	$sp, $fp
	lw	$ra, ($fp)		# restore saved $ra
	lw	$fp, -4($sp)		# restore saved $fp
	jr	$ra			# return to kernel
