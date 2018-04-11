.wrt_INT:                     # Integer write label
    .string "%d\n"            # String used for printing integers
.globl main
factorial:                    # Start of body
    push %rbp                 # Pushing base pointer
    movq %rsp, %rbp           # Making stack pointer new base pointer
    subq $24, %rsp            # Reserving space for temps in body
    movq 8(%rbp), t1          # Copy val to new temp, to not harm it
    movq $2, t2               # Moving constant to register
    addq t1, t2               # Addition
    movq $1, t3               # Moving constant to register
    addq t2, t3               # Addition
    movq t3, %rax             # Return value placed in RAX
    jmp end_factorial         # Jump to functions end label
end_factorial:                # End of body
    movq %rbp, %rsp           # Restoring stack pointer
    push %rbp                 # Restoring base pointer
    ret                       # Return from function
main:                         # Start of body
    push %rbp                 # Pushing base pointer
    movq %rsp, %rbp           # Making stack pointer new base pointer
    subq $16, %rsp            # Reserving space for temps in body
    movq $5, t4               # Moving constant to register
    push t4                   # Push argument for function
    call factorial            # Calling function
    movq %rax, t5             # Saving return value from function in temp
    push %rax                 # Saving value of RAX before printf call
    movq $.wrt_INT, %rdi      # First argument for printf
    movq t5, %rsi             # Second argument for printf
    movq $0, %rax             # No vector arguments
    call printf               # Calling printf
    pop %rax                  # Restoring RAX
main_end:                     # End of body
    movq %rbp, %rsp           # Restoring stack pointer
    push %rbp                 # Restoring base pointer
    movq $0, %rax             # Return "no error" exit code
    ret                       # Program return
