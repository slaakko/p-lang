program IntList;
uses
  System.List;
type
  IntList = List of integer;
var
  list: IntList;
  i: integer;
begin
  list := new IntList();
  list.Add(1);
  list.Add(2);
  list.Add(3);
  for i := 0 to list.count - 1 do Writeln(list.Get(i));
  Writeln(list.IndexOf(0));
  Writeln(list.IndexOf(2));
end.
