.globl main

.equ STDOUT, 1
.equ STDIN, 0
.equ __NR_READ, 63
.equ __NR_WRITE, 64
.equ __NR_EXIT, 93

.text

# Main function
main:
    # Write the prompt to terminal
    la a0, str           # Load prompt message address into a0
    # Call puts function to print prompt
    jal puts             

    # Get input from the user
    la a0, buf           # Load the address of the buffer into a0
    # Call the gets function to read user input
    jal gets             

    # Print the input message
    la a0, buf           # Load input buffer address into a0
    # Call the puts function to print the input message
    jal puts             
    
# Function to read a character from stdin
getchar:
  # Put STDIN code into a0
    li a0, STDIN        
    # Put value 1 into a2 (read 1 byte) 
    li a2, 1               
    # Put NR_READ code into a7
    li a7, __NR_READ       
    # Invoke the read system call
    ecall                
    # Return the read character (already in a0)  
    ret                     

# Function to write a character to stdout
putchar:
    # Put STDOUT code into a0
    li a0, STDOUT    
    # Put value 1 into a2 (write 1 byte)      
    li a2, 1               
     # Put NR_WRITE code into a7
    li a7, __NR_WRITE     
    # Invoke the write system call
    ecall            
    # Return the written character (same as input)       
    ret                     

# Function to read a string from stdin
gets:
    # Move the address of the buffer to t0
    mv t0, a0               
    li t1, 100              # Maximum buffer size
gets_loop:
    jal ra, getchar         # Call getchar to read a character
    # Move the read character to t2
    mv t2, a0               
    li t3, -1               # Represent -1 as 0xffffffff
     # If getchar returned -1, exit loop
    beq t2, t3, gets_exit 
    # Store the read-in character at the address pointer
    sb t2, 0(t0)            
    addi t0, t0, 1          # Increment t0 by 1
    addi t1, t1, -1         # Decrement buffer size counter
    li t4, 10               # ASCII value for newline character
    # If read-in char is newline, exit loop
    beq t2, t4, gets_exit  
    # If buffer size reached, exit loop
    beqz t1, gets_exit      
    j gets_loop             # Continue loop
gets_exit:
    # End the input string with a null byte
    sb zero, 0(t0)          
    addi t0, t0, 1          # Move to the next byte
     # Put the address of the null-terminated string into a0
    mv a0, t0             
    ret

# Function to write a string to stdout
puts:
    mv t0, a0               # Move string address to t0
puts_loop:
    # Load byte that t0 is currently pointing to into a0
    lb a0, 0(t0)   
    # If a0 is zero (null character), exit loop       
    beqz a0, puts_exit 
    # Call putchar to print the character     
    jal ra, putchar    
    # Increment string pointer t0     
    addi t0, t0, 1          
    j puts_loop             # Continue loop
puts_exit:
    # Put newline char into a0 (value 10)
    li a0, 10     
    # Call putchar to print newline         
    jal ra, putchar   
    # Put 0 into a0 (return 0)      
    li a0, 0                
    ret
    
.data
str: .ascii "Enter a message: "
buf: .space 100
