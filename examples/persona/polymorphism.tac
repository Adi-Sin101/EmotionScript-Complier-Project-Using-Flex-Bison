;; EmotionScript TAC
; EmotionScript Three Address Code (TAC)
CLASS Human
METHOD Human.role:
SPEAK "Human role"
RETURN yes
END_METHOD Human.role
END_CLASS Human
CLASS Teacher EXTENDS Human
METHOD Teacher.role:
SPEAK "Teacher role"
RETURN yes
END_METHOD Teacher.role
END_CLASS Teacher
ALLOC h1, Human
CALL_METHOD_IF_EXISTS h1.init, 0
ALLOC t1, Teacher
CALL_METHOD_IF_EXISTS t1.init, 0
$t1 = CALL_METHOD h1.role, 0
DROP $t1
$t2 = CALL_METHOD t1.role, 0
DROP $t2
