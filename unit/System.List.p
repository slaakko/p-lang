unit System.List;

interface

type
  List = object of T
    items:  array of T;
    count: integer;
    constructor();
    function IsEmpty(): boolean;
    procedure Add(item: T);
    function Get(index: integer): T;
    function IndexOf(item: T): integer;
    procedure Remove(index: integer);
  end;

implementation

constructor List();
begin
  items := new T[0];
  count := 0;
end;

function List.IsEmpty(): boolean;
begin
  IsEmpty := count = 0;
end;

procedure List.Add(item: T);
var
  length, newLength, i: integer;
  newItems: array of T;
begin
  length := items.Length();
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

function List.IndexOf(item: T): integer;
var
  i, index: integer;
  found: boolean;
begin
  index := -1;
  found := false;
  i := 0;
  while (i < count) and (not found) do 
  begin
    if items[i] = item then begin index := i; found := true; end else i := Succ(i);
  end;
  IndexOf := index;
end;

procedure List.Remove(index: integer);
var
  i: integer;
begin
  if (index >= 0) and (index < count) then 
  begin
    for i := index to count - 2 do items[i] := items[i + 1];
    count := Pred(count);
  end 
  else Panic('invalid List index');
end;

end.
