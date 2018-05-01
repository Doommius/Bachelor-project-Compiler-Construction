.wrt_INT:                     # Integer write label
    .string "%d\n"            # String used for printing integers
.wrt_TRUE:                    # TRUE write label
    .string "TRUE\n"          # String used to print TRUE
.wrt_FALSE:                   # FALSE write label
    .string "FALSE\n"         # String used for printing FALSE
.globl main
<<<<<<< HEAD
main:                         # Start of body
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
=======
factorial:                    # Start of body
    movq %r15, t14            # Move parameter to register in function
    movq t14, t14             # Used to get target for next instruction
    movq t14, t15             # Copy val to new temp, to not harm it
    movq $0, t16              # Moving constant to register
    cmp t15, t16              # Compare, EQ
    je cmpTrue_1              # If true, jump to label
    movq $0, t17              # Setting result to 0 (false)
    jmp endCMP_1              # Jump to after compare label
cmpTrue_1:                    # Compare true label
    movq $1, t17              # Setting result to 1 (true)
endCMP_1:                     # After compare label
    movq t17, t17             # Used to get "target" when creating next instruction
    movq $1, t18              # Setting default value of result to true
    cmp $1, t17               # Compare left side of OR with true
    je endBoolCMP_2           # If true, skip right expression
    movq t14, t14             # Used to get target for next instruction
    movq t14, t19             # Copy val to new temp, to not harm it
    movq $1, t20              # Moving constant to register
    cmp t19, t20              # Compare, EQ
    je cmpTrue_3              # If true, jump to label
    movq $0, t21              # Setting result to 0 (false)
    jmp endCMP_3              # Jump to after compare label
cmpTrue_3:                    # Compare true label
    movq $1, t21              # Setting result to 1 (true)
endCMP_3:                     # After compare label
    movq t21, t21             # Used to get "target" when creating next instruction
    movq t21, t18             # Result is set to the value of the right expression
endBoolCMP_2:                 # OR expression label
    movq t18, t18             # Used to get target for next instruction
    cmp $1, t18               # Check if IF expression is true
    jne else_1                # Expression is false, jump to ELSE part
    movq t14, t14             # Used to get target for next instruction
    movq t14, t22             # Copy val to new temp, to not harm it
    movq t22, %rax            # Return value placed in RAX
    jmp end_factorial         # Jump to functions end label
    jmp if_end_1              # Skip ELSE part
else_1:                       # Start of ELSE
    movq t14, t14             # Used to get target for next instruction
    movq t14, t23             # Copy val to new temp, to not harm it
    movq t14, t14             # Used to get target for next instruction
    movq t14, t24             # Copy val to new temp, to not harm it
    movq $1, t25              # Moving constant to register
    subq t25, t24             # Subtraction
    movq t24, %r15            # Moving function parameter to register
    movq 8(%rbp), t26         # Retrieving static link
    push t26                  # Storing static link for function
    call factorial            # Calling function
    addq $8, %rsp             # Remove static link
    movq %rax, t27            # Saving return value from function in temp
    movq t23, %rax            # Using RAX for multiplication
    movq t27, %rdx            # Using RDX for multiplication
    imul %rdx                 # Multiplication using RAX and RDX
    movq %rax, t28            # Storing result here (temp)
    movq t28, %rax            # Return value placed in RAX
    jmp end_factorial         # Jump to functions end label
if_end_1:                     # End of IF
end_factorial:                # End of body
    ret                       # Return from function
main:                         # Start of body
    movq $5, t29              # Moving constant to register
    movq t29, %r15            # Moving function parameter to register
    movq %rbp, t30            # Setting address wanted for static link
    addq $8, t30              # Adding offset of 8 to static link
    push t30                  # Storing static link for function
    call factorial            # Calling function
    addq $8, %rsp             # Remove static link
    movq %rax, t31            # Saving return value from function in temp
    movq $.wrt_INT, %rdi      # First argument for printf
    movq t31, %rsi            # Second argument for printf
>>>>>>> 05fcf02e18214f0c96631372e54263aaa5632d3c
    movq $0, %rax             # No vector arguments
    call printf               # Calling printf
main_end:                     # End of body
    movq $0, %rax             # Return "no error" exit code
    ret                       # Program return
