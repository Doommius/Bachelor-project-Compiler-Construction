.wrt_INT:                     # Integer write label
    .string "%d\n"            # String used for printing integers
.wrt_TRUE:                    # TRUE write label
    .string "TRUE\n"          # String used to print TRUE
.wrt_FALSE:                   # FALSE write label
    .string "FALSE\n"         # String used for printing FALSE
.globl main
factorial:                    # Start of body
    push %rbp                 # Push old base pointer to stack
    movq %rsp, %rbp           # Move stack pointer to base pointer
    movq $0, %rax             # Moving constant to register
    cmp %r15, %rax            # Compare, EQ
    je cmpTrue_1              # If true, jump to label
    movq $0, %rbx             # Setting result to 0 (false)
    jmp endCMP_1              # Jump to after compare label
cmpTrue_1:                    # Compare true label
    movq $1, %rbx             # Setting result to 1 (true)
endCMP_1:                     # After compare label
    movq $1, %rax             # Setting default value of result to true
    cmp $1, %rbx              # Compare left side of OR with true
    je endBoolCMP_2           # If true, skip right expression
    movq $1, %rax             # Moving constant to register
    cmp %r15, %rax            # Compare, EQ
    je cmpTrue_3              # If true, jump to label
    movq $0, %rax             # Setting result to 0 (false)
    jmp endCMP_3              # Jump to after compare label
cmpTrue_3:                    # Compare true label
    movq $1, %rax             # Setting result to 1 (true)
endCMP_3:                     # After compare label
endBoolCMP_2:                 # OR expression label
    cmp $1, %rax              # Check if IF expression is true
    jne else_1                # Expression is false, jump to ELSE part
    movq $1, %rax             # Moving constant to register
    jmp end_factorial         # Jump to functions end label
    jmp if_end_1              # Skip ELSE part
else_1:                       # Start of ELSE
    movq %r15, %rbx           # Copy val to new temp, to not harm it
    push %rbx                 # Register is live in function, so saving it before CALL
    movq $1, %rax             # Moving constant to register
    subq %rax, %r15           # Subtraction
    movq 16(%rbp), %rax       # Retrieving static link
    push %rax                 # Storing static link for function
    call factorial            # Calling function
    addq $8, %rsp             # Remove static link
    movq %rax, %rdx           # Saving return value from function in temp
    pop %rbx                  # Register was live in function, so restoring it after CALL
    movq %rbx, %rax           # Using RAX for multiplication
    imul %rdx                 # Multiplication using RAX and RDX
    jmp end_factorial         # Jump to functions end label
if_end_1:                     # End of IF
end_factorial:                # End of body
    movq %rbp, %rsp           # Retore old stack pointer
    pop %rbp                  # Restore old base pointer
    ret                       # Return from function

main:                         # Start of body
    push %rbp                 # Push old base pointer to stack
    movq %rsp, %rbp           # Move stack pointer to base pointer
    subq $8, %rsp             # Make space for variables and spills
    movq $5, %r15             # Moving constant to register
    movq %rbp, %rax           # Setting address wanted for static link
    addq $8, %rax             # Adding offset of 8 to static link
    push %rax                 # Storing static link for function
    call factorial            # Calling function
    addq $8, %rsp             # Remove static link
    push %rax                 # Register is live in function, so saving it before CALL
    movq $.wrt_INT, %rdi      # First argument for printf
    movq %rax, %rsi           # Second argument for printf
    movq $0, %rax             # No vector arguments
    call printf               # Calling printf
    pop %rax                  # Register was live in function, so restoring it after CALL
main_end:                     # End of body
    addq $8, %rsp             # Remove space for variables and spills
    movq %rbp, %rsp           # Retore old stack pointer
    pop %rbp                  # Restore old base pointer
    movq $0, %rax             # Return "no error" exit code
    ret                       # Program return

