.data

fibinput: .word 16

fibnumber: .word 0
.text
.globl main
main:
ldr r0, =fibinput
ldr r0, [r0]
BL fibonacci
ldr r1, =fibnumber
str r0, [r1]
done: B done

fibonacci:
STMFD SP!, {r4, r5, lr}
CMP r0, #0 
BGT NZ
MOV r0, #0
B RETURN
NZ: CMP r0, #1
BEQ RETONE
CMP r0, #2
BEQ RETONE
MOV r4, r0
SUB r0, r0, #1
BL fibonacci
MOV r5, r0
MOV r0, r4
SUB r0, r0, #2
BL fibonacci
ADD r0, r0, r5 
B RETURN
RETONE:
MOV r0, #1
RETURN:
 LDMFD SP!, {r4, r5, lr} 
MOV pc, lr 