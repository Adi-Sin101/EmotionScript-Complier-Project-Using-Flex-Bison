;; EmotionScript TAC
; EmotionScript Three Address Code (TAC)
FUNC multiply:
FORMAL x
FORMAL y
$t1 = x * y
RETURN $t1
END_FUNC multiply
FUNC add:
FORMAL a
FORMAL b
$t2 = a + b
RETURN $t2
END_FUNC add
PARAM 3
PARAM 7
$t3 = CALL multiply, 2
DROP $t3
PARAM 10
PARAM 20
$t4 = CALL add, 2
DROP $t4
