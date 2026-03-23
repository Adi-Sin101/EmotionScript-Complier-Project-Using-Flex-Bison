;; EmotionScript TAC
; EmotionScript Three Address Code (TAC)
CLASS Human
ATTR hidden count age
END_CLASS Human
CLASS Student EXTENDS Human
ATTR hidden count id
END_CLASS Student
ALLOC s1, Student
CALL_METHOD_IF_EXISTS s1.init, 0
SET_FIELD s1.age, 20
$t1 = GET_FIELD s1.age
SPEAK $t1
