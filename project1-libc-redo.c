.globl main
.equ STDOUT, 1
.equ STDIN, 0
.equ __NR_READ, 63
.equ __NR_WRITE, 64
.equ __NR_EXIT, 93

.text

main:
    # Print the prompt message
    la a0, prompt         # Put string pointer (prompt) into a0
    jal ra, puts          # Call puts

    # Get input from the user
    la a0, buf            # Put address pointer (buf) into a0
    jal ra, gets          # Call gets and read user input

    # Print the input message
    la a0, buf            # Put string pointer (buf) into a0
    jal ra, puts          # Call puts to print the input message

    # Exit the program
    li a0, 0              # Put 0 into a0 (return 0)
    li a7, __NR_EXIT      # Put NR_EXIT code into a7
    ecall

halt:
    ebreak
    j halt                 # stop execution

getchar:
    li a0, STDIN           # Put STDIN code into a0
    li a2, 1               # Put value 1 into a2 (read in one byte)
    li a7, __NR_READ       # Put NR_READ code into a7
    ecall
    ret

putchar:
    li a0, STDOUT          # Put STDOUT code into a0
    li a2, 1               # Put value 1 into a2 (write 1 byte)
    li a7, __NR_WRITE      # Put NR_WRITE code into a7
    ecall
    ret

gets:
    mv t0, a0              # Move a0 into t0 (use t0 to increment the address pointer)
    li t1, 100             # Maximum buffer size
gets_loop:
    jal ra, getchar        # Call getchar
    sb a0, 0(t0)           # Store the read-in character at the address pointer
    addi t0, t0, 1         # Increment t0 by 1
    addi t1, t1, -1        # Decrement buffer size counter
    beqz a0, gets_exit     # If read-in char is newline, exit loop
    j gets_loop            # Continue loop

gets_exit:
    sb zero, 0(t0)         # End the input string with a terminating 0 byte
    mv a0, t0              # Put the address of the null-terminated string into a0
    ret

puts:
    mv t0, a0              # Move a0 into t0 (use t0 to increment the address pointer)
puts_loop:
    lb a0, 0(t0)           # Load the byte that t0 is currently pointing to into a0
    beqz a0, puts_exit     # If a0 is zero (null character), exit loop
    jal ra, putchar        # Call putchar
    addi t0, t0, 1         # Increment string pointer t0
    j puts_loop            # Continue loop

puts_exit:
    li a0, 10              # Put newline char into a0 (value 10)
    jal ra, putchar        # Call putchar to print newline
    li a0, 0               # Put 0 into a0 (return 0)
    ret

.data
prompt: .ascii "Enter a message: "
newline: .byte 10
buf: .space 100
