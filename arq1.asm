

main:
    addi 7
    addi 3
    add r1, r0
    addi -7
    addi -3

loop:    
    addi -1
    add r1, r0
    addi 1
    ji pulo0

volta3:
    ji loop    

pulo0:
    addi A
    add r0, r1
    ld r2, r0
    add r3, r2
    sub r0, r0
    ji pulo1

volta2:
    ji volta3

pulo1:
    addi B
    add r0, r1
    ld r2, r0
    add r3, r2
    sub r0, r0
    ji pulo2

volta1:
    ji volta2

pulo2:
    addi R
    add r0, r1
    st r3, r0
    sub r0, r0

branch:
    brzr r1, branch
    ji volta1
    ebreak

A: 
.bits8 0 2 4 6 8 10 12 14 16 18

B: 
.bits8 1 3 5 7 9 11 13 15 17 19

R: 
.bits8 0 0 0 0 0 0 0 0 0 0




