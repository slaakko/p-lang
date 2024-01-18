type
  Foo = object
    foo_field: integer;
    procedure foo_method();
  end;

  Vector = array of real;

var
  x, y: integer;
  done: boolean;
  count: integer;

const
  MB: integer = 1024 * 1024;

function foo(): integer;

{ Primary expressions: }

x                 { variable reference }
real(x)           { variable typecast }
0                 { unsigned constant }
'string'          { unsigned constant }
MB                { unsigned constant }
true              { unsigned constant }
nil               { unsigned constant }
(y + 1)           { parenthesized expression }
not done          { logical NOT expression }
not x             { bitwise complement expression }
-1                { negative number }
foo               { function identifier }
integer(1234.5)   { value typecast }
new Foo           { new-expression }
new Vector[count] { new-expression }
this              { reference to this-object (inside a method) }
base              { reference to base-object (inside a method) }
foo_field         { field identifier (inside Foo-object) }
foo_method        { method identifier (inside Foo-object) }
