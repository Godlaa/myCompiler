includelib ucrt.lib
includelib legacy_stdio_definitions.lib
EXTERN printf : PROC
.data
temp real4 ?
fmtStr byte 'Result is: %d', 10, 0
fmtStr2 byte 'Result is: %f', 10, 0
x real4 ?
i QWORD ?
xt QWORD ?

.code
print proc
sub rsp, 20h
lea rcx, fmtStr; param 1: format string
mov rdx, rax
call printf
add rsp, 20h; Restore the stack
ret
print endp

printFloat proc
sub rsp, 20h
movss xmm0, [temp]
cvtss2sd xmm1, xmm0
movd rdx, xmm1
lea rcx, fmtStr2
call printf
add rsp, 20h
ret
printFloat endp
expression proc

mov rdx,20
push rdx
mov rdx,5
push rdx
pop rdx
pop rax
mul rdx
push rax
mov rcx,5
cvtsi2ss xmm1, rcx
movd temp, xmm1
fld temp
mov rcx,10
cvtsi2ss xmm1, rcx
movd temp, xmm1
fld temp
fdiv
mov rcx,10
cvtsi2ss xmm1, rcx
movd temp, xmm1
fld temp
fadd
pop rax
cvtsi2ss xmm1, rax
movd temp, xmm1
fld temp
fmul
fistp i
mov rdx,2
push rdx
mov rcx,5
cvtsi2ss xmm1, rcx
movd temp, xmm1
fld temp
mov rcx,10
cvtsi2ss xmm1, rcx
movd temp, xmm1
fld temp
fdiv
mov rcx,2
cvtsi2ss xmm1, rcx
movd temp, xmm1
fld temp
fadd
pop rax
cvtsi2ss xmm1, rax
movd temp, xmm1
fld temp
fmul
mov rdx,i
push rdx
pop rax
cvtsi2ss xmm1, rax
movd temp, xmm1
fld temp
fmul
fstp x
fld x
mov rdx,2
push rdx
pop rax
cvtsi2ss xmm1, rax
movd temp, xmm1
fld temp
fmul
fistp xt
fld x
fstp temp
call printfloat
ret
expression ENDP
end