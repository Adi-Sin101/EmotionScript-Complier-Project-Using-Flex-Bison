;; EmotionScript TAC
; EmotionScript Three Address Code (TAC)
DECL count i
i = 0
LABEL L1
$t1 = i < 3
IF_FALSE $t1 GOTO L2
SPEAK i
$t2 = i
i = i + 1
DROP $t2
GOTO L1
LABEL L2
