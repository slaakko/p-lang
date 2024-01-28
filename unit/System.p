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

type
  StringBuilder = object
    native: pointer;
    constructor();
    procedure Clear();
    procedure AppendStr(s: string);
    procedure AppendChar(c: char);
    function GetString(): string;
  end;

procedure Panic(message: string);

implementation

function IntToString(x: integer): string; external;
function RealToString(x: real): string; external;
function ParseInt(x: string): integer; external;
function ParseReal(x: string): real; external;

procedure InitStringBuilder(s: StringBuilder); external;

constructor StringBuilder();
begin
  InitStringBuilder(this);
end;

procedure StringBuilder.Clear(); external;
procedure StringBuilder.AppendStr(s: string); external;
procedure StringBuilder.AppendChar(c: char); external;
function StringBuilder.GetString(): string; external;

procedure Panic(message: string); external;

end.
