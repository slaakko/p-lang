{ Repeat statements: }

repeat
  Writeln(count);
  count := Pred(count);
until count = 0

repeat
  done := DoWork();
until done
