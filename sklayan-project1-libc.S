.globl getchar
.globl putchar
.globl gets
.globl puts

.equ STDOUT, 1
.equ STDIN, 0
.equ __NR_READ, 63
.equ __NR_WRITE, 64

.text

# Function to read a single character from stdin
getchar:
    li a7, __NR_READ    # System call number for read
    li a0, STDIN        # File descriptor for stdin
    mv a1, sp           # Move buffer pointer to a1
    li a2, 1            # Number of bytes to read (1 character)
    ecall               # Execute read system call
    mv t0, a0           # Move result to temporary register t0
    beqz t0, getchar_exit # If end of file or error, exit with -1
    ret

getchar_exit:
    li t0, -1           # Return -1 on end of file or error
    ret

# Function to write a single character to stdout
putchar:
    li a7, __NR_WRITE   # System call number for write
    li a0, STDOUT       # File descriptor for stdout
    mv a1, sp           # Move buffer pointer to a1 (character to write)
    li a2, 1            # Number of bytes to write (1 character)
    ecall               # Execute write system call
    mv a0, a1           # Return the input unsigned char
    ret

# Function to read a line from stdin into a buffer
gets:
    mv a1, sp           # Save buffer address
    mv a2, sp           # Save current character address
gets_loop:
    jal getchar         # Call getchar to read a character
    mv t0, a0           # Move result to temporary register t0
    beqz t0, gets_exit  # If end of file or error, exit
    sb t0, 0(a2)        # Store the character in the buffer
    addi a2, a2, 1      # Move buffer pointer to the next character
   bne t0, x10, gets_loop # Continue reading until newline character
gets_exit:
    sb x0, 0(a2)        # Null-terminate the string
    sub a0, a2, a1      # Calculate the number of bytes read
    ret

# Function to write a string to stdout
puts:
    mv a1, sp           # Move buffer pointer to a1 (string to write)
puts_loop:
    lb t0, 0(a1)        # Load a character from the string
    beqz t0, puts_exit  # If null terminator is reached, exit
    jal putchar         # Call putchar to write the character
    addi a1, a1, 1      # Move to the next character in the string
    j puts_loop         # Continue writing characters
puts_exit:
    li a0, 0            # Return 0 on success
    ret

# Main function
.globl main
main:
    # Allocate space for the buffer
    addi sp, sp, -100

    # Load the string pointer
    la a0, str

    # Call puts to print the string
    jal puts

    # Load the buffer address
    la a0, buffer

    # Call gets to read input into the buffer
    jal gets

    # Call puts to print the buffer
    la a0, buffer
    jal puts

    # Exit the program
    li a7, 93           # System call number for exit
    li a0, 0            # Exit code 0
    ecall               # Execute exit system call

.data
prompt: .ascii "Enter a message: "
buffer: .space 100      # Buffer for input
