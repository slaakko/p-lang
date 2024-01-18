type
  IntArray = array of integer;

var 
  a: IntArray;
  c: boolean;

const
  n: integer = 7;
  b: boolean = true;

{ Terms: }

0           { primary expression is a term }
a[4]        { factor is a term }
2 * n       { integer multiplication (14) }
n / 2       { real division operator produces real result (3.5) }
n div 2     { integer division operator produces integer result (3) }
n mod 3     { integer modulo operation (1) }
n and 2     { integer bitwise AND operation (2) }
b and c     { Boolean logical AND operation }
n shl 1     { integer bitwise shift left operation (14) }
n shr 1     { integer bitwise shift right operation (3) }
