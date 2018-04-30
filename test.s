.wrt_INT:                     # Integer write label
    .string "%d\n"            # String used for printing integers
.globl main
main:
    push %rbp
    movq %rsp, %rbp
    push %r15

    movq $5, %rax
    movq $2, %rdx
    imul %rdx

    

    movq $.wrt_INT, %rdi      # First argument for printf
    movq %rdx, %rsi           # Second argument for printf
    movq $0, %rax             # No vector arguments
    call printf               # Calling printf
   
    movq $0, %rax             # Return "no error" exit code

    pop %r15
    movq %rbp, %rsp
    pop %rbp
    ret                       # Program return
