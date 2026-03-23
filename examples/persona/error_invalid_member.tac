;; EmotionScript TAC
; EmotionScript Three Address Code (TAC)
CLASS Student
ATTR hidden count id
END_CLASS Student
ALLOC s1, Student
CALL_METHOD_IF_EXISTS s1.init, 0
$t1 = GET_FIELD s1.age
SPEAK $t1
