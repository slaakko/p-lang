type
  A = array of integer;

  Foo = object 
    f: integer;
    constructor(f: integer);
    procedure m();
  end;

var
  a: A;
  x: Foo;

function func(x: integer; foo: Foo): integer;
begin
  { ... }
  func := x;
end;


{ Factors: }

0                   { primary expression is a factor }
a[4]                { array index operation }
x.f                 { member selection operation }  
this.f              { this-field access (inside Foo) }
this.m()            { this-method call (inside Foo) }
func(1, new Foo(1)) { function call with unsigned constant and }
                    { new-expression arguments }
