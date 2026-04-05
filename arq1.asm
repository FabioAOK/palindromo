main:
    addi 9
    add r1, r0
    addi 7

loop:
    addi A
    add r0, r1
    ld r2, r0
    add r3, r2
    sub r0, r0

    ji 2
    ji -6

    addi B
    add r0, r1
    ld r2, r0
    add r3, r2
    sub r0, r0

    ji 2
    ji -7

    addi R
    add r0, r1
    st r3, r0
    sub r0, r0

    ji 2
    ji -6

    addi -1
    add r1, r0
    addi 1

    brzr r0, r0
    ji -5


A: 
.bits8 0 2 4 6 8 10 12 14 16 18
B: 
.bits8 1 3 5 7 9 11 13 15 17 19
R: 
.bits8 0 0 0 0 0 0 0 0 0 0


