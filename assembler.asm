includelib ucrt.lib
includelib legacy_stdio_definitions.lib

EXTERN printf: PROC

.data
fmtStr2 DB "Result is: %d", 10, 0
i QWORD ?
x QWORD ?
.code
expression proc

    mov rdx,4
    push rdx
    mov rdx,1
    push rdx
    mov rdx,2
    push rdx
    pop rdx
    pop rax
    sub rax, rdx
    push rax
    pop rdx
    pop rax
    mul rdx
    push rax
    pop rax
    mov i, rax
    mov rax,i

ret
expression ENDP
end