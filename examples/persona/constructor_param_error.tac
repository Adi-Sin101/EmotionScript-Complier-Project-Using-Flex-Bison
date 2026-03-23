;; EmotionScript TAC
; EmotionScript Three Address Code (TAC)
CLASS Student
ATTR open count id
METHOD Student.init:
FORMAL x
id = x
RETURN yes
END_METHOD Student.init
END_CLASS Student
ALLOC s1, Student
PARAM "oops"
CALL_METHOD_IF_EXISTS s1.init, 1
