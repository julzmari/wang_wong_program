section .text
bits 64
default rel
global saxpy_asm

saxpy_asm:
    ; Arguments:
    ; n      -> rcx (number of elements)
    ; X      -> rdx (pointer to array X)
    ; Y      -> r8  (pointer to array Y)
    ; Z      -> r9  (pointer to array Z, result storage)
    ; A      -> passed on the stack at [rsp+40] (scalar value A)

    ; Load scalar A from stack into XMM4 and broadcast it to all 4 positions
    movss XMM4, [rsp+40]          ; Load A into XMM4
    shufps XMM4, XMM4, 0x00       ; Broadcast A to all elements in XMM4

    ; Align loop to process 4 elements at a time using SIMD
    mov rax, rcx                  ; Copy 'n' to rax for SIMD operations
    shr rcx, 2                    ; Divide n by 4 to get SIMD iteration count
    jz scalar_process             ; If no full sets of 4, jump to scalar processing

simd_loop:
    ; Process 4 elements of X and Y in parallel
    movups XMM0, [rdx]            ; Load 4 elements of X into XMM0
    mulps XMM0, XMM4              ; Multiply 4 elements of X by A
    movups XMM1, [r8]             ; Load 4 elements of Y into XMM1
    addps XMM0, XMM1              ; Add 4 elements of Y to (X * A)
    movups [r9], XMM0             ; Store result in Z (4 elements)

    ; Move to the next set of 4 elements
    add rdx, 16                   ; Move pointer X by 4 elements (4*4 bytes)
    add r8, 16                    ; Move pointer Y by 4 elements (4*4 bytes)
    add r9, 16                    ; Move pointer Z by 4 elements (4*4 bytes)

    dec rcx                       ; Decrement SIMD loop counter
    jnz simd_loop                 ; Repeat SIMD loop if rcx != 0

    ; Handle remaining elements (if n was not a multiple of 4)
scalar_process:
    ; rax still holds the original n
    and rax, 3                    ; Mask out multiples of 4, leaving remainder
    jz end_saxpy                  ; If no remaining elements, finish

L1:
    ; Scalar processing of remaining elements (1 element at a time)
    movss XMM1, [rdx]             ; Load X[i] into XMM1
    mulss XMM1, XMM4              ; Multiply X[i] by A
    movss XMM2, [r8]              ; Load Y[i] into XMM2
    addss XMM1, XMM2              ; Add Y[i] to (X[i] * A)
    movss [r9], XMM1              ; Store result in Z[i]

    ; Move to the next single element
    add rdx, 4                    ; Move to the next element in X
    add r8, 4                     ; Move to the next element in Y
    add r9, 4                     ; Move to the next element in Z
    dec rax                       ; Decrement scalar counter
    jnz L1                        ; Repeat scalar loop if rax != 0

end_saxpy:
    ret
