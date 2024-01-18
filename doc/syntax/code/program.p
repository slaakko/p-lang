{ Program: }

program SyntaxDiagrams;

uses System.Graphics.Primitive, System.Graphics, Syntax;
 
type
  DiagramArray = array of Diagram;

var
  diagrams: DiagramArray;
  diagramCount: integer;
  x, y: integer;
  initialBitmap: Bitmap;
  initialGraphics: Graphics;

procedure AddDiagram(diagram: Diagram);
var
  newLength, i: integer;
  newDiagrams: DiagramArray;
begin
  if diagramCount = diagrams.Length then
  begin
    if diagrams.Length < 4 then newLength := 4 
    else newLength := 2 * diagrams.Length;
    newDiagrams := new Diagram[newLength];
    for i := 0 to diagramCount - 1 do newDiagrams[i] := diagrams[i];
    diagrams := newDiagrams;
  end;
  diagrams[diagramCount] := diagram;
  diagramCount := Succ(diagramCount);
end;

procedure MakeDiagrams();
begin
  { ... }
end;

procedure MeasureDiagrams();
begin
  { ... }
end;

procedure PrintDiagrams();
begin
  { ... }
end;

procedure DrawDiagrams();
begin
  { ... }
end;

begin
  x := MMToPixels(100, dpiX);
  y := MMToPixels(100, dpiY);
  initialBitmap := new Bitmap(x, y);
  initialGraphics := initialBitmap.GetGraphics();
  diagrams := new Diagram[0];
  diagramCount := 0;
  MakeDiagrams();
  MeasureDiagrams();
  PrintDiagrams();
  DrawDiagrams();
end.  
