.globl main 
.equ STDOUT, 1
.equ STDIN, 0
.equ __NR_READ, 63
.equ __NR_WRITE, 64
.equ __NR_EXIT, 93

.text
main:
    # Write the prompt to terminal
    la a0, prompt           # Put address of prompt into a0
    li a1, 16               # Put the length of the string (in bytes) into a1
    jal ra, write_string    # Call write_string

    # Read up to (buf_end - buf) bytes from terminal input
    la a0, buf              # Put address of buf into a0
    li a1, 100              # Put (buf_end - buffer) into a1
    jal ra, read_string     # Call read_string

    # At this point, a0 holds the number of bytes of the input
    # Write the just read chars to the terminal
    mv a1, a0               # Move a0 to a1
    la a0, buf              # Put address of buf into a0
    jal ra, write_string    # Call write_string

write_string:
    # Prolog
    addi sp, sp, -16        # Make room for 4 items on the stack
    sw a0, 0(sp)            # Save a0
    sw a1, 4(sp)            # Save a1
    sw a2, 8(sp)            # Save a2
    sw a7, 12(sp)           # Save a7
    # Body
    mv a2, a1               # Move a1 into a2
    mv a1, a0               # Move a0 into a1
    li a0, 1                # Put STDOUT code into a0
    li a7, 64               # Put NR_WRITE code into a7
    ecall
    # Epilog
    lw a0, 0(sp)            # Restore a0
    lw a1, 4(sp)            # Restore a1
    lw a2, 8(sp)            # Restore a2
    lw a7, 12(sp)           # Restore a7
    addi sp, sp, 16         # Restore stack pointer
    ret

read_string:
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
    ret

.data
prompt:   .ascii "Enter a message: "
buf:      .space 100
buf_end:
