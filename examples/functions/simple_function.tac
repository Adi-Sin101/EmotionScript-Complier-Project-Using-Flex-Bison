;; EmotionScript TAC
; EmotionScript Three Address Code (TAC)
FUNC greet:
SPEAK "hello"
RETURN 0
END_FUNC greet
$t1 = CALL greet, 0
DROP $t1
