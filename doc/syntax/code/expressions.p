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

  Color = object
    a, r, g, b: Byte;
    constructor(a, r, g, b: Byte);
  end;

const
  inchMM: real = 25.4;

var
  dpi: real;
  mm: real;
  pixels: integer;
  minX, minY, maxX, maxY: integer;

{ Expressions: }

integer(mm * dpi / inchMM) 
pixels * inchMM / dpi
new Rect(new Point(minX, minY), new Size(maxX - minX, maxY - minY))
new Color(Byte(255), Byte(0), Byte(0), Byte(0))
