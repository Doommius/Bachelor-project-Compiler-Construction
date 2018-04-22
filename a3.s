.wrt_INT:                     # Integer write label
    .string "%d\n"            # String used for printing integers
.globl main
main:                         # Start of body
    movq $50, %r15            # Moving constant to register
    movq $1, %r14             # Moving constant to register
    movq $2, %r13             # Moving constant to register
    movq $3, %r12             # Moving constant to register
    movq $4, %r11             # Moving constant to register
    movq $2, %r10             # Moving constant to register
    movq $2, %r9              # Moving constant to register
    movq $2, %r8              # Moving constant to register
    movq $2, %rdi             # Moving constant to register
    movq $2, %rdx             # Moving constant to register
    movq $2, %rcx             # Moving constant to register
    movq $2, %rbx             # Moving constant to register
    movq $2, %rsi             # Moving constant to register
    addq %r15, %r14           # Addition
    addq %r14, %r13           # Addition
    addq %r13, %r12           # Addition
    addq %r12, %r11           # Addition
    addq %r11, %r10           # Addition
    addq %r10, %r9            # Addition
    addq %r9, %r8             # Addition
    addq %r8, %rdi            # Addition
    addq %rdi, %rdx           # Addition
    addq %rdx, %rcx           # Addition
    addq %rcx, %rbx           # Addition
    addq %rbx, %rsi           # Addition
    push %rax                 # Saving value of RAX before printf call
    movq $.wrt_INT, %rdi      # First argument for printf
    movq $0, %rax             # No vector arguments
    call printf               # Calling printf
    pop %rax                  # Restoring RAX
main_end:                     # End of body
    movq $0, %rax             # Return "no error" exit code
    ret                       # Program return
