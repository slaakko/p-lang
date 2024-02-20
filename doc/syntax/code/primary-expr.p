type
  Foo = object
    field: integer;
    procedure method();
  end;

  Vector = array of real;

var
  x, y: integer;
  done: boolean;
  count: integer;

{ Primary expressions: }

x                 { identifier }
real(x)           { typecast }
0                 { unsigned constant }
'string'          { unsigned constant }
true              { unsigned constant }
nil               { unsigned constant }
(y + 1)           { parenthesized expression }
not done          { logical NOT expression }
not x             { bitwise complement expression }
-1                { negative number }
integer(1234.5)   { typecast }
new Foo           { new-expression }
new Vector[count] { new-expression }
this              { reference to this-object (inside a method) }
base              { reference to base-object (inside a method) }
