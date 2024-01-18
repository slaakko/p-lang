program virtual_shape;
type
  Shape = object
    procedure Print(); virtual;
  end;

  Circle = object(Shape)
    procedure Print(); override;
  end;

var
  s, c: Shape;

procedure Shape.Print();
begin
  Writeln('Shape.Print()');
end;

procedure Circle.Print();
begin
  base.Print();
  Writeln('Circle.Print()');
end;

begin
  s := new Shape();
  s.Print();
  c := new Circle();
  c.Print();
end.

{
output of the program:
Shape.Print()
Shape.Print()
Circle.Print()
}
