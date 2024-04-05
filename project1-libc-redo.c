.globl main

.equ STDOUT, 1
.equ STDIN, 0
.equ __NR_READ, 63
.equ __NR_WRITE, 64
.equ __NR_EXIT, 93

.text

# Main function
main:
    # Call puts (* str )
    la a0, prompt            # Put string pointer (prompt) into a0
    jal puts                 # Call puts

    # Call gets (* buf )
    la a0, buf               # Put address pointer (buf) into a0
    jal gets                 # Call gets

    # Call puts (* buf )
    la a0, buf               # Put string pointer (buf) into a0
    jal puts                 # Call puts

    # Halt the program
halt:
    ebreak
    j halt                   # Stop execution

# Function to read a string from stdin
gets:
    # Prolog
    addi sp, sp, -12        # Make room for 3 items on the stack
    sw ra, 0(sp)            # Save return address
    sw a0, 4(sp)            # Save addr pointer

gets_loop:
    jal ra, getchar         # Call getchar
    mv t2, a0               # Move the read character to t2

    li t3, -1               # Represent -1 as 0xffffffff
    beq t2, t3, gets_error  # If getchar returned -1, goto gets_error

    sb t2, 0(t0)            # Store the read-in character at the address pointer
    addi t0, t0, 1          # Increment buffer length counter

    li t4, 10               # ASCII value for newline character
    beq t2, t4, gets_exit   # If read-in char is newline, exit loop

    j gets_loop             # Continue loop

gets_exit:
    sb zero, 0(t0)          # End the input string with a null byte
    addi t0, t0, 1          # Move to the next byte
    mv a0, t0               # Put the address of the null-terminated string into a0

    lw a0, 4(sp)            # Restore addr pointer
    lw ra, 0(sp)            # Restore return address
    addi sp, sp, 12         # Restore stack pointer
    ret                     # Return

gets_error:
    lw ra, 0(sp)            # Restore return address
    addi sp, sp, 12         # Restore stack pointer
    ret                     # Return

# Function to write a string to stdout
puts:
    addi sp, sp, -4          # Adjust stack pointer to make room for one item
    sw ra, 0(sp)             # Save return address

    # Initialize s0 to point to the address pointer
    mv s0, a0                # Move a0 into s0

puts_loop:
    lb a0, 0(s0)             # Load the byte that s0 is currently pointing to into a0
    beqz a0, puts_exit       # If a0 is zero (null character), exit loop

    jal ra, putchar          # Call putchar
    addi s0, s0, 1           # Increment string pointer s0

    bgez a0, puts_loop       # If a0 >= 0, continue loop

    li a0, -1                # Put -1 into a0 (return -1)
    addi sp, sp, 4           # Restore stack pointer
    lw ra, 0(sp)             # Restore return address
    ret                      # Return

puts_exit:
    li a0, 10                # Put newline char into a0 (value 10)
    jal ra, putchar          # Call putchar to print newline
    li a0, 0                 # Put 0 into a0 (return 0)

    lw ra, 0(sp)             # Restore return address
    addi sp, sp, 4           # Restore stack pointer
    ret                      # Return

# Function to read a character from stdin
getchar:
    # Prolog
    addi sp, sp, -12        # Make room for 3 items on the stack
    sw a1, 0(sp)            # Save a1
    sw a2, 4(sp)            # Save a2
    sw a7, 8(sp)            # Save a7

    # Body
    mv a2, a1               # Move a1 into a2
    mv a1, a0               # Move a0 into a1
    li a0, 0                # Put STDIN code into a0
    li a7, 63               # Put NR_READ code into a7
    ecall

    # Epilog
    lw a1, 0(sp)            # Restore a1
    lw a2, 4(sp)            # Restore a2
    lw a7, 8(sp)            # Restore a7
    addi sp, sp, 12         # Restore stack pointer

    ret                     # Return

# Function to write a character to stdout
putchar:
    addi sp, sp, -4          # Adjust stack pointer to make room for a temporary variable
    sw a0, 0(sp)             # Store input char into memory

    li a0, STDOUT            # Put STDOUT code into a0
    mv a1, sp                # Put stack pointer into a1 as a pointer to input char
    li a2, 1                 # Put value 1 into a2 (write 1 byte)
    li a7, __NR_WRITE        # Put NR_WRITE code into a7
    ecall                    # Invoke the write system call

    lw a0, 0(sp)             # Load the input char back into a0
    addi sp, sp, 4           # Restore stack pointer

    ret                       # Return, a0 contains the same value as input char

.data
prompt: .ascii "Enter a message: "
buf: .space 100
buf_end:
