{ Block: }

const
  N: integer = 10;

type
  Vector = array of real;

  Point = object
    x, y: integer;
    constructor(x, y: integer);
  end;

var
  vec: Vector;
  point: Point;
  r: integer;

  constructor Point(x, y: integer);
  begin
    this.x := x;
    this.y := y;
  end;

  procedure Proc(v: Vector);
  const
    M: integer = 20;
  type  
    Object = object
      i: integer;
    end;
  var
    z: integer;
  begin
    { ... }
  end;

  function Func(p: Point): integer;
  const 
    F: integer = 30;
  type
    T = object
      { ... }
    end;
  var 
    u: integer;
  begin
    { ... }
    Func := 1;
  end;

begin
  vec := new real[2];
  vec[0] := 1.5;
  vec[1] := 2;
  Proc(vec);
  point := new Point(5, 6);
  r := Func(point);
end;
