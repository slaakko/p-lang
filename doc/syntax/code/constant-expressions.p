{ Constant expressions: }

const 
  x: integer = -3 - 1;    { -4 }
  y: integer = Succ(x);   { -3 }
  z: integer = Pred(x);   { -5 }
  a: char = 'a';
  ordA: integer = Ord(a); { 97 }
  chrA: char = Chr(ordA); { 'a' }
  b: char = Succ(a);      { 'b' }
  predB: char = Pred(b);  { 'a' }
  absX: integer = Abs(x); { 4 }
  t: boolean = true;
  ordT: integer = Ord(t); { 1 }
  c: boolean = Pred(t);   { false }
  ordC: integer = Ord(c); { 0 }
