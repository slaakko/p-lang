{ Program: }

program SyntaxDiagrams;

uses 
  System.Graphics.Primitive, 
  System.Graphics, 
  Syntax,
  System.List;
 
type
  DiagramList = List of Diagram;

var
  diagrams: DiagramList;
  x, y: integer;
  initialBitmap: Bitmap;
  initialGraphics: Graphics;

procedure AddDiagram(diagram: Diagram);
begin
  diagrams.Add(diagram);
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
  diagrams := new DiagramList();
  MakeDiagrams();
  MeasureDiagrams();
  PrintDiagrams();
  DrawDiagrams();
end.  
