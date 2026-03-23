;; EmotionScript TAC
; EmotionScript Three Address Code (TAC)
CLASS Student
ATTR hidden count id
METHOD Student.get_id:
RETURN id
END_METHOD Student.get_id
END_CLASS Student
ALLOC s1, Student
CALL_METHOD_IF_EXISTS s1.init, 0
SET_FIELD s1.id, 101
$t1 = CALL_METHOD s1.get_id, 0
DROP $t1
