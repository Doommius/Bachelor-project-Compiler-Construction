.wrt_INT:                     # Integer write label
    .string "%d\n"            # String used for printing integers
.globl main
main:                         # Start of body
    push %rbp                 # Pushing base pointer
    movq %rsp, %rbp           # Making stack pointer new base pointer
    subq $64, %rsp            # Reserving space for temps in body
    movq $1, t16              # Moving constant to register
    movq t14, t14             # Used to get target for next instruction
    movq t16, t14             # Assigning value to var
    movq $4, t17              # Moving constant to register
    movq t15, t15             # Used to get target for next instruction
    movq t17, t15             # Assigning value to var
loop_start_1:                 # Start of while
    movq t14, t14             # Used to get target for next instruction
    movq t14, t18             # Copy val to new temp, to not harm it
    movq t15, t15             # Used to get target for next instruction
    movq t15, t19             # Copy val to new temp, to not harm it
    cmp t18, t19              # Compare, LT
    jl cmpTrue_1              # If true, jump to label
    movq $0, t20              # Setting result to 0 (false)
    jmp endCMP_1              # Jump to after compare label
cmpTrue_1:                    # Compare true label
    movq $1, t20              # Setting result to 1 (true)
endCMP_1:                     # After compare label
    movq t20, t20             # Used to get "target" when creating next instruction
    cmp $1, t20               # Check if condition in while is true
    jne loop_end_1            # If condition is false, jump to end
    movq t14, t14             # Used to get target for next instruction
    movq t14, t21             # Copy val to new temp, to not harm it
    movq $1, t22              # Moving constant to register
    addq t21, t22             # Addition
    movq t14, t14             # Used to get target for next instruction
    movq t22, t14             # Assigning value to var
    jmp loop_start_1          # Jump to start of while
loop_end_1:                   # End of while
    movq t14, t14             # Used to get target for next instruction
    movq t14, t23             # Copy val to new temp, to not harm it
    push %rax                 # Saving value of RAX before printf call
    movq $.wrt_INT, %rdi      # First argument for printf
    movq t23, %rsi            # Second argument for printf
    movq $0, %rax             # No vector arguments
    call printf               # Calling printf
    pop %rax                  # Restoring RAX
main_end:                     # End of body
    movq %rbp, %rsp           # Restoring stack pointer
    pop %rbp                  # Restoring base pointer
    movq $0, %rax             # Return "no error" exit code
    ret                       # Program return
