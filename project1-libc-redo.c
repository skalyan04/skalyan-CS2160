.globl main

.equ STDOUT, 1
.equ STDIN, 0
.equ __NR_READ, 63
.equ __NR_WRITE, 64
.equ __NR_EXIT, 93

.text

main:
    # Call puts to display the prompt message
    la a0, prompt         # Load string pointer (prompt) into a0
    jal ra, puts          # Call puts

    # Call gets to read input from the user
    la a0, buf            # Load address pointer (buf) into a0
    jal ra, gets          # Call gets

    # Call puts to display the input message
    la a0, buf            # Load string pointer (buf) into a0
    jal ra, puts          # Call puts

    # Exit program
    li a0, 0              # Put 0 into a0 (return 0)
    li a7, __NR_EXIT      # Put NR_EXIT code into a7
    ecall

getchar:
    addi sp, sp, -4         # Make room for 1 item on the stack
    sw zero, 0(sp)          # Zero-out the memory address that sp is pointing to

    li a0, STDIN            # Put STDIN code into a0
    mv a1, sp               # Put sp into a1 (a place to store the read-in char)
    li a2, 1                # Put value 1 into a2 (read in one byte)
    li a7, __NR_READ        # Put NR_READ code into a7
    ecall

    lw a0, 0(sp)            # Load *sp into a0 (return ASCII value)
    addi sp, sp, 4          # Restore sp
    ret

putchar:
    # Prolog
    addi sp, sp, -4         # Make room for 1 item on the stack
    sw a0, 0(sp)            # Store a0 into stack (save input char into memory)

    # Body
    li a0, STDOUT           # Put STDOUT code into a0
    mv a1, sp               # Put sp into a1 (pointer to input char)
    li a2, 1                # Put value 1 into a2 (write 1 byte)
    li a7, __NR_WRITE       # Put NR_WRITE code into a7
    ecall

    # Epilog
    lw a0, 0(sp)            # Load (byte value) of *sp back into a0
    addi sp, sp, 4          # Restore sp
    ret

gets:
    # Prolog
    addi sp, sp, -12        # Make room for 3 items on the stack
    sw ra, 0(sp)            # Save ra
    sw a0, 4(sp)            # Save a0
    sw s0, 8(sp)            # Save s0

    # Body
    mv s0, a0               # Move address pointer (a0) into s0

gets_loop:
    jal ra, getchar         # Call getchar
    mv t0, a0               # Move the read character to t0

    li t1, -1               # Check for negative read-in character (EOF)
    beq t0, t1, gets_error  # If getchar returned -1, goto gets_error

    sb t0, 0(s0)            # Store the read-in character at the address pointer
    addi s0, s0, 1          # Increment s0 by 1

    li t1, 10               # Load the ASCII value of newline

    beq t0, t1, epilog_gets # If read-in char is newline, go to epilog_gets

    j gets_loop             # Continue loop

# Epilog
epilog_gets:
    lw ra, 0(sp)            # Restore ra
    lw a0, 4(sp)            # Restore a0
    lw s0, 8(sp)            # Restore s0
    addi sp, sp, 12         # Restore stack pointer

    # Calculate and store the length of the read-in string
    sub a0, s0, a0

    ret

gets_error:
    lw ra, 0(sp)            # Restore ra
    addi sp, sp, 12         # Restore stack pointer
    ret

puts:
    # Prolog
    addi sp, sp, -12        # Make room for 3 items on the stack
    sw ra, 0(sp)            # Save ra
    sw a0, 4(sp)            # Save a0
    sw s0, 8(sp)            # Save s0

    # Body
    mv s0, a0               # Move address pointer (a0) into s0

puts_loop:
    lb a1, 0(s0)            # Load the ASCII value from memory into a1
    beqz a1, puts_exit      # If ASCII value is zero (null char), exit loop

    jal ra, putchar         # Call putchar
    addi s0, s0, 1          # Increment string pointer s0

    bgez a1, puts_loop      # If a1 >= 0, continue loop

    # Error branch (a1 < 0)
    li a0, -1               # Put -1 into a0 (return -1)
    j epilog_puts

puts_exit:
    li a0, newline          # Put newline char into a0 (value 10)
    jal ra, putchar         # Call putchar
    li a0, 0                # Put 0 into a0 (return 0)

# Epilog
epilog_puts:
    lw ra, 0(sp)            # Restore ra
    lw a0, 4(sp)            # Restore a0
    lw s0, 8(sp)            # Restore s0
    addi sp, sp, 12         # Restore stack pointer
    ret

.data
prompt: .ascii "Enter a message: "
newline: .byte 10
buf: .space 100
