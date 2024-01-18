{ For statements: }

for i := 0 to 10 do Writeln(i);

for i := 0 to count - 1 do
begin
  s := SyntaxShape(components[i]);
  w := w + s.bounds.size.w;
  h := Max(h, s.bounds.size.h);
end;
