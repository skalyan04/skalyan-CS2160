.globl main 
.equ STDOUT, 1
.equ STDIN, 0
.equ __NR_READ, 63
.equ __NR_WRITE, 64
.equ __NR_EXIT, 93

.text


# NEW: Function to read from stdin into a buffer
read_string:
    li a7, __NR_READ    # System call number for read
    mv a0, STDIN        # File descriptor for stdin
    mv a1, a0           # Move buffer pointer to a1
    add a2, zero, a1    # Move buffer pointer to a2
    ecall               # Execute read system call
    ret
# End of read_string function

# Function to write to stdout from a buffer
write_string:
    li a7, __NR_WRITE   # System call number for write
    mv a0, STDOUT       # File descriptor for stdout
    mv a1, a0           # Move buffer pointer to a1
    ecall               # Execute write system call
    ret
# End of write_string function


main:
	# main() prolog
	addi sp, sp, -104
	sw ra, 100(sp)

	# main() body

	# Write the prompt to the terminal (stdout)
	la a0, prompt # Load address of prompt string
    li a1, prompt_end - prompt   # Calculate length of prompt string
    jal write_string # Call write_string function

	#  Read up to 100 characters from the terminal (stdin)
	mv a1, sp    # Move buffer pointer to a1
    li a2, 100  # Number of bytes to read
    jal read_string # NEW: Call read_string function

	# Write the just read characters to the terminal (stdout)
	mv a1, a0               # Move buffer pointer to a1
    jal write_string        # Call write_string function

	# main() epilog
	lw ra, 100(sp)
	addi sp, sp, 104
	ret

.data
prompt:   .ascii  "Enter a message: "
prompt_end:
