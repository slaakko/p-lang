unit System.List;

interface

type
  List = object of T
    items: array of T;
    count: integer;
    constructor();
    procedure Add(T item);
    function Get(index: integer): T;
  end;

implementation

constructor List();
begin
  items := new T[0];
  count := 0;
end;

procedure List.Add(T item);
var
  length, newLength, i: integer;
  newItems: array of T;
begin
  length := items.Length;
  if count = length then 
  begin
    if length < 4 then newLength := 4 else newLength := 2 * length;
    newItems := new T[newLength];
    for i := 0 to count - 1 do newItems[i] := items[i];
    items := newItems;
  end;
  items[count] := item;
  count := Succ(count);
end;

function List.Get(index: integer): T;
begin
  if (index >= 0) and (index < count) then Get := items[index] else Panic('invalid List index');
end;

end.
