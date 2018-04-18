.wrt_INT:                     # Integer write label
    .string "%d\n"            # String used for printing integers
.globl main
main:                         # Start of body
    movq $50, t28             # Moving constant to register
    movq t28, t14             # Assigning value to var
    movq $1, t29              # Moving constant to register
    movq t29, t15             # Assigning value to var
    movq $2, t30              # Moving constant to register
    movq t30, t16             # Assigning value to var
    movq $3, t31              # Moving constant to register
    movq t31, t17             # Assigning value to var
    movq $4, t32              # Moving constant to register
    movq t32, t18             # Assigning value to var
    movq $2, t33              # Moving constant to register
    movq t33, t19             # Assigning value to var
    movq $2, t34              # Moving constant to register
    movq t34, t20             # Assigning value to var
    movq $2, t35              # Moving constant to register
    movq t35, t21             # Assigning value to var
    movq $2, t36              # Moving constant to register
    movq t36, t22             # Assigning value to var
    movq $2, t37              # Moving constant to register
    movq t37, t23             # Assigning value to var
    movq $2, t38              # Moving constant to register
    movq t38, t24             # Assigning value to var
    movq $2, t39              # Moving constant to register
    movq t39, t25             # Assigning value to var
    movq $2, t40              # Moving constant to register
    movq t40, t26             # Assigning value to var
    movq t14, t41             # Copy val to new temp, to not harm it
    movq t15, t42             # Copy val to new temp, to not harm it
    addq t41, t42             # Addition
    movq t16, t43             # Copy val to new temp, to not harm it
    addq t42, t43             # Addition
    movq t17, t44             # Copy val to new temp, to not harm it
    addq t43, t44             # Addition
    movq t18, t45             # Copy val to new temp, to not harm it
    addq t44, t45             # Addition
    movq t19, t46             # Copy val to new temp, to not harm it
    addq t45, t46             # Addition
    movq t20, t47             # Copy val to new temp, to not harm it
    addq t46, t47             # Addition
    movq t21, t48             # Copy val to new temp, to not harm it
    addq t47, t48             # Addition
    movq t22, t49             # Copy val to new temp, to not harm it
    addq t48, t49             # Addition
    movq t23, t50             # Copy val to new temp, to not harm it
    addq t49, t50             # Addition
    movq t24, t51             # Copy val to new temp, to not harm it
    addq t50, t51             # Addition
    movq t25, t52             # Copy val to new temp, to not harm it
    addq t51, t52             # Addition
    movq t26, t53             # Copy val to new temp, to not harm it
    addq t52, t53             # Addition
    movq t53, t27             # Assigning value to var
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
