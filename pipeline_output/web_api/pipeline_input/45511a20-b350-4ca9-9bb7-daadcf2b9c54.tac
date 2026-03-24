;; EmotionScript TAC
; EmotionScript Three Address Code (TAC)
DECL count i
i = 0
LABEL L1
$t1 = i < 3
IF_FALSE $t1 GOTO L3
SPEAK i
LABEL L2
$t2 = i
i = i + 1
GOTO L1
LABEL L3
