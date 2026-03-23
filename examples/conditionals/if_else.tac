;; EmotionScript TAC
; EmotionScript Three Address Code (TAC)
DECL count score
score = 2
$t1 = score > 5
IF_FALSE $t1 GOTO L1
SPEAK "high"
GOTO L2
LABEL L1
SPEAK "low"
LABEL L2
