{ Function: }

function MMToPixels(mm: real; dpi: real): integer;
var
  pixels: integer;
begin
  pixels := integer(mm * dpi / inchMM);
  MMToPixels := pixels;
end;
