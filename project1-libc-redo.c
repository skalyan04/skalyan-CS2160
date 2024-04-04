.globl main

.equ STDOUT, 1
.equ STDIN, 0
.equ __NR_READ, 63
.equ __NR_WRITE, 64
.equ __NR_EXIT, 93

.data
prompt:   .ascii "Enter a message: "
buf:      .space 100
buf_end:

.text
main:
    # Call puts(str)
    la a0, prompt
    jal ra, puts

    # Call gets(buf)
    la a0, buf
    jal ra, gets

    # Call puts(buf)
    la a0, buf
    jal ra, puts

    # Infinite loop
loop:
    j loop

# Implementation of getchar function
getchar:
    # Prolog
    addi sp, sp, -4     # Reserve space on the stack
    sw ra, 0(sp)         # Save return address

    # Body
    li a0, STDIN         # File descriptor for stdin
    li a2, 1             # Read one byte
    li a7, __NR_READ     # System call number for read
    ecall                # Invoke system call

    # Epilog
    lw ra, 0(sp)         # Restore return address
    mv a0, a0            # Move the read-in character to a0
    addi sp, sp, 4       # Restore stack pointer
    jr ra                # Return from subroutine

# Implementation of putchar function
putchar:
    # Prolog
    addi sp, sp, -4     # Reserve space on the stack
    sw ra, 0(sp)        # Save return address

    # Body
    li a0, STDOUT       # File descriptor for stdout
    mv a1, a0           # Move the character to be written to a1
    li a2, 1            # Write one byte
    li a7, __NR_WRITE   # System call number for write
    ecall               # Invoke system call

    # Epilog
    lw ra, 0(sp)        # Restore return address
    mv a0, a1           # Move the written character to a0
    addi sp, sp, 4      # Restore stack pointer
    jr ra               # Return from subroutine

# Implementation of gets function
gets:
    # Prolog
    addi sp, sp, -12    # Reserve space on the stack
    sw ra, 0(sp)        # Save return address
    sw a1, 4(sp)        # Save a1
    sw a2, 8(sp)        # Save a2

    # Body
    mv a2, a1           # Move a1 into a2
    mv a1, a0           # Move a0 into a1
    li a0, STDIN        # Put STDIN code into a0
    li a7, __NR_READ    # Put NR_READ code into a7
    ecall               # Invoke system call

    # Epilog
    lw ra, 0(sp)        # Restore return address
    lw a1, 4(sp)        # Restore a1
    lw a2, 8(sp)        # Restore a2
    addi sp, sp, 12     # Restore stack pointer
    jr ra               # Return from subroutine

# Implementation of puts function
puts:
    # Prolog
    addi sp, sp, -4     # Reserve space on the stack
    sw ra, 0(sp)        # Save return address

    # Body
    # Load string address
    la a1, buf
putchar_loop:
    lbu a0, 0(a1)       # Load byte from buffer
    beqz a0, putchar_exit  # If byte is null, exit loop
    jal ra, putchar     # Call putchar
    addi a1, a1, 1      # Move to next byte in buffer
    j putchar_loop

putchar_exit:
    lw ra, 0(sp)        # Restore return address
    addi sp, sp, 4      # Restore stack pointer
    ret
