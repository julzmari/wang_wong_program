section .text
bits 64
default rel
global saxpy_asm

saxpy_asm:
    ; Perform SAXPY (A * X[i] + Y[i])

L1: ; n=rcx, vec1=rdx, vec2=r8, vec3=r9, a=rsp+40

    movss XMM4, [rsp+40]           ; Load A into XMM4 (debugging step)
    movss XMM1, [rdx]              ; Load X[i] into XMM1
    vmulss XMM2, XMM1, XMM4        ; Multiply X[i] * A (stored in XMM4)
    movss XMM1, [r8]               ; Load Y[i] into XMM1
    vaddss XMM3, XMM1, XMM2        ; Add Y[i] to X[i]*A and store in XMM3
    movss [r9], XMM3               ; Store result into vec3 (r9)

    add rdx, 4                     ; Move to the next element in vec1
    add r8, 4                      ; Move to the next element in vec2
    add r9, 4                      ; Move to the next element in vec3
    dec rcx                        ; Decrement the loop counter (n)
    jnz L1                         ; Jump if rcx is not zero (repeat the loop)

    ret
