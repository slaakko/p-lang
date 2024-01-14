unit Syntax;

interface

uses System.Graphics.Primitive, System.Graphics;

const
  roundingRadius: real = 3;
  gapWidth: real = 3;
  branchWidth: real = 6;

type
  SyntaxShape = object(CompoundShape)
    bounds: Rect;
    padding: Padding;
    constructor();
    constructor(padding: Padding);
    procedure Print(); override;
    procedure Measure(graphics: Graphics); override;
    function Bounds(): Rect; override;
    function Left(): Point; virtual;
    function Top(): Point; virtual;
    function Right(): Point; virtual;
    function Bottom(): Point; virtual;
    function Center(): Point; virtual;
    function ContentOrigin(): Point; virtual;
  end;

  DiagramTitle = object(SyntaxShape)
    title: string;
    titleSize: Size;
    constructor(title: string);
    procedure Print(); override;
    function ContentOrigin(): Point; override;
    procedure Measure(graphics: Graphics); override;
    procedure Draw(graphics: Graphics); override;
  end;

  RuleBox = object(SyntaxShape)
    ruleName: string;
    ruleSize: Size;
    constructor(ruleName: string);
    procedure Print(); override;
    function ContentOrigin(): Point; override;
    procedure Measure(graphics: Graphics); override;
    procedure Draw(graphics: Graphics); override;
  end;

  Terminal = object(SyntaxShape)
    terminal: string;
    terminalSize: Size;
    constructor(terminal: string);
    procedure Print(); override;
    function ContentOrigin(): Point; override;
    procedure Measure(graphics: Graphics); override;
    procedure Draw(graphics: Graphics); override;
  end;

  TerminalNoFrame = object(SyntaxShape)
    terminal: string;
    terminalSize: Size;
    constructor(terminal: string);
    procedure Print(); override;
    function ContentOrigin(): Point; override;
    procedure Measure(graphics: Graphics); override;
    procedure Draw(graphics: Graphics); override;
  end;

  Keyword = object(SyntaxShape)
    keyword: string;
    keywordSize: Size;
    constructor(keyword: string);
    procedure Print(); override;
    function ContentOrigin(): Point; override;
    procedure Measure(graphics: Graphics); override;
    procedure Draw(graphics: Graphics); override;
  end;

  Gap = object(SyntaxShape)
    constructor();
    procedure Print(); override;
  end;

  Branch = object(SyntaxShape)
    constructor();
    procedure Print(); override;
  end;

  Out = object(SyntaxShape)
    constructor();
    procedure Print(); override;
  end;

  RoutingPoint = object(SyntaxShape)
    constructor();
  end;

  SyntaxConnector = object(SyntaxShape)
    source, target: SyntaxShape;
    direction1, direction2: Direction;
    connector: Shape;
    constructor(source, target: SyntaxShape; direction1, direction2: Direction);
    procedure CreateConnectorShape(points: PointArray); virtual;
    procedure Measure(graphics: Graphics); override;
    procedure Draw(graphics: Graphics); override;
  end;

  SyntaxArrow = object(SyntaxConnector)
    constructor(source, target: SyntaxShape; direction1, direction2: Direction);
    procedure Print(); override;
    procedure CreateConnectorShape(points: PointArray); override;
  end;

  SyntaxLine = object(SyntaxConnector)
    constructor(source, target: SyntaxShape; direction1, direction2: Direction);
    procedure Print(); override;
    procedure CreateConnectorShape(points: PointArray); override;
  end;

  SyntaxCell = object(SyntaxShape)
    row, column: integer;
    constructor(row, column: integer);
    procedure Print(); override;
    procedure Measure(graphics: Graphics); override;
    procedure SetLocations();
  end;

  SyntaxRow = object(SyntaxShape)
    row: integer;
    function GetCell(column: integer): SyntaxCell;
    constructor(row: integer);
    procedure Print(); override;
    procedure MeasureRow(graphics: Graphics);
    procedure SetLocations();
  end;

  SyntaxGrid = object(SyntaxShape)
    constructor();
    function GetRow(row: integer): SyntaxRow;
    function GetCell(row, column: integer): SyntaxCell;
    procedure Print(); override;
    procedure Measure(graphics: Graphics); override;
    procedure AddShape(row, column: integer; shape: SyntaxShape);
    procedure SetLocations();
  end;

  Connectors = object(CompoundShape)
    constructor();
  end;

  Diagram = object(SyntaxShape)
    title: DiagramTitle;
    fileName: string;
    grid: SyntaxGrid;
    connectors: Connectors;
    constructor(title: DiagramTitle; fileName: string);
    procedure Print(); override;
    procedure Draw(graphics: Graphics); override;
    procedure SetLocations();
    procedure MeasureConnectors(graphics: Graphics);
    procedure Measure(graphics: Graphics); override;
    procedure AddShape(row, column: integer; shape: SyntaxShape);
    procedure AddConnector(connector: SyntaxShape);
    procedure DrawConnectors(graphics: Graphics);
  end;

