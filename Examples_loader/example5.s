
.data

Number1: .word 12
Number2: .word 43
Number3: .word 25

LargestNum: .word 0
.text
.globl main
main:
ldr r0, =Number1
ldr r0, [r0]
ldr r1, =Number2
ldr r1, [r1]
ldr r2, =Number3
ldr r2, [r2]
BL largest
ldr r0, =LargestNum 
str r4, [r0]
done: B done

largest:
MOV r4, r0 
CMP r4, r1 
BGE compThird 
MOV r4, r1
compThird:
CMP r4, r2 
BGE compDone
MOV r4, r2
compDone:
MOV pc, r14 
