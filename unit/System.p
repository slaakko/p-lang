unit System;

interface

type
  Byte = 0..255;

const
  pi: real = 3.1415926535897932385;

function IntToString(x: integer): string;
function RealToString(x: real): string;
function ParseInt(x: string): integer;
function ParseReal(x: string): real;

implementation

function IntToString(x: integer): string; external;
function RealToString(x: real): string; external;
function ParseInt(x: string): integer; external;
function ParseReal(x: string): real; external;

end.
