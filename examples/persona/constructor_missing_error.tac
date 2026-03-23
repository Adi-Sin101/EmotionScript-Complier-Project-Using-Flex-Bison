;; EmotionScript TAC
; EmotionScript Three Address Code (TAC)
CLASS Student
ATTR open count id
END_CLASS Student
ALLOC s1, Student
PARAM 101
CALL_METHOD_IF_EXISTS s1.init, 1
