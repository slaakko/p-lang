{ Case statements: }

case direction1 of
  east: 
  begin
    points[0] := source.Right();
    points[1] := target.Left();
  end;
  west:
  begin
    points[0] := source.Left();
    points[1] := target.Right();
  end;
  north:
  begin
    points[0] := source.Top();
    points[1] := source.Bottom();
  end;
  south:
  begin
    points[0] := source.Bottom();
    points[1] := source.Top();
  end;
end

case x of 
  0..3: Writeln('small');
  4: Writeln('four');
else 
  Writeln('large');
end
