;; EmotionScript TAC
; EmotionScript Three Address Code (TAC)
CLASS Student
METHOD Student.greet:
SPEAK "Hello"
RETURN yes
END_METHOD Student.greet
END_CLASS Student
ALLOC s1, Student
CALL_METHOD_IF_EXISTS s1.init, 0
$t1 = CALL_METHOD s1.greet, 0
DROP $t1
