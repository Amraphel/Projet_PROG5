.global main
.text
main:
    mov     r1, #2    
    mov     r2, #3   
    cmp     r1, r2   
    blt     r1_lower  
    mov     r0, r1    
    b       end        
r1_lower:
    mov r0, r2       
    b end            
end:
    bx lr             