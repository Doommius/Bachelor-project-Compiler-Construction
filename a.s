.wrt_INT:                     # Integer write label
    .string "%d\n"            # String used for printing integers
.globl main
factorial:                    # Start of body
    movq 8(%rbp), t2          # Move val from stack to temp
    movq t2, t3               # Copy val to new temp, to not harm it
    movq $0, t4               # Moving constant to register
    cmp t3, t4                # Compare, EQ
    je cmpTrue_1              # If true, jump to label
    movq $0, t5               # Setting result to 0 (false)
    jmp endCMP_1              # Jump to after compare label
cmpTrue_1:                    # Compare true label
    movq $1, t5               # Setting result to 1 (true)
endCMP_1:                     # After compare label
    movq t5, t5               # Used to get "target" when creating next instruction
    cmp $1, t5                # Compare left side of OR with true
    je endBoolCMP_2           # If true, skip right expression
endBoolCMP_2:                 # OR expression label
    movq 8(%rbp), t6          # Move val from stack to temp
    movq t6, t7               # Copy val to new temp, to not harm it
    movq $1, t8               # Moving constant to register
    cmp t7, t8                # Compare, EQ
    je cmpTrue_3              # If true, jump to label
    movq $0, t9               # Setting result to 0 (false)
    jmp endCMP_3              # Jump to after compare label
cmpTrue_3:                    # Compare true label
    movq $1, t9               # Setting result to 1 (true)
endCMP_3:                     # After compare label
    movq t9, t9               # Used to get "target" when creating next instruction
    cmp $1, t9                # Check if IF expression is true
    jne else_1                # Expression is false, jump to ELSE part
    movq $1, t10              # Moving constant to register
    movq t10, %rax            # Return value placed in RAX
    jmp end_factorial         # Jump to functions end label
    jmp if_end_1              # Skip ELSE part
else_1:                       # Start of ELSE
    movq 8(%rbp), t11         # Move val from stack to temp
    movq t11, t12             # Copy val to new temp, to not harm it
    movq 8(%rbp), t13         # Move val from stack to temp
    movq t13, t14             # Copy val to new temp, to not harm it
    movq $1, t15              # Moving constant to register
    subq t14, t15             # Subtraction
    push t15                  # Push argument for function
    call factorial            # Calling function
    movq %rax, t16            # Saving return value from function in temp
    movq t12, %rax            # Using RAX for multiplication
    movq t16, %rbx            # Using RBX for multiplication
    imul %rbx                 # Multiplication using RAX and RBX
    movq %rax, t17            # Storing result here (temp)
    movq t17, %rax            # Return value placed in RAX
    jmp end_factorial         # Jump to functions end label
if_end_1:                     # End of IF
end_factorial:                # End of body
    ret                       # Return from function
main:                         # Start of body
    movq $5, t18              # Moving constant to register
    push t18                  # Push argument for function
    call factorial            # Calling function
    movq %rax, t19            # Saving return value from function in temp
    push %rax                 # Saving value of RAX before printf call
    movq $.wrt_INT, %rdi      # First argument for printf
    movq t19, %rsi            # Second argument for printf
    movq $0, %rax             # No vector arguments
    call printf               # Calling printf
    pop %rax                  # Restoring RAX
main_end:                     # End of body
    movq $0, %rax             # Return "no error" exit code
    ret                       # Program return