var 
  font, boldFont: Font;
  noPadding: Padding;
  titlePadding: Padding;
  ruleBoxPadding: Padding;
  terminalPadding: Padding;
  keywordPadding: Padding;
  cellPadding: Padding;
  diagramPadding: Padding;

implementation

constructor SyntaxShape(padding: Padding);
begin
  bounds := new Rect();
  this.padding := padding;
end;

constructor SyntaxShape() : this(noPadding);
begin
end;

procedure SyntaxShape.Print(); 
begin
  base.Print();
  Write('bounds: ');
  bounds.Print();
  Write('padding: ');
  padding.Print();
end;

procedure SyntaxShape.Measure(graphics: Graphics);
begin
  base.Measure(graphics);
  bounds.size.w := bounds.size.w + padding.Horizontal();
  bounds.size.h := bounds.size.h + padding.Vertical();
end;

function SyntaxShape.Bounds() : Rect;
begin
  Bounds := bounds;
end;

function SyntaxShape.Left(): Point; 
begin
  Left := new Point(bounds.location.x, bounds.location.y + bounds.size.h / 2);
end;

function SyntaxShape.Top(): Point; 
begin
  Top := new Point(bounds.location.x + bounds.size.w / 2, bounds.location.y);
end;

function SyntaxShape.Right(): Point; 
begin
  Right := new Point(bounds.location.x + bounds.size.w, bounds.location.y + bounds.size.h / 2);
end;

function SyntaxShape.Bottom(): Point; 
begin
  Bottom := new Point(bounds.location.x + bounds.size.w / 2, bounds.location.y + bounds.size.h);
end;

function SyntaxShape.Center(): Point; 
begin
  Center := new Point(bounds.location.x + bounds.size.w / 2, bounds.location.y + bounds.size.h / 2);
end;

function SyntaxShape.ContentOrigin(): Point; 
var
  contentOrigin: Point;
begin
  contentOrigin := new Point(bounds.location.x, bounds.location.y);
  contentOrigin.Offset(padding.left, padding.top);
  ContentOrigin := contentOrigin;
end;

constructor DiagramTitle(title: string) : base(titlePadding);
begin
  this.title := title;
end;

