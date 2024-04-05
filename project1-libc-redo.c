.globl main

.equ STDOUT, 1
.equ STDIN, 0
.equ __NR_READ, 63
.equ __NR_WRITE, 64
.equ __NR_EXIT, 93

.text

main:
    # call puts(*str)
    la a0, prompt         # Put string pointer (prompt) into a0
    jal ra, puts           # Call puts
    # call gets(*buf)
    la a0, buf            # Put address pointer (buf) into a0
    jal ra, gets           # Call gets
    # call puts(*buf)
     la a0, buf            # Put string pointer (buf) into a0
     jal ra, puts           # Call puts

     # exit program
     li a0, 0               # Put 0 into a0 (return 0)
     li a7, __NR_EXIT       # Put NR_EXIT code into a7
     ecall
     
     halt:
         ebreak
         j halt                 # stop execution


gets:
    # prolog
    addi sp, sp, -12        # Make room for 3 items on the stack
    sw ra, 0(sp)            # Save return address
    sw a0, 4(sp)            # Save addr pointer
    sw s0, 8(sp)            # Save s0

    # body
    mv s0, a0               # Move a0 into s0

    gets_loop:
        jal ra, getchar     # Call getchar
        mv t0, a0           # Move the read character to t0

        li t1, -1           # Represent -1 as 0xffffffff
        beq t0, t1, gets_error  # If getchar returned -1, goto gets_error
        sb t0, 0(s0)           # Store the read-in character at the address pointer
        addi s0, s0, 1         # Increment s0 by 1
        li t1, 10              # Load the ASCII value of newline into a temporary register (t1)
        beq t0, t1, gets_exit # Branch to gets_exit if read-in char is newline
        j gets_loop            # Continue loop if not newline

    gets_error:
        li a0, -1              # Put -1 into a0 (return -1)
        j epilog_gets

    gets_exit:
        sb zero, 0(s0)         # End the input string with a terminating 0 byte
        j epilog_gets

    epilog_gets:
        lw ra, 0(sp)           # restore ra
        lw a0, 4(sp)           # restore a0
        lw s0, 8(sp)           # restore s0
        addi sp, sp, 12        # restore stack pointer
        ret
    
    
puts:
    # prolog
    addi sp, sp, -4        # make room for 1 item on the stack
    sw ra, 0(sp)           # save ra
    # body
    mv s0, a0              # move a0 into s0 (use s0 to increment the addr pointer)
puts_loop:
    lb t0, 0(s0)           # Put the ascii value that s0 is currently pointing to into t0
    beqz t0, puts_exit     # if ascii value is zero (null char), goto to puts_exit
    jal ra, putchar        # Call putchar
    addi s0, s0, 1         # Increment string pointer s0
    bgez t0, puts_loop     # If t0 >= 0, goto puts_loop
puts_error:
    li a0, -1              # Put -1 into a0 (return -1)
    j epilog_puts
puts_exit:
    li a0, newline         # Put newline char into a0 (value 10)
    jal ra, putchar        # Call putchar
    li a0, 0               # Put 0 into a0 (return 0)
epilog_puts:
    lw ra, 0(sp)           # restore ra
    addi sp, sp, 4         # restore stack pointer
    ret


getchar:
    # prolog
    addi sp, sp, -4        # make room for 1 item on the stack

    # body
    li a0, STDIN           # Put STDIN code into a0
    mv a1, sp              # Put sp into a1 (a place to store the read-in char)
    li a2, 1               # Put value 1 into a2 (read in one byte)
    li a7, __NR_READ       # Put NR_READ code into a7
    ecall

    # epilog
    lw a0, 0(sp)           # load *sp into a0 (return ascii value)
    addi sp, sp, 4         # restore sp
    ret


putchar:
    # prolog
    addi sp, sp, -4        # make room for 1 item on the stack
    sw a0, 0(sp)           # store a0 into stack (save input char into memory)

    # body
    li a0, STDOUT          # Put STDOUT code into a0
    mv a1, sp              # Put sp into a1 (pointer to input char)
    li a2, 1               # Put value 1 into a2 (write 1 byte)
    li a7, __NR_WRITE      # Put NR_WRITE code into a7
    ecall

    # epilog
    lw a0, 0(sp)           # load (byte value) of *sp back into a0
    addi sp, sp, 4         # restore sp
    ret
    

.data
prompt: .ascii "Enter a message: "
newline: .byte 10
buf: .space 100
