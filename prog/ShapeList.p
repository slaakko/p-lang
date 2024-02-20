program ShapeList;
uses
  System.List;
type
  Shape = object
    name: string;
    constructor(name: string);
  end;
  ShapeList = List of Shape;
var
  s1, s2, s3, s4: Shape;
  shapeList: ShapeList;
  i: integer;

constructor Shape(name: string);
begin
  this.name := name;
end;

begin
  shapeList := new ShapeList();
  s1 := new Shape('square');
  s2 := new Shape('circle');
  s3 := new Shape('triangle');
  s4 := new Shape('ellipse');
  shapeList.Add(s1);
  shapeList.Add(s2);
  shapeList.Add(s3);
  for i := 0 to shapeList.count - 1 do Writeln(shapeList.Get(i).name);
  Writeln(shapeList.IndexOf(s4));
  Writeln(shapeList.IndexOf(s2));
end.
