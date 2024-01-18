program Constructors;

type
  Color = object
    a, r, g, b: Byte;
    constructor(a, r, g, b: Byte);
  end;

  Shape = object
    color: Color;
    constructor(color: Color);
  end;

  Circle = object(Shape)
    constructor(color: Color);
  end;

var
  red: Color;
  circle: Circle;

{  Constructors: }

constructor Color(a, r, g, b: Byte);
begin
  this.a := a;
  this.r := r;
  this.g := g;
  this.b := b;
end;

constructor Shape(color: Color);
begin
  this.color := color;
end;

constructor Circle(color: Color) : base(color);
begin
end;

begin
  red := new Color(Byte(255), Byte(255), Byte(0), Byte(0));
  circle := new Circle(red);
end.
