.globl main 
.equ STDOUT, 1
.equ STDIN, 0
.equ __NR_READ, 63
.equ __NR_WRITE, 64
.equ __NR_EXIT, 93

.text
main:
	# main() prolog
	addi sp, sp, -104
	sw ra, 100(sp)

	# main() body

	# Write the prompt to the terminal (stdout)
	li a7, __NR_WRITE
	li a0, STDOUT
	la a1, prompt
	addi  a2, zero, prompt_end - prompt        
	ecall

	#  Read up to 100 characters from the terminal (stdin)
	li a7, __NR_READ
	li a0, STDIN
	mv a1, sp
	addi a2, zero, 100
	ecall

	# Write the just read characters to the terminal (stdout)
	addi a2, a0, 0
	li a7, __NR_WRITE
	li a0, STDOUT
	mv a1, sp
	ecall

	# main() epilog
	lw ra, 100(sp)
	addi sp, sp, 104
	ret

read_string:
    # Procedure - see below
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

.data
prompt:   .ascii  "Enter a message: "
prompt_end:
