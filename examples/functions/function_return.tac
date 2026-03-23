;; EmotionScript TAC
; EmotionScript Three Address Code (TAC)
DECL count result
FUNC getFive:
RETURN 5
END_FUNC getFive
$t1 = CALL getFive, 0
result = $t1
SPEAK result
