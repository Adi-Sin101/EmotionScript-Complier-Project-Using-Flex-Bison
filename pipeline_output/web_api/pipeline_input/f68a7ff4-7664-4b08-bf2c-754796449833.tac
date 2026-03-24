;; EmotionScript TAC
; EmotionScript Three Address Code (TAC)
CLASS Student
ATTR hidden count id
END_CLASS Student
ALLOC s1, Student
CALL_METHOD_IF_EXISTS s1.init, 0
SET_FIELD s1.id, 101
$t1 = GET_FIELD s1.id
SPEAK $t1
