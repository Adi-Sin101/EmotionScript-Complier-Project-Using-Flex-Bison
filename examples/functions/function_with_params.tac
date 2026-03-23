;; EmotionScript TAC
; EmotionScript Three Address Code (TAC)
FUNC add:
FORMAL a
FORMAL b
DECL count c
$t1 = a + b
c = $t1
RETURN c
END_FUNC add
PARAM 1
PARAM 2
$t2 = CALL add, 2
DROP $t2
