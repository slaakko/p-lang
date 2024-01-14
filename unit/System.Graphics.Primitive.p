unit System.Graphics.Primitive;

interface

type
  Point = object
    x, y: real;
    constructor();
    constructor(x, y: real);
    procedure Print();
    procedure Offset(dx, dy: real);
  end;

  Size = object
    w, h: real;
    constructor();
    constructor(w, h: real);
    procedure Print();
  end;

  Rect = object
    location: Point;
    size: Size;
    constructor();
    constructor(location: Point; size: Size);
    procedure Print();
    procedure SetLocation(location: Point);
    procedure SetSize(size: Size);
    function Left(): real;
    function Top(): real;
    function Right(): real;
    function Bottom(): real;
    function IsEmpty(): boolean;
    function Contains(p: Point): boolean;
    function IntersectsWith(r: Rect): boolean;
    procedure Offset(dx, dy: real);
    procedure Inflate(dx, dy: real);
  end;

  Vector = object
    x, y: real;
    constructor();
    constructor(x, y: real);
    constructor(p: Point);
    procedure Print();
    function Length(): real;
    function ToPoint(): Point;
  end;

type
  Direction = (noDir, north, east, south, west);

function Distance(s, e: Point): real;

function Union(a, b: Rect): Rect;

function Sum(u, v: Vector): Vector;

function Difference(u, v: Vector): Vector;

function Product(a: real; v: Vector): Vector;

function Dot(u, v: Vector): real;

function Unit(v: Vector): Vector;

function ProjectionFactor(u, v: Vector): real;

function Proj(u, v: Vector): Vector;

function Rotate(v: Vector; angleRad: real): Vector;

function GetDirection(source, target: Point): Direction;

type
  LineSegment = object
    s, e: Point;
    constructor(s, e: Point);
    constructor(s: Point; v: Vector);
    procedure Print();
    function ToVector(): Vector;
    function Length(): real;
  end;

function RotateLineSegment(line: LineSegment; angleDeg: real): LineSegment;

implementation

constructor Point();
begin
  x := 0;
  y := 0;
end;

constructor Point(x, y: real);
begin
  this.x := x;
  this.y := y;
end;

procedure Point.Print();
begin
  Write('(');
  Write(x);
  Write(', ');
  Write(y);
  Write(')');
  Writeln();
end;

procedure Point.Offset(dx, dy: real);
begin
  x := x + dx;
  y := y + dy;
end;

constructor Size();
begin
  w := 0;
  h := 0;
end;

constructor Size(w, h: real);
begin
  this.w := w;
  this.h := h;
end;

procedure Size.Print();
begin
  Write('(');
  Write(w);
  Write(', ');
  Write(h);
  Write(')');
  Writeln();
end;

constructor Rect();
begin
  location := new Point();
  size := new Size();
end;

constructor Rect(location: Point; size: Size);
begin
  this.location := location;
  this.size := size;
end;

procedure Rect.Print();
begin
  Write('[');
  Write(location.x);
  Write(', ');
  Write(location.y);
  Write(', ');
  Write(size.w);
  Write(', ');
  Write(size.h);
  Write(']');
  Writeln();
end;

procedure Rect.SetLocation(location: Point);
begin
  this.location := location;
end;

procedure Rect.SetSize(size: Size);
begin
  this.size := size;
end;

function Rect.Left(): real;
begin
  Left := location.x;
end;

function Rect.Top(): real;
begin
  Top := location.y;
end;

function Rect.Right(): real;
begin
  Right := location.x + size.w;
end;

function Rect.Bottom() : real;
begin
  Bottom := location.y + size.h;
end;

function Rect.IsEmpty(): boolean;
begin
  IsEmpty := (size.w = 0) and (size.h = 0);
end;

function Rect.Contains(p: Point): boolean;
begin
  Contains := (p.x >= Left()) and (p.x < Right()) and (p.y >= Top()) and (p.y < Bottom());
end;

function Rect.IntersectsWith(r: Rect): boolean;
begin
   IntersectsWith := (Left() < r.Right()) and (Top() < r.Bottom()) and (Right() > r.Left()) and (Bottom() > r.Top());
end;

procedure Rect.Offset(dx, dy: real);
begin
  location.Offset(dx, dy);
end;

procedure Rect.Inflate(dx, dy: real);
begin
  location.Offset(-dx, -dy);
  size.w = size.w + 2 * dx;
  size.h = size.h + 2 * dy;
