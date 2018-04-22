.wrt_INT:                     # Integer write label
    .string "%d\n"            # String used for printing integers
.globl main
main:                         # Start of body
    movq $50, t43             # Moving constant to register
    movq $1, t44              # Moving constant to register
    movq $2, t45              # Moving constant to register
    movq $3, t46              # Moving constant to register
    movq $4, t47              # Moving constant to register
    movq $2, t48              # Moving constant to register
    movq $2, t49              # Moving constant to register
    movq $2, t50              # Moving constant to register
    movq $2, t51              # Moving constant to register
    movq $2, t52              # Moving constant to register
    movq $2, t53              # Moving constant to register
    movq $2, t54              # Moving constant to register
    movq $2, t55              # Moving constant to register
    movq $1, t56              # Moving constant to register
    addq t43, t44             # Addition
    addq t44, t45             # Addition
    addq t45, t46             # Addition
    addq t46, t47             # Addition
    addq t47, t48             # Addition
    addq t48, t49             # Addition
    addq t49, t50             # Addition
    addq t50, t51             # Addition
    addq t51, t52             # Addition
    addq t52, t53             # Addition
    addq t53, t54             # Addition
    addq t54, t55             # Addition
    addq t55, t56             # Addition
    movq t56, t57             # Assigning value to var
    push %rax                 # Saving value of RAX before printf call
    movq $.wrt_INT, %rdi      # First argument for printf
    movq t57, %rsi            # Second argument for printf
    movq $0, %rax             # No vector arguments
    call printf               # Calling printf
    pop %rax                  # Restoring RAX
main_end:                     # End of body
    movq $0, %rax             # Return "no error" exit code
    ret                       # Program return
