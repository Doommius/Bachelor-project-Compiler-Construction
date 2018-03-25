.wrt_INT:                     # Integer write label
    .string "%d\n"            # String used for printing integers
.globl main
main:
    movq $2, %r8               # Moving constant to register
    movq $2, %r9               # Moving constant to register
    addq %r8, %r9               # Addition
    push %rax                 # Saving value of RAX before printf call
    movq $.wrt_INT, %rdi      # First argument for printf
    movq %r9, %rsi             # Second argument for printf
    movq $0, %rax             # No vector arguments
    call printf               # Calling printf
    pop %rax                  # Restoring RAX
