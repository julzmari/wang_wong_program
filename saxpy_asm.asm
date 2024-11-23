section .data
; No data here; all passed via arguments

section .text
global saxpy_c

saxpy_asm:
    ; Arguments: xmm0 (A), rdi (X), rsi (Y), rdx (Z), rcx (n)
.loop_start:
    test rcx, rcx       ; Check if n == 0
    jz .done            ; Exit if done

    movss xmm1, [rdi]   ; Load X[i] into xmm1
    mulss xmm1, xmm0    ; A * X[i]
    addss xmm1, [rsi]   ; (A * X[i]) + Y[i]
    movss [rdx], xmm1   ; Store result in Z[i]

    ; Increment pointers
    add rdi, 4          ; X++
    add rsi, 4          ; Y++
    add rdx, 4          ; Z++
    dec rcx             ; n--
    jmp .loop_start    

.done:
    ret
