;; EmotionScript TAC
; EmotionScript Three Address Code (TAC)
CLASS Student
METHOD Student.greet:
FORMAL msg
$t1 = "Hi " + msg
SPEAK $t1
RETURN yes
END_METHOD Student.greet
END_CLASS Student
