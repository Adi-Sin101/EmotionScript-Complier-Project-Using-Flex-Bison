;; EmotionScript TAC
; EmotionScript Three Address Code (TAC)
DECL count x
x = 4
$t1 = x > 1
IF_FALSE $t1 GOTO L1
$t2 = x > 3
IF_FALSE $t2 GOTO L3
SPEAK "inner true"
GOTO L4
LABEL L3
SPEAK "inner false"
LABEL L4
GOTO L2
LABEL L1
LABEL L2
