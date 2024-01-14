program RealToString;
var
  s: string;
  t: string;
begin
  s := RealToString(1234);
  Writeln(s);
  t := RealToString(123.4);
  Writeln(t);
end.
