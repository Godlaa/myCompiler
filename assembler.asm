.data
i QWORD ?
x QWORD ?
.code
expression proc

mov rdx,2
push rdx
mov rdx,2
push rdx
pop rdx
pop rax
add rax, rdx
push rax
mov rdx,2
push rdx
pop rdx
pop rax
add rax, rdx
push rax
mov rdx,2
push rdx
pop rdx
pop rax
add rax, rdx
push rax
mov rdx,2
push rdx
pop rdx
pop rax
add rax, rdx
push rax
pop rax
mov i, rax
mov rdx,i
push rdx
mov rdx,2
push rdx
pop rdx
pop rax
mul rdx
push rax
pop rax
mov x, rax
ret
expression ENDP
end