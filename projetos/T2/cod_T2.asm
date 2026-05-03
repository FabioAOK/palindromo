    addir1 7
    addir1 3
    
    addir1 -1
    addi20
    addi20

    ji 2
    ji -4

    add r0, r1
    ld r2, r0
    add r3, r2
    sub r0, r0

    ji 2
    ji -6

    addi20
    addi20
    addi10

    ji 2
    ji -5

    add r0, r1
    ld r2, r0
    add r3, r2
    sub r0, r0

    ji 2
    ji -6

    addi20
    addi20
    addi20

    ji 2
    ji -5

    add r0, r1
    st r3, r0
    sub r3, r3
    sub r0, r0

    brzr r1, r1
    ji -6

A: está no endereço 0x28 
 0 2 4 6 8 10 12 14 16 18

B: está no endereço 0x32 
1 3 5 7 9 11 13 15 17 19

R: está no endereço 0x3c
0 0 0 0 0 0 0 0 0 0