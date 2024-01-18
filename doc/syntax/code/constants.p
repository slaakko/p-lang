type
  IntArray = array of integer;
  Foo = object
    x, y: integer;
  end;

{ Constant declarations: }

const
  MB: integer = 1024 * 1024;  { simple constant }
  pi_c: real = 3.1415926;     { simple constant }
  arr: IntArray = [1, 2];     { array constant }
  foo: Foo = (x: 1; y: 2);    { object constant }
