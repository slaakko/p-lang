{ If statements: }

if n > 1 then
begin
  { ... }
end

if i < n - 1 then graphics.DrawLine(blackPen, s, e) 
else DrawArrowLine(graphics, s, e, lineArrowWidth, lineArrowHeight)

if not r.IsEmpty() then
begin
  if bounds.IsEmpty() then bounds := r else bounds := Union(bounds, r);
end
