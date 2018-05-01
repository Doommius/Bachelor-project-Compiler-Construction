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
    movq -16(%rbp), %rsi        # Copy static link to new reg
    movq $.wrt_INT, %rdi      # First argument for printf
    movq $0, %rax             # No vector arguments
    call printf               # Calling printf
    movq 8(%rbp), %rdx        # Copy static link to new reg
    movq %r15, %rax           # Using RAX for multiplication
    imul %rdx                 # Multiplication using RAX and RDX
    jmp end_factorial         # Jump to functions end label
end_factorial:                # End of body
    movq %rbp, %rsp           # Retore old stack pointer
    pop %rbp                  # Restore old base pointer
    ret                       # Return from function
main:                         # Start of body
    push %rbp                 # Push old base pointer to stack
    movq %rsp, %rbp           # Move stack pointer to base pointer
    subq $16, %rsp            # Make space for variables and spills
    movq $2, %rax             # Moving constant to register
    movq %rax, -8(%rbp)       # Assigning value to var
    push %r15                 # Register is live in function, so saving it before CALL
    movq $5, %r15             # Moving constant to register
    movq %rbp, %rax           # Setting address wanted for static link
    subq $8, %rax             # Adding offset of 8 to static link
    push %rax                 # Storing static link for function
    call factorial            # Calling function
    addq $8, %rsp             # Remove static link
    pop %r15                  # Register was live in function, so restoring it after CALL
    movq $.wrt_INT, %rdi      # First argument for printf
    movq %rax, %rsi           # Second argument for printf
    movq $0, %rax             # No vector arguments
    call printf               # Calling printf
main_end:                     # End of body
    addq $16, %rsp            # Remove space for variables and spills
    movq %rbp, %rsp           # Retore old stack pointer
    pop %rbp                  # Restore old base pointer
    movq $0, %rax             # Return "no error" exit code
    ret                       # Program return
