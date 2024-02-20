{ Object type declarations: }

type
  Empty = object
  end;

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

  Shape = object
    constructor();
    procedure Print(); virtual;
    procedure Measure(graphics: Graphics); virtual;
    procedure Draw(graphics: Graphics); virtual;
    function Bounds(): Rect; virtual;
  end;

  List = object of T
    items:  array of T;
    count: integer;
    constructor();
    function IsEmpty(): boolean;
    procedure Add(item: T);
    function Get(index: integer): T;
    function IndexOf(item: T): integer;
    procedure Remove(index: integer);
  end;

  ShapeList = List of Shape;

  CompoundShape = object(Shape)
    components: ShapeList;
    constructor();
    procedure Print(); override;
    procedure Add(shape: Shape);
    procedure Measure(graphics: Graphics); override;
    procedure Draw(graphics: Graphics); override;
    function CombinedBounds(): Rect;
  end;