procedure DiagramTitle.Print(); 
begin
  Writeln('diagram-title: ''', title, '''');
  base.Print();
end;

function DiagramTitle.ContentOrigin(): Point; 
var
  x, y: real;
begin
  x := bounds.location.x + padding.left;
  y := bounds.location.y + padding.top;
  ContentOrigin := new Point(x, y);
end;

procedure DiagramTitle.Measure(graphics: Graphics); 
begin
  base.Measure(graphics);
  titleSize := graphics.MeasureString(title, font);
  bounds.SetSize(new Size(bounds.size.w + titleSize.w, bounds.size.h + titleSize.h));
end;

procedure DiagramTitle.Draw(graphics: Graphics); 
begin
  base.Draw(graphics);
  graphics.DrawString(title, font, blackBrush, ContentOrigin());
end;

constructor RuleBox(ruleName: string) : base(ruleBoxPadding);
begin
  this.ruleName := ruleName;
end;

procedure RuleBox.Print(); 
begin
  Writeln('rule-box: ''', ruleName, '''');
  base.Print();
end;

function RuleBox.ContentOrigin(): Point;
var
  x, y: real;
begin
  x := bounds.location.x + padding.left;
  y := bounds.location.y + padding.top;
  ContentOrigin := new Point(x, y);
end;

procedure RuleBox.Measure(graphics: Graphics); 
begin
  base.Measure(graphics);
  ruleSize := graphics.MeasureString(ruleName, font);
  bounds.SetSize(new Size(bounds.size.w + ruleSize.w, bounds.size.h + ruleSize.h));
end;

procedure RuleBox.Draw(graphics: Graphics); 
begin
  base.Draw(graphics);
  graphics.DrawString(ruleName, font, blackBrush, ContentOrigin());
  graphics.DrawRectangle(blackPen, bounds);
end;

constructor Terminal(terminal: string) : base(terminalPadding);
begin
  this.terminal := terminal;
end;

procedure Terminal.Print(); 
begin
  Write('terminal ', terminal);
  base.Print();
end;

function Terminal.ContentOrigin(): Point; 
var
  x, y: real;
begin
  x := bounds.location.x + (bounds.size.w - terminalSize.w) / 2;
  y := bounds.location.y + (bounds.size.h - terminalSize.h) / 2;
  ContentOrigin := new Point(x, y);
end;

procedure Terminal.Measure(graphics: Graphics); 
var 
  max: real;
begin
  base.Measure(graphics);
  terminalSize := graphics.MeasureString(terminal, boldFont);
  max := Max(terminalSize.w, terminalSize.h);
  bounds.SetSize(new Size(bounds.size.w + max, bounds.size.h + max));
end;

procedure Terminal.Draw(graphics: Graphics); 
begin
  base.Draw(graphics);
  graphics.DrawString(terminal, boldFont, blackBrush, ContentOrigin());
  if terminal.Length = 1 then graphics.DrawEllipse(blackPen, bounds) else DrawRoundedRectangle(graphics, bounds, roundingRadius);
end;

constructor TerminalNoFrame(terminal: string) : base(terminalPadding);
begin
  this.terminal := terminal;
end;

procedure TerminalNoFrame.Print(); 
begin
  Write('terminal no frame', terminal);
  base.Print();
end;

function TerminalNoFrame.ContentOrigin(): Point; 
var
  x, y: real;
begin
  x := bounds.location.x + (bounds.size.w - terminalSize.w) / 2;
  y := bounds.location.y + (bounds.size.h - terminalSize.h) / 2;
  ContentOrigin := new Point(x, y);
end;

procedure TerminalNoFrame.Measure(graphics: Graphics); 
var 
  max: real;
begin
  base.Measure(graphics);
  terminalSize := graphics.MeasureString(terminal, boldFont);
  max := Max(terminalSize.w, terminalSize.h);
  bounds.SetSize(new Size(bounds.size.w + max, bounds.size.h + max));
end;

procedure TerminalNoFrame.Draw(graphics: Graphics); 
begin
  base.Draw(graphics);
  graphics.DrawString(terminal, font, blackBrush, ContentOrigin());
end;

constructor Keyword(keyword: string) : base(keywordPadding);
begin
  this.keyword := keyword;
end;

procedure Keyword.Print();
begin
  Writeln('keyword: ''', keyword, '''');
  base.Print();
end;

function Keyword.ContentOrigin(): Point; 
var
  x, y: real;
begin
  x := bounds.location.x + padding.left;
  y := bounds.location.y + padding.top;
  ContentOrigin := new Point(x, y);
end;

procedure Keyword.Measure(graphics: Graphics); 
begin
  base.Measure(graphics);
  keywordSize := graphics.MeasureString(keyword, boldFont);
  bounds.SetSize(new Size(bounds.size.w + keywordSize.w, bounds.size.h + keywordSize.h));
end;

procedure Keyword.Draw(graphics: Graphics); 
begin
  base.Draw(graphics);
  graphics.DrawString(keyword, boldFont, blackBrush, ContentOrigin());
  DrawRoundedRectangle(graphics, bounds, roundingRadius);
end;

constructor Gap();
begin
  bounds.size.w := gapWidth;
  bounds.size.h := 0;
end;

procedure Gap.Print();
begin
  Writeln('branch');
  base.Print();
end;

constructor Branch();
begin
  bounds.size.w := branchWidth;
  bounds.size.h := 0;
end;

procedure Branch.Print();
begin
  Writeln('branch');
  base.Print();
end;

constructor Out();
begin
  bounds.size.w := 0;
  bounds.size.h := 0;
end;

procedure Out.Print();
begin
  Writeln('out');
  base.Print();
end;

constructor RoutingPoint();
begin
  bounds.size.w := 0;
  bounds.size.h := 0;
end;

constructor SyntaxConnector(source, target: SyntaxShape; direction1, direction2: Direction);
begin
  this.source := source;
  this.target := target;
  this.direction1 := direction1;
  this.direction2 := direction2;
end;

procedure SyntaxConnector.CreateConnectorShape(points: PointArray); 
begin
end;

procedure SyntaxConnector.Measure(graphics: Graphics); 
var
  points: PointArray;
begin
  if direction2 = noDir then
  begin
    points := new Point[2];
    case direction1 of
      east: 
      begin
        points[0] := source.Right();
        points[1] := target.Left();
      end;
      west:
      begin
        points[0] := source.Left();
        points[1] := target.Right();
      end;
      north:
      begin
        points[0] := source.Top();
        points[1] := source.Bottom();
      end;
      south:
      begin
        points[0] := source.Bottom();
        points[1] := source.Top();
      end;
    end;
    CreateConnectorShape(points);
  end
  else
  begin
    points := new Point[3];
    case direction1 of
      south:
      begin
        points[0] := source.Bottom();
        points[1] := new Point(source.Bottom().x, target.Center().y);
      end;
      north:
      begin
        points[0] := source.Top();
        points[1] := new Point(source.Top().x, target.Center().y);
      end;
      east:
      begin
        points[0] := source.Right();
        points[1] := new Point(target.Bottom().x, source.Right().y);
      end;
      west:
      begin
        points[0] := source.Left();
        points[1] := new Point(target.Bottom().x, source.Left().y);
      end;
    end;
    case direction2 of
      east:
      begin
        points[2] := target.Left();
      end;
      west:
      begin
        points[2] := target.Right();
      end;
      south:
      begin
        points[2] := target.Top();
      end;
      north:
      begin
        points[2] := target.Bottom();
      end;
    end;
    CreateConnectorShape(points);
  end;
end;

procedure SyntaxConnector.Draw(graphics: Graphics); 
begin
  connector.Draw(graphics);
end;

constructor SyntaxArrow(source, target: SyntaxShape; direction1, direction2: Direction) : base(source, target, direction1, direction2);
begin
end;

procedure SyntaxArrow.CreateConnectorShape(points: PointArray);
begin
  connector := new Arrow(points);
end;

procedure SyntaxArrow.Print();
begin
  Writeln('syntax-arrow');
  base.Print();
end;

constructor SyntaxLine(source, target: SyntaxShape; direction1, direction2: Direction) : base(source, target, direction1, direction2);
begin
end;

procedure SyntaxLine.CreateConnectorShape(points: PointArray);
begin
  connector := new Line(points);
end;

procedure SyntaxLine.Print();
begin
  Writeln('syntax-line');
  base.Print();
end;

constructor SyntaxCell(row, column: integer) : base(cellPadding);
begin
  this.row := row;
  this.column := column;
end;

procedure SyntaxCell.Print(); 
begin
  Writeln('syntax-cell[', row, ', ', column, ']');
  base.Print();
end;

procedure SyntaxCell.Measure(graphics: Graphics); 
var
  i: integer;
  s: SyntaxShape;
  w, h: real;
begin
  base.Measure(graphics);
  w := 0;
  h := 0;
  for i := 0 to count - 1 do
  begin
    s := SyntaxShape(components[i]);
    w := w + s.bounds.size.w;
    h := Max(h, s.bounds.size.h);
  end;
  bounds.size := new Size(bounds.size.w + w, bounds.size.h + h);
end;

procedure SyntaxCell.SetLocations();
var
  loc: Point;
  i: integer;
  s: SyntaxShape;
  y: real;
begin
  loc := new Point(padding.left + bounds.location.x, padding.top + bounds.location.y);
  for i := 0 to count - 1 do
  begin
    s := SyntaxShape(components[i]);
    y := bounds.location.y + (bounds.size.h - s.bounds.size.h) / 2;
    s.bounds.location := new Point(loc.x, y);
    loc.Offset(s.bounds.size.w, 0);
  end;
end;

constructor SyntaxRow(row: integer) : base(noPadding);
begin
  this.row := row;
end;

function SyntaxRow.GetCell(column: integer): SyntaxCell;
var
  cell: SyntaxCell;
begin
  while column >= count do
  begin
    Add(new SyntaxCell(row, count));
  end;
  cell := SyntaxCell(components[column]);
  GetCell := cell;
end;

procedure SyntaxRow.Print(); 
begin
  Writeln('syntax-row: ', row);
  base.Print();
end;

procedure SyntaxRow.MeasureRow(graphics: Graphics); 
var
  i: integer;
  w, h: real;
  cell: SyntaxCell;
begin
  w := 0;
  h := 0;
  for i := 0 to count - 1 do
  begin
    cell := SyntaxCell(components[i]);
    w := w + cell.bounds.size.w;
    h := Max(h, cell.bounds.size.h);
  end;
  bounds.size := new Size(w, h);
end;

procedure SyntaxRow.SetLocations();
var
  loc: Point;
  i: integer;
  y: real;
  cell: SyntaxCell;
begin
  loc := new Point(bounds.location.x, bounds.location.y);
  for i := 0 to count - 1 do
  begin
    cell := SyntaxCell(components[i]);
    y := loc.y + (bounds.size.h - cell.bounds.size.h) / 2;
    cell.bounds.location := new Point(loc.x, y);
    loc.Offset(cell.bounds.size.w, 0);
    cell.SetLocations();
  end;
end;

constructor SyntaxGrid() : base(noPadding);
begin
end;

procedure SyntaxGrid.Print(); 
var
  i: integer;
  syntaxRow: SyntaxRow;
begin
  for i := 0 to count - 1 do
  begin
    syntaxRow := SyntaxRow(components[i]);
    syntaxRow.Print();
  end;
end;

function SyntaxGrid.GetRow(row: integer): SyntaxRow;
var
  syntaxRow: SyntaxRow;
begin
  while row >= count do
  begin
    Add(new SyntaxRow(count));
  end;
  syntaxRow := SyntaxRow(components[row]);
  GetRow := syntaxRow;
end;

function SyntaxGrid.GetCell(row, column: integer): SyntaxCell;
var
  syntaxRow: SyntaxRow;
  syntaxCell: SyntaxCell;
begin
  syntaxRow := GetRow(row);
  syntaxCell := syntaxRow.GetCell(column);
  GetCell := syntaxCell;
end;

procedure SyntaxGrid.Measure(graphics: Graphics); 
var
  c, i, n: integer;
  w, h: real;
  r: SyntaxRow;
  cell: SyntaxCell;
begin
  base.Measure(graphics);
  n := 0;
  for i := 0 to count - 1 do
  begin
    r := SyntaxRow(components[i]);
    n := Max(n, r.count);
  end;
  for c := 0 to n - 1 do
  begin
    w := 0;
    h := 0;
    for i := 0 to count - 1 do
    begin
      cell := GetCell(i, c);
      w := Max(w, cell.bounds.size.w);
      h := Max(h, cell.bounds.size.h);
    end;
    for i := 0 to count - 1 do
    begin
      cell := GetCell(i, c);
      cell.bounds.size.w := w;
      cell.bounds.size.h := h;
    end;
  end;
  for i := 0 to count - 1 do
  begin
    r := SyntaxRow(components[i]);
    r.MeasureRow(graphics);
  end;
end;

procedure SyntaxGrid.AddShape(row, column: integer; shape: SyntaxShape);
var
  syntaxCell: SyntaxCell;
begin
  if shape = nil then
  begin
    Writeln('trying to add nil shape');
  end else
  begin
    syntaxCell := GetCell(row, column);
    syntaxCell.Add(shape);
  end;
end;

procedure SyntaxGrid.SetLocations();
var
  i: integer;
  row: SyntaxRow;
  loc: Point;
begin
  loc := new Point(bounds.location.x, bounds.location.y);
  for i := 0 to count - 1 do
  begin
    row := SyntaxRow(components[i]);
    row.bounds.location := new Point(loc.x, loc.y);
    row.SetLocations();
    loc.Offset(0, row.bounds.size.h);
  end;
end;

constructor Connectors();
begin
end;

constructor Diagram(title: DiagramTitle; fileName: string) : base(diagramPadding);
begin
  this.title := title;
  Add(title);
  this.fileName := fileName;
  grid := new SyntaxGrid();
  Add(grid);
  connectors := new Connectors();
end;

procedure Diagram.Print(); 
begin
  Writeln('Diagram:');
  base.Print();
end;

procedure Diagram.SetLocations();
var
  firstRow: SyntaxRow;
  h: real;
  loc: Point;
begin
  firstRow := SyntaxRow(grid.components[0]);
  h := (firstRow.bounds.size.h - title.bounds.size.h) / 2;
  loc := new Point(padding.left, padding.top);
  title.bounds.location := new Point(loc.x, loc.y + h);
  loc.Offset(title.bounds.size.w, 0);
  grid.bounds.location := new Point(loc.x, loc.y);
  grid.SetLocations();
end;

procedure Diagram.MeasureConnectors(graphics: Graphics);
var
  i: integer;
  connector: SyntaxShape;
begin
  for i := 0 to connectors.count - 1 do
  begin
    connector := SyntaxShape(connectors.components[i]);
    connector.Measure(graphics);
  end
end;

procedure Diagram.Measure(graphics: Graphics); 
var
  combinedBounds: Rect;
  i: integer;
begin
  base.Measure(graphics);
  SetLocations();
  grid.bounds := grid.CombinedBounds();
  combinedBounds := CombinedBounds();
  bounds.size.w := bounds.size.w + combinedBounds.size.w;
  bounds.size.h := bounds.size.h + combinedBounds.size.h;
  MeasureConnectors(graphics);
end;

procedure Diagram.Draw(graphics: Graphics); 
begin
  graphics.Clear(white);
  base.Draw(graphics);
  DrawConnectors(graphics);
end;

procedure Diagram.AddShape(row, column: integer; shape: SyntaxShape);
begin
  grid.AddShape(row, column, shape);
end;

procedure Diagram.AddConnector(connector: SyntaxShape);
begin
  connectors.Add(connector);
end;

procedure Diagram.DrawConnectors(graphics: Graphics);
var
  i: integer;
  connector: SyntaxShape;
begin
  for i := 0 to connectors.count - 1 do
  begin
    connector := SyntaxShape(connectors.components[i]);
    connector.Draw(graphics);
  end;
end;

begin
  font := new Font('Arial', 11, regularFontStyle);
  boldFont := new Font('Arial', 11, boldFontStyle);
  noPadding := new Padding(0, 0, 0, 0);
  titlePadding := noPadding;
  ruleBoxPadding := new Padding(0.5, 0.5, 0.5, 0.5);
  terminalPadding := new Padding(1, 1, 1, 1);
  keywordPadding := new Padding(1, 1, 1, 1);
  cellPadding := new Padding(2, 2, 2, 2);
  diagramPadding := new Padding(5, 5, 5, 5);
end.
