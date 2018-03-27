.wrt_INT:                     # Integer write label
    .string "%d\n"            # String used for printing integers
.globl main
factorial:                    # Start of body
    movq 8(%rbp), t0          # Move val from stack to temp
    movq t0, t1               # Copy val to new temp, to not harm it
    movq $0, t2               # Moving constant to register
    cmp t1, t2                # Compare, EQ
    je cmpTrue_0              # If true, jump to label
    movq $0, t3               # Setting result to 0 (false)
    jmp endCMP_0              # Jump to after compare label
cmpTrue_0:                    # Compare true label
    movq $1, t3               # Setting result to 1 (true)
endCMP_0:                     # After compare label
    movq t3, t3               # Used to get "target" when creating next instruction
    cmp $1, t3                # Compare left side of OR with true
    je endBoolCMP_1           # If true, skip right expression
endBoolCMP_1:                 # OR expression label
    movq 8(%rbp), t4          # Move val from stack to temp
    movq t4, t5               # Copy val to new temp, to not harm it
    movq $1, t6               # Moving constant to register
    cmp t5, t6                # Compare, EQ
    je cmpTrue_2              # If true, jump to label
    movq $0, t7               # Setting result to 0 (false)
    jmp endCMP_2              # Jump to after compare label
cmpTrue_2:                    # Compare true label
    movq $1, t7               # Setting result to 1 (true)
endCMP_2:                     # After compare label
    movq t7, t7               # Used to get "target" when creating next instruction
    cmp $1, t7                # Check if IF expression is true
    jne else_0                # Expression is false, jump to ELSE part
    movq $1, t8               # Moving constant to register
    movq t8, %rax             # Return value placed in RAX
    jmp end_factorial         # Jump to functions end label
    jmp if_end_0              # Skip ELSE part
else_0:                       # Start of ELSE
    movq 8(%rbp), t9          # Move val from stack to temp
    movq t9, t10              # Copy val to new temp, to not harm it
    movq 8(%rbp), t11         # Move val from stack to temp
    movq t11, t12             # Copy val to new temp, to not harm it
    movq $1, t13              # Moving constant to register
    push t13                  # Push argument for function
    call factorial            # Calling function
    movq %rax, t14            # Saving return value from function in temp
    movq t10, %rax            # Using RAX for multiplication
    movq t14, %rbx            # Using RBX for multiplication
    movq %rax, t15            # Storing result here (temp)
    movq t15, %rax            # Return value placed in RAX
    jmp end_factorial         # Jump to functions end label
if_end_0:                     # End of IF
end_factorial:                # End of body
    ret                       # Return from function
main:                         # Start of body
    movq $5, t16              # Moving constant to register
    push t16                  # Push argument for function
    call factorial            # Calling function
    movq %rax, t17            # Saving return value from function in temp
    push %rax                 # Saving value of RAX before printf call
    movq $.wrt_INT, %rdi      # First argument for printf
    movq t17, %rsi            # Second argument for printf
    movq $0, %rax             # No vector arguments
    call printf               # Calling printf
    pop %rax                  # Restoring RAX
main_end:                     # End of body
    movq $0, %rax             # Return "no error" exit code
    ret                       # Program return
