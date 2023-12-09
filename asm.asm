.data
    x QWORD ?
    i QWORD ?
    temp QWORD ?
.code
somefunction PROC

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
somefunction ENDP
end