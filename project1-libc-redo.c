.globl main

.equ STDOUT, 1
.equ STDIN, 0
.equ __NR_READ, 63
.equ __NR_WRITE, 64
.equ __NR_EXIT, 93

.text

main:
    # Call puts(*str)
    la a0, prompt         # Put string pointer (prompt) into a0
    jal ra, puts           # Call puts

    # Call gets(*buf)
    la a0, buf            # Put address pointer (buf) into a0
    jal ra, gets           # Call gets

    # Call puts(*buf)
    la a0, buf            # Put string pointer (buf) into a0
    jal ra, puts           # Call puts

    # Exit program
    li a0, 0               # Put 0 into a0 (return 0)
    li a7, __NR_EXIT       # Put NR_EXIT code into a7
    ecall
     
halt:
    ebreak
    j halt                 # Stop execution


gets:
    # Prolog
    addi sp, sp, -12        # Make room for 3 items on the stack
    sw a1, 0(sp)            # Save a1
    sw a2, 4(sp)            # Save a2
    sw a7, 8(sp)            # Save a7

    # Body
    mv a2, a1               # Move a1 into a2
    mv a1, a0               # Move a0 into a1
    li a0, STDIN            # Put STDIN code into a0
    li a7, __NR_READ        # Put NR_READ code into a7
    ecall

    # Epilog
    lw a1, 0(sp)            # Restore a1
    lw a2, 4(sp)            # Restore a2
    lw a7, 8(sp)            # Restore a7
    addi sp, sp, 12         # Restore stack pointer
    ret


puts:
    # Prolog
    addi sp, sp, -16        # Make room for 4 items on the stack
    sw a0, 0(sp)            # Save a0
    sw a1, 4(sp)            # Save a1
    sw a2, 8(sp)            # Save a2
    sw a7, 12(sp)           # Save a7

puts_loop:
    lb a1, 0(a0)            # Load the ASCII value from memory into a1
    beqz a1, puts_exit      # If ASCII value is zero (null char), exit loop
    jal ra, putchar         # Call putchar
    addi a0, a0, 1          # Increment string pointer a0
    j puts_loop             # Continue loop

puts_exit:
    # Epilog
    lw a0, 0(sp)            # Restore a0
    lw a1, 4(sp)            # Restore a1
    lw a2, 8(sp)            # Restore a2
    lw a7, 12(sp)           # Restore a7
    addi sp, sp, 16         # Restore stack pointer
    ret


getchar:
    # Prolog
    addi sp, sp, -12        # Make room for 3 items on the stack
    sw a1, 0(sp)            # Save a1
    sw a2, 4(sp)            # Save a2
    sw a7, 8(sp)            # Save a7

    # Body
    li a0, STDIN            # Put STDIN code into a0
    mv a2, a0               # Move a0 into a2
    li a0, 0                # Put a0 into 0
    li a7, __NR_READ        # Put NR_READ code into a7
    ecall

    # Epilog
    lw a1, 0(sp)            # Restore a1
    lw a2, 4(sp)            # Restore a2
    lw a7, 8(sp)            # Restore a7
    addi sp, sp, 12         # Restore stack pointer
    ret


putchar:
    # Prolog
    addi sp, sp, -16        # Make room for 4 items on the stack
    sw a0, 0(sp)            # Save a0
    sw a1, 4(sp)            # Save a1
    sw a2, 8(sp)            # Save a2
    sw a7, 12(sp)           # Save a7

    # Body
    li a0, STDOUT           # Put STDOUT code into a0
    mv a1, a0               # Move the ASCII value into a1
    li a2, 1                # Put value 1 into a2 (write 1 byte)
    li a7, __NR_WRITE       # Put NR_WRITE code into a7
    ecall

    # Epilog
    lw a0, 0(sp)            # Restore a0
    lw a1, 4(sp)            # Restore a1
    lw a2, 8(sp)            # Restore a2
    lw a7, 12(sp)           # Restore a7
    addi sp, sp, 16         # Restore stack pointer
    ret


.data
prompt: .ascii "Enter a message: "
newline: .byte 10
buf: .space 100
