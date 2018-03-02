#include <string.h>
#include "../parser/include/tree.h"


char code_or_statement(expression a, expression b){
    char output     ="(eval(a)";
    strcat(output,("push  %eax           ;save value of e1 on the stack"));
    strcat(output,("eval(b)"));
    strcat(output,("pop   %ecx           ;pop e1 from the stack into ecx"));
    strcat(output,("orl   %ecx, %eax     ;compute e1 | e2, set ZF"));
    strcat(output,("movl  $0, %eax       ;zero out EAX without changing ZF"));
    strcat(output,("setne %al            ;set AL register (the lower byte of EAX) to 1 iff e1 | e2 != 0"));
    return output;
}

char code_and_statement(expression a, expression b){
    char output     = "<CODE FOR e1 GOES HERE>";
    strcat(output,("push  %eax            ;save value of e1 on the stack"));
    strcat(output,("        <CODE FOR e2 GOES HERE>"));
    strcat(output,("pop   %ecx            ;pop e1 from the stack into ECX"));
    strcat(output,("; Step 1: SET CL = 1 iff e1 != 0"));
    strcat(output,("cmpl  $0, %ecx        ;compare e1 to 0"));
    strcat(output,("setne %cl             ;set CL to 1 iff e1 != 0"));
    strcat(output,("        ; Step 2: SET AL = 1 iff e2 != 0"));
    strcat(output,("cmpl  $0, %eax        ;compare e2 to 0"));
    strcat(output,("movl  $0, %eax        ;zero EAX register before storing result"));
    strcat(output,("setne %al             ;set AL to 1 iff e2 != 0"));
    strcat(output,("        ; Step 3: compute al & cl"));
    strcat(output,("andb  %cl, %al        ;store AL & CL in AL"));
    return output;
}

char code_GQ_statement(expression a, expression b){
    char output = "<CODE FOR e1 GOES HERE>";
    return output;
}
