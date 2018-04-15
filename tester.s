.wrt_INT:                     # Integer write label
    .string "%d\n"            # String used for printing integers
.globl main
    movq $1, %rbx             # RBX is nr. 1
    movq $0, %rcx             # RCX is nr. 2
    movq $0, %rdx             # RDX is nr. 3
    movq $0, %rsi             # RSI is nr. 4
loop:                         # loop
    andq %rbx, %rdi           # 
    cmp $0, %rdi              # compare
    jne else                  # jump
    addq %rbx, %rcx           # add
    jmp endif                 # jump
else:                         # 
    addq %rbx, %rdx           # add
endif:                        # 
    addq %rbx, %rsi           # add
    addq $1, %rbx             # add
    cmp $9, %rbx              # 
    jle loop                  # 
