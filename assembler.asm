.data
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