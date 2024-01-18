program Scopes;
var 
  v: integer;

  procedure Outer();
  var
    outerVar: integer;

    procedure Inner();
    var
      innerVar: integer;
    begin
      Writeln('outer var: ', outerVar);
      innerVar := 10;
      outerVar := 2;
    end;

  begin
    outerVar := 1;
    Inner();
    Writeln('outer var is now: ', outerVar);
    v := 3;
  end;

begin
  v := 0;
  Outer();
  Writeln('v is now: ', v);
end.

{   
    output of the program is:
    outer var: 1
    outer var is now: 2
    v is now: 3
}
