;; EmotionScript TAC
; EmotionScript Three Address Code (TAC)
CLASS A
ATTR hidden count a
END_CLASS A
CLASS B EXTENDS A
ATTR hidden count b
END_CLASS B
CLASS C EXTENDS B
ATTR hidden count c
END_CLASS C
ALLOC c1, C
CALL_METHOD_IF_EXISTS c1.init, 0
SET_FIELD c1.a, 1
SET_FIELD c1.b, 2
SET_FIELD c1.c, 3
$t1 = GET_FIELD c1.a
SPEAK $t1
$t2 = GET_FIELD c1.b
SPEAK $t2
$t3 = GET_FIELD c1.c
SPEAK $t3
