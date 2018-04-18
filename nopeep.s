.wrt_INT:                     # Integer write label
    .string "%d\n"            # String used for printing integers
.globl main
main:                         # Start of body
    movq $50, t28             # Moving constant to register
    movq t14, t14             # Used to get target for next instruction
    movq t28, t14             # Assigning value to var
    movq $1, t29              # Moving constant to register
    movq t15, t15             # Used to get target for next instruction
    movq t29, t15             # Assigning value to var
    movq $2, t30              # Moving constant to register
    movq t16, t16             # Used to get target for next instruction
    movq t30, t16             # Assigning value to var
    movq $3, t31              # Moving constant to register
    movq t17, t17             # Used to get target for next instruction
    movq t31, t17             # Assigning value to var
    movq $4, t32              # Moving constant to register
    movq t18, t18             # Used to get target for next instruction
    movq t32, t18             # Assigning value to var
    movq $2, t33              # Moving constant to register
    movq t19, t19             # Used to get target for next instruction
    movq t33, t19             # Assigning value to var
    movq $2, t34              # Moving constant to register
    movq t20, t20             # Used to get target for next instruction
    movq t34, t20             # Assigning value to var
    movq $2, t35              # Moving constant to register
    movq t21, t21             # Used to get target for next instruction
    movq t35, t21             # Assigning value to var
    movq $2, t36              # Moving constant to register
    movq t22, t22             # Used to get target for next instruction
    movq t36, t22             # Assigning value to var
    movq $2, t37              # Moving constant to register
    movq t23, t23             # Used to get target for next instruction
    movq t37, t23             # Assigning value to var
    movq $2, t38              # Moving constant to register
    movq t24, t24             # Used to get target for next instruction
    movq t38, t24             # Assigning value to var
    movq $2, t39              # Moving constant to register
    movq t25, t25             # Used to get target for next instruction
    movq t39, t25             # Assigning value to var
    movq $2, t40              # Moving constant to register
    movq t26, t26             # Used to get target for next instruction
    movq t40, t26             # Assigning value to var
    movq t14, t14             # Used to get target for next instruction
    movq t14, t41             # Copy val to new temp, to not harm it
    movq t15, t15             # Used to get target for next instruction
    movq t15, t42             # Copy val to new temp, to not harm it
    addq t41, t42             # Addition
    movq t16, t16             # Used to get target for next instruction
    movq t16, t43             # Copy val to new temp, to not harm it
    addq t42, t43             # Addition
    movq t17, t17             # Used to get target for next instruction
    movq t17, t44             # Copy val to new temp, to not harm it
    addq t43, t44             # Addition
    movq t18, t18             # Used to get target for next instruction
    movq t18, t45             # Copy val to new temp, to not harm it
    addq t44, t45             # Addition
    movq t19, t19             # Used to get target for next instruction
    movq t19, t46             # Copy val to new temp, to not harm it
    addq t45, t46             # Addition
    movq t20, t20             # Used to get target for next instruction
    movq t20, t47             # Copy val to new temp, to not harm it
    addq t46, t47             # Addition
    movq t21, t21             # Used to get target for next instruction
    movq t21, t48             # Copy val to new temp, to not harm it
    addq t47, t48             # Addition
    movq t22, t22             # Used to get target for next instruction
    movq t22, t49             # Copy val to new temp, to not harm it
    addq t48, t49             # Addition
    movq t23, t23             # Used to get target for next instruction
    movq t23, t50             # Copy val to new temp, to not harm it
    addq t49, t50             # Addition
    movq t24, t24             # Used to get target for next instruction
    movq t24, t51             # Copy val to new temp, to not harm it
    addq t50, t51             # Addition
    movq t25, t25             # Used to get target for next instruction
    movq t25, t52             # Copy val to new temp, to not harm it
    addq t51, t52             # Addition
    movq t26, t26             # Used to get target for next instruction
    movq t26, t53             # Copy val to new temp, to not harm it
    addq t52, t53             # Addition
    movq t27, t27             # Used to get target for next instruction
    movq t53, t27             # Assigning value to var
    movq t27, t27             # Used to get target for next instruction
    movq t27, t54             # Copy val to new temp, to not harm it
    push %rax                 # Saving value of RAX before printf call
    movq $.wrt_INT, %rdi      # First argument for printf
    movq t54, %rsi            # Second argument for printf
    movq $0, %rax             # No vector arguments
    call printf               # Calling printf
    pop %rax                  # Restoring RAX
main_end:                     # End of body
    movq $0, %rax             # Return "no error" exit code
    ret                       # Program return
