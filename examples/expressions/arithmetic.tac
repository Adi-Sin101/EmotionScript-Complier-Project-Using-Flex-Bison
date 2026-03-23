;; EmotionScript TAC
; EmotionScript Three Address Code (TAC)
DECL count a
a = 8
DECL count b
b = 2
DECL measure result
$t1 = a + b
result = $t1
$t2 = a - b
result = $t2
$t3 = a * b
result = $t3
$t4 = a / b
result = $t4
SPEAK result
