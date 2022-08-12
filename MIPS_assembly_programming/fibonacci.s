# Author:    Jingyun Li   
# UID number:    117242874 
# Directory ID:    jingyli 

fibonacci:
    # PROLOGUE
    subu    $sp, $sp, 8        # expand stack by 8 bytes
    sw      $ra, 8($sp)        # push $ra (ret addr, 4 bytes)
    sw      $fp, 4($sp)        # push $fp (4 bytes)
    addu    $fp, $sp, 8        # set $fp to saved $ra
    
    # BODY
    subu    $sp, $sp, 8        # expand stack by 4 bytes
    sw      $s0, 8($sp)
    sw      $a0, 4($sp)        # save arg n

    li      $v0, 0                
    beq     $a0, 0, ret        # return 0 if n = 0
    li      $v0, 1
    beq     $a0, 1, ret        # return 1 if n = 1
 
    sub     $a0, $a0, 1         # $a0 = arg n - 1
    jal     fibonacci           # $v0 = fibonacci(n-1)
    move    $s0, $v0            # save $v0 into $s0
    lw      $a0, 4($sp)
    sub     $a0, $a0, 2         # $a0 = arg n - 2
    jal     fibonacci           # $v0 = fibonacci(n-2)
    add     $v0, $s0, $v0       # $v0 = fibonacci(n-1) + fibonacci(n-2)
 
ret:
    # EPILOGUE
    lw      $s0, 8($sp)
    move    $sp, $fp             # restore $sp
    lw      $ra, ($fp)           # restore saved $ra
    lw      $fp, -4($sp)         # restore saved $fp
    jr      $ra                  # return to kernel 
