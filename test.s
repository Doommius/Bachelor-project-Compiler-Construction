.wrt_INT:                     # Integer write label
    .string "%d\n"            # String used for printing integers
.globl main
main:
    push %rbp
    movq %rsp, %rbp

    

    movq $.wrt_INT, %rdi      # First argument for printf
    movq %r8, %rsi           # Second argument for printf
    movq $0, %rax             # No vector arguments
    call printf               # Calling printf
   
    movq $0, %rax             # Return "no error" exit code

    movq %rbp, %rsp
    pop %rbp
    ret                       # Program return
