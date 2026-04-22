ADD $t0, $s1, $s2
ADDI $t0, $s1, #5
BNE $s1, $s2, #25
AND $t0, $s1, $s2
ANDI $t0, $s1, #12
BEQ $s1, $s2, #25
DIV $s1, $s2
LUI $t0, #100
LW $t0, #4, $s0
MFHI $t0
MFLO $t0
OR $t0, $s1, $s2
SLT $t0, $s1, $s2
SUB $t0, $s1, $s2
SW $t0, #8, $s0
MULT $s1, $s2
ORI $t0, $s1, #12
SLTI $t0, $s1, #7
