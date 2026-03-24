;; EmotionScript TAC
; EmotionScript Three Address Code (TAC)
CLASS Human
METHOD Human.greet:
SPEAK "Human says hi"
RETURN yes
END_METHOD Human.greet
END_CLASS Human
CLASS Teacher EXTENDS Human
METHOD Teacher.greet:
SPEAK "Teacher says hi"
RETURN yes
END_METHOD Teacher.greet
END_CLASS Teacher
ALLOC t1, Teacher
CALL_METHOD_IF_EXISTS t1.init, 0
$t1 = CALL_METHOD t1.greet, 0
DROP $t1
