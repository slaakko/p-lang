{ Forward declaration: }

program Forward;

procedure Second(x: integer); forward;

procedure First(x: integer);
var
  i: integer;
begin
  i := x - 1;
  Writeln('first: ', i);
  if i > 0 then 
  begin
    Second(i);
  end;
end;

procedure Second(x: integer);
begin
  Writeln('second: ', x);
  First(x);
end;

begin
  First(3);
end.

{ 
output of the program:
first: 2
second: 2
first: 1
second: 1
first: 0
}
