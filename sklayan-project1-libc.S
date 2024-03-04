.globl main
.globl getchar
.globl putchar
.globl gets
.globl puts
.globl read_string
.globl write_string

.equ STDOUT, 1
.equ STDIN, 0
.equ __NR_READ, 63
.equ __NR_WRITE, 64
.equ __NR_EXIT, 93

.text

# Function to read from stdin into a buffer
read_string:
    # Function prologue
    addi sp, sp, -16
    sw ra, 0(sp)

    # Read from stdin
    li a7, __NR_READ    # System call number for read
    li a0, STDIN        # File descriptor for stdin
    mv a1, sp           # Move buffer pointer to a1
    mv a2, a1           # Move buffer pointer to a2
    ecall               # Execute read system call

    # Function epilogue
    lw ra, 0(sp)
    addi sp, sp, 16
    ret

# Function to write to stdout from a buffer
write_string:
    # Function prologue
    addi sp, sp, -16
    sw ra, 0(sp)

    # Write to stdout
    li a7, __NR_WRITE   # System call number for write
    li a0, STDOUT       # File descriptor for stdout
    mv a1, sp           # Move buffer pointer to a1
    mv a2, a0           # Move buffer size to a2
    ecall               # Execute write system call

    # Function epilogue
    lw ra, 0(sp)
    addi sp, sp, 16
    ret

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


# Main function
main:
    # Main prologue
    addi sp, sp, -104
    sw ra, 100(sp)

    # Write the prompt to the terminal (stdout)
    la a0, prompt       # Load address of prompt string
    li a1, prompt_end - prompt   # Calculate length of prompt string
    jal write_string    # Call write_string function

    # Read up to 100 characters from the terminal (stdin)
    la a0, buf          # Load address of buffer
    li a1, 100          # Number of bytes to read
    jal read_string     # Call read_string function

    # Write the just read characters to the terminal (stdout)
    la a0, buf          # Load address of buffer
    jal write_string    # Call write_string function

    # Main epilogue
    lw ra, 100(sp)
    addi sp, sp, 104
    ret

.data
prompt:   .ascii  "Enter a message: "
prompt_end:
buf:      .space 100     # Reserve space for buffer
