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
