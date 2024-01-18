const 
  n: integer = 4;
  m: integer = 5;
  r: real = 1.5;
  q: real = 2.5;
  b: boolean = true;
  d: boolean = false;
  c: char = 'a';
  x: char = 'b';
  s: string = 'abc';
  t: string = 'abd';

{ Relational expressions: }

n = m { false }
r = q { false }
b <> d { true }
c <> x { true }
s <> t { true }
n > m { false }
s < t { true }
r >= q { false }
n <= m { true }
t <= s { false }
