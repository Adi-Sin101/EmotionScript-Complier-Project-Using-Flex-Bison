;; EmotionScript TAC
; EmotionScript Three Address Code (TAC)
DECL count i
i = 0
LABEL L1
$t1 = i < 6
IF_FALSE $t1 GOTO L2
$t2 = i
i = i + 1
DROP $t2
$t3 = i == 2
IF_FALSE $t3 GOTO L3
GOTO L1
GOTO L4
LABEL L3
LABEL L4
$t4 = i == 5
IF_FALSE $t4 GOTO L5
GOTO L2
GOTO L6
LABEL L5
LABEL L6
SPEAK i
GOTO L1
LABEL L2
