;; EmotionScript TAC
; EmotionScript Three Address Code (TAC)
CLASS Human
METHOD Human.speak:
FORMAL msg
$t1 = "Human says: " + msg
SPEAK $t1
RETURN yes
END_METHOD Human.speak
END_CLASS Human
CLASS Student EXTENDS Human
ATTR hidden count id
ATTR hidden words name
END_CLASS Student
CLASS Teacher EXTENDS Human
ATTR open count teacherId
ATTR hidden words subject
METHOD Teacher.speak:
FORMAL msg
$t2 = "Teacher says: " + msg
SPEAK $t2
RETURN yes
END_METHOD Teacher.speak
END_CLASS Teacher
ALLOC student1, Student
CALL_METHOD_IF_EXISTS student1.init, 0
ALLOC teacher1, Teacher
CALL_METHOD_IF_EXISTS teacher1.init, 0
SET_FIELD student1.id, 101
SET_FIELD student1.name, "Alice"
SET_FIELD teacher1.teacherId, 501
SET_FIELD teacher1.subject, "Math"
PARAM "Hello from student!"
$t3 = CALL_METHOD student1.speak, 1
DROP $t3
PARAM "Hello from teacher!"
$t4 = CALL_METHOD teacher1.speak, 1
DROP $t4
