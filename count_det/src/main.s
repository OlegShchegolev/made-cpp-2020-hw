.text

.global count_det
count_det:
            mov  r3, r0
            ldr r0, [r3]
            ldr r1, [r3, #16]
            mul r0, r1
            ldr r1, [r3, #32]
            mul r0, r1
            ldr r1, [r3, #4]
            ldr r2, [r3, #20]
            mul r1, r2
            ldr r2, [r3, #24]
            mul r1, r2
            add r0, r1
            ldr r1, [r3, #12]
            ldr r2, [r3, #28]
            mul r1, r2
            ldr r2, [r3, #8]
            mul r1, r2
            add r0, r1
            ldr r1, [r3, #8]
            ldr r2, [r3, #16]
            mul r1, r2
            ldr r2, [r3, #24]
            mul r1, r2
            sub r0, r1
            ldr r1, [r3, #4]
            ldr r2, [r3, #12]
            mul r1, r2
            ldr r2, [r3, #32]
            mul r1, r2
            sub r0, r1
            ldr r1, [r3]
            ldr r2, [r3, #20]
            mul r1, r2
            ldr r2, [r3, #28]
            mul r1, r2
            sub r0, r1
            bx  lr
