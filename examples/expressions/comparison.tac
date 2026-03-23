;; EmotionScript TAC
; EmotionScript Three Address Code (TAC)
DECL count a
a = 5
DECL count b
b = 3
DECL truth gt
DECL truth lt
DECL truth eq
$t1 = a > b
gt = $t1
$t2 = a < b
lt = $t2
$t3 = a == b
eq = $t3
SPEAK gt
SPEAK lt
SPEAK eq