end;

constructor Vector();
begin
  x := 0;
  y := 0;
end;
  
constructor Vector(x, y: real);
begin
  this.x := x;
  this.y := y;
end;

constructor Vector(p: Point);
begin
  x := p.x;
  y := p.y;
end;

procedure Vector.Print();
begin
  Write('(');
  Write(x);
  Write(', ');
  Write(y);
  Write(')');
  Writeln();
end;

function Vector.Length(): real;
begin
  Length := Sqrt(x * x + y * y);
end;

function Vector.ToPoint(): Point;
begin
  ToPoint := new Point(x, y);
end;

function Distance(s, e: Point): real;
var
  dx, dy: real;
begin
  dx := Abs(s.x - e.x);
  dy := Abs(s.y - e.y);
  Distance := Sqrt(dx * dx + dy * dy);
end;

function Union(a, b: Rect): Rect;
var
  right, bottom, left, top: real;
begin
  right := Max(a.Right(), b.Right());
  bottom := Max(a.Bottom(), b.Bottom());
  left := Min(a.Left(), b.Left());
  top := Min(a.Top(), b.Top());
  Union := new Rect(new Point(left, top), new Size(right - left, bottom - top));
end;

function Sum(u, v: Vector): Vector;
begin
  Sum := new Vector(u.x + v.x, u.y + v.y);
end;

function Difference(u, v: Vector): Vector;
begin
  Difference := new Vector(u.x - v.x, u.y - v.y);
end;

function Product(a: real; v: Vector): Vector;
begin
  Product := new Vector(a * v.x, a * v.y);
end;

function Dot(u, v: Vector): real;
begin
  Dot := u.x * v.x + u.y * v.y;
end;

function Unit(v: Vector): Vector;
var
  length: real;
begin
  length := v.Length();
  Unit := Product(1 / length, v);
end;

function ProjectionFactor(u, v: Vector): real;
begin
   ProjectionFactor := Dot(v, Product(1 / u.Length(), u));
end;

function Proj(u, v: Vector): Vector;
begin
  Proj := Product(ProjectionFactor(v, u), Unit(u));
end;

function Rotate(v: Vector; angleRad: real): Vector;
var
  cosTheta, sinTheta: real;
begin
  cosTheta := Cos(angleRad);
  sinTheta := Sin(angleRad);
  Rotate := new Vector(v.x * cosTheta - v.y * sinTheta, v.x * sinTheta + v.y * cosTheta);
end;

function GetDirection(source, target: Point): Direction;
var
  v, u: Vector;
  direction: Direction;
begin
  v := new Vector(target.x - source.x, target.y - source.y);
  if v.x >= 0 then 
  begin
    if v.y >= 0 then 
    begin 
      u := new Vector(1, -1);
      if Dot(v, u) >= 0 then direction := east else direction := south;
    end else
    begin
      u := new Vector(-1, -1);
      if Dot(v, u) >= 0 then direction := north else direction := east;
    end
  end else
  begin
    if v.y >= 0 then 
    begin
      u := new Vector(1, 1);
      if Dot(v, u) >= 0 then direction := south else direction := west;
    end else
    begin
      u := new Vector(-1, 1);
      if Dot(v, u) >= 0 then direction := west else direction := north;
    end
  end;
end;

constructor LineSegment(s, e: Point);
begin
  this.s := s;
  this.e := e;
end;

constructor LineSegment(s: Point; v: Vector);
var
  u: Vector;
begin
  this.s := s;
  u := Sum(new Vector(s), v);
  this.e := u.ToPoint();
end;

procedure LineSegment.Print();
begin
  Write('{(');
  Write(s.x);
  Write(', ');
  Write(s.y);
  Write('); (');
  Write(e.x);
  Write(', ');
  Write(e.y);
  Write(')}');
  Writeln();
end;

function LineSegment.ToVector(): Vector;
begin
  ToVector := new Vector(e.x - s.x, e.y - s.y);
end;

function LineSegment.Length(): real;
begin
  Length := Distance(s, e);
end;

function RotateLineSegment(line: LineSegment; angleDeg: real): LineSegment;
var
  angleRad: real;
  v, r: Vector;
begin
  angleRad := pi / 180.0 * angleDeg;
  v := line.ToVector();  
  r := Rotate(v, angleRad);
  RotateLineSegment := new LineSegment(line.s, r);
end;

end.
