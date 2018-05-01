.wrt_INT:                     # Integer write label
    .string "%d\n"            # String used for printing integers
.globl main
main:                         # Start of body
    movq $1, %rdx             # Moving constant to register
    movq $4, %rcx             # Moving constant to register
loop_start_1:                 # Start of while
    cmp %rdx, %rcx            # Compare, LT
    jl cmpTrue_1              # If true, jump to label
    movq $0, %rbx             # Setting result to 0 (false)
    jmp endCMP_1              # Jump to after compare label
cmpTrue_1:                    # Compare true label
    movq $1, %rbx             # Setting result to 1 (true)
endCMP_1:                     # After compare label
    cmp $1, %rbx              # Check if condition in while is true
    jne loop_end_1            # If condition is false, jump to end
    movq $1, %rbx             # Moving constant to register
    addq %rsi, %rbx           # Addition
    movq %rbx, %rsi           # Assigning value to var
    jmp loop_start_1          # Jump to start of while
loop_end_1:                   # End of while
    push %rax                 # Saving value of RAX before printf call
    movq $.wrt_INT, %rdi      # First argument for printf
    movq $0, %rax             # No vector arguments
    call printf               # Calling printf
    pop %rax                  # Restoring RAX
main_end:                     # End of body
    movq $0, %rax             # Return "no error" exit code
    ret                       # Program return
