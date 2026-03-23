;; EmotionScript TAC
; EmotionScript Three Address Code (TAC)
DECL truth canRun
canRun = yes
$t1 = canRun == yes
IF_FALSE $t1 GOTO L1
SPEAK "branch: if"
GOTO L2
LABEL L1
LABEL L2
