program Matrix;
type
  Vector = array of real;
  Matrix = array of Vector;
var
  identityMatrix: Matrix;

procedure Print(matrix: Matrix);
var
  n, m, i, j: integer;
  v: Vector;
  r: real;
begin
  n := matrix.Length;
  for i := 0 to n - 1 do
  begin
    v := matrix[i];
    m := v.Length;
    for j := 0 to m - 1 do
    begin
      r := v[j];
      Writeln('(', i, ', ', j, ') = ', r);
    end;
  end;
end;

begin
  identityMatrix := new Vector[3];
  identityMatrix[0] := new real[3];
  identityMatrix[1] := new real[3];
  identityMatrix[2] := new real[3];
  identityMatrix[0][0] := 1.0;
  identityMatrix[0][1] := 0.0;
  identityMatrix[0][2] := 0.0;
  identityMatrix[1][0] := 0.0;
  identityMatrix[1][1] := 1.0;
  identityMatrix[1][2] := 0.0;
  identityMatrix[2][0] := 0.0;
  identityMatrix[2][1] := 0.0;
  identityMatrix[2][2] := 1.0;
  Print(identityMatrix);
end.
