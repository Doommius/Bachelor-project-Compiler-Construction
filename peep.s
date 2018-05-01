.wrt_INT:                     # Integer write label
    .string "%d\n"            # String used for printing integers
.wrt_TRUE:                    # TRUE write label
    .string "TRUE\n"          # String used to print TRUE
.wrt_FALSE:                   # FALSE write label
    .string "FALSE\n"         # String used for printing FALSE
.globl main
factorial:                    # Start of body
    movq %r15, t14            # Move parameter to register in function
    movq 8(%rbp), t15         # Copy static link to new reg
    movq t15, t16             # Copy val to new temp, to not harm it
    movq $.wrt_INT, %rdi      # First argument for printf
    movq t16, %rsi            # Second argument for printf
    movq $0, %rax             # No vector arguments
    call printf               # Calling printf
    movq t14, t17             # Copy val to new temp, to not harm it
    movq 8(%rbp), t18         # Copy static link to new reg
    movq t18, t19             # Copy val to new temp, to not harm it
    movq t17, %rax            # Using RAX for multiplication
    movq t19, %rdx            # Using RDX for multiplication
    imul %rdx                 # Multiplication using RAX and RDX
    movq %rax, t20            # Storing result here (temp)
    movq t20, %rax            # Return value placed in RAX
    jmp end_factorial         # Jump to functions end label
end_factorial:                # End of body
    ret                       # Return from function
main:                         # Start of body
    movq $2, t21              # Moving constant to register
    movq t21, -8(%rbp)        # Assigning value to var
    movq $5, t22              # Moving constant to register
    movq t22, %r15            # Moving function parameter to register
    movq %rbp, t23            # Setting address wanted for static link
    addq $8, t23              # Adding offset of 8 to static link
    push t23                  # Storing static link for function
    call factorial            # Calling function
    addq $8, %rsp             # Remove static link
    movq %rax, t24            # Saving return value from function in temp
    movq $.wrt_INT, %rdi      # First argument for printf
    movq t24, %rsi            # Second argument for printf
    movq $0, %rax             # No vector arguments
    call printf               # Calling printf
main_end:                     # End of body
    movq $0, %rax             # Return "no error" exit code
    ret                       # Program return
