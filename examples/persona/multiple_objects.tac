;; EmotionScript TAC
; EmotionScript Three Address Code (TAC)
CLASS Student
ATTR hidden count id
END_CLASS Student
ALLOC s1, Student
CALL_METHOD_IF_EXISTS s1.init, 0
ALLOC s2, Student
CALL_METHOD_IF_EXISTS s2.init, 0
SET_FIELD s1.id, 101
SET_FIELD s2.id, 202
$t1 = GET_FIELD s1.id
SPEAK $t1
$t2 = GET_FIELD s2.id
SPEAK $t2
