program VarParam;
var
  v: integer;

procedure P(var x: integer);
begin
  Writeln('value of x is ', x);
  x := 100;
end;

begin
  v := 1;
  P(v);
  Writeln('value of v is ', v);
end.

{   
    output of the program is:
    value of x is 1
    value of v is 100
}
