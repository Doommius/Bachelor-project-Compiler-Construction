.wrt_INT:                     # Integer write label
    .string "%d\n"            # String used for printing integers
.globl main
main:
    push %rbp
    movq %rsp, %rbp

    

    subq $24, %rsp
    movq $10, 8(%rsp)
    movq $25, 16(%rsp)
    movq -16(%rbp), %r8

    movq $.wrt_INT, %rdi      # First argument for printf
    movq %r8, %rsi           # Second argument for printf
    movq $0, %rax             # No vector arguments
    call printf               # Calling printf
    pop %r10
    pop %rbx
    pop %rax
    movq $0, %rax             # Return "no error" exit code

    movq %rbp, %rsp
    pop %rbp
    ret                       # Program return
