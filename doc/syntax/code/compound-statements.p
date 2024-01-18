{ Compound statements: }

begin
end

begin
  angleRad := pi / 180.0 * angleDeg;
  v := line.ToVector();  
  r := Rotate(v, angleRad);
  RotateLineSegment := new LineSegment(line.s, r);
end

begin
  count := 0;
  components := new Shape[4];
end
