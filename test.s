.wrt_INT:                     # Integer write label
    .string "%d\n"            # String used for printing integers
.globl main
main:
    push %rbp
    movq %rsp, %rbp
    subq $16, %rsp
    movq $2, %rax
    movq %rax, -8(%rbp)
    movq $16, 0(%rbp)
    movq %rbp, %rbx
    subq $8, %rbx

    

    

    movq $.wrt_INT, %rdi      # First argument for printf
    movq 8(%rsp), %rsi           # Second argument for printf
    movq $0, %rax             # No vector arguments
    call printf               # Calling printf
   
    movq $0, %rax             # Return "no error" exit code

    addq $16, %rsp
    movq %rbp, %rsp
    pop %rbp
    ret                       # Program return
