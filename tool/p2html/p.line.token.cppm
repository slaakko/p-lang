
// this file has been automatically generated from 'C:/work/p-lang/tool/p2html/p.line.token' using soul lexer generator version 5.0.0

export module p.line.token;

import std.core;

export namespace p::line::token {

constexpr int32_t tokenSetID = 302252482;

constexpr int64_t AND = (static_cast<int64_t>(tokenSetID) << 32) | 1;
constexpr int64_t ARRAY = (static_cast<int64_t>(tokenSetID) << 32) | 2;
constexpr int64_t BASE = (static_cast<int64_t>(tokenSetID) << 32) | 3;
constexpr int64_t BEGIN = (static_cast<int64_t>(tokenSetID) << 32) | 4;
constexpr int64_t CASE = (static_cast<int64_t>(tokenSetID) << 32) | 5;
constexpr int64_t CONST = (static_cast<int64_t>(tokenSetID) << 32) | 6;
constexpr int64_t CONSTRUCTOR = (static_cast<int64_t>(tokenSetID) << 32) | 7;
constexpr int64_t DIV = (static_cast<int64_t>(tokenSetID) << 32) | 8;
constexpr int64_t DO = (static_cast<int64_t>(tokenSetID) << 32) | 9;
constexpr int64_t DOWNTO = (static_cast<int64_t>(tokenSetID) << 32) | 10;
constexpr int64_t ELSE = (static_cast<int64_t>(tokenSetID) << 32) | 11;
constexpr int64_t END = (static_cast<int64_t>(tokenSetID) << 32) | 12;
constexpr int64_t EXTERNAL = (static_cast<int64_t>(tokenSetID) << 32) | 13;
constexpr int64_t FALSE = (static_cast<int64_t>(tokenSetID) << 32) | 14;
constexpr int64_t FOR = (static_cast<int64_t>(tokenSetID) << 32) | 15;
constexpr int64_t FORWARD = (static_cast<int64_t>(tokenSetID) << 32) | 16;
constexpr int64_t FUNCTION = (static_cast<int64_t>(tokenSetID) << 32) | 17;
constexpr int64_t THIS = (static_cast<int64_t>(tokenSetID) << 32) | 18;
constexpr int64_t NEW = (static_cast<int64_t>(tokenSetID) << 32) | 19;
constexpr int64_t IF = (static_cast<int64_t>(tokenSetID) << 32) | 20;
constexpr int64_t IMPLEMENTATION = (static_cast<int64_t>(tokenSetID) << 32) | 21;
constexpr int64_t INTERFACE = (static_cast<int64_t>(tokenSetID) << 32) | 22;
constexpr int64_t MOD = (static_cast<int64_t>(tokenSetID) << 32) | 23;
constexpr int64_t NIL = (static_cast<int64_t>(tokenSetID) << 32) | 24;
constexpr int64_t NOT = (static_cast<int64_t>(tokenSetID) << 32) | 25;
constexpr int64_t OBJECT = (static_cast<int64_t>(tokenSetID) << 32) | 26;
constexpr int64_t OF = (static_cast<int64_t>(tokenSetID) << 32) | 27;
constexpr int64_t OR = (static_cast<int64_t>(tokenSetID) << 32) | 28;
constexpr int64_t PROCEDURE = (static_cast<int64_t>(tokenSetID) << 32) | 29;
constexpr int64_t PROGRAM = (static_cast<int64_t>(tokenSetID) << 32) | 30;
constexpr int64_t REPEAT = (static_cast<int64_t>(tokenSetID) << 32) | 31;
constexpr int64_t SHL = (static_cast<int64_t>(tokenSetID) << 32) | 32;
constexpr int64_t SHR = (static_cast<int64_t>(tokenSetID) << 32) | 33;
constexpr int64_t THEN = (static_cast<int64_t>(tokenSetID) << 32) | 34;
constexpr int64_t TO = (static_cast<int64_t>(tokenSetID) << 32) | 35;
constexpr int64_t TRUE = (static_cast<int64_t>(tokenSetID) << 32) | 36;
constexpr int64_t TYPE = (static_cast<int64_t>(tokenSetID) << 32) | 37;
constexpr int64_t UNTIL = (static_cast<int64_t>(tokenSetID) << 32) | 38;
constexpr int64_t UNIT = (static_cast<int64_t>(tokenSetID) << 32) | 39;
constexpr int64_t USES = (static_cast<int64_t>(tokenSetID) << 32) | 40;
constexpr int64_t VAR = (static_cast<int64_t>(tokenSetID) << 32) | 41;
constexpr int64_t WHILE = (static_cast<int64_t>(tokenSetID) << 32) | 42;
constexpr int64_t XOR = (static_cast<int64_t>(tokenSetID) << 32) | 43;
constexpr int64_t INTEGER = (static_cast<int64_t>(tokenSetID) << 32) | 44;
constexpr int64_t REAL = (static_cast<int64_t>(tokenSetID) << 32) | 45;
constexpr int64_t BOOLEAN = (static_cast<int64_t>(tokenSetID) << 32) | 46;
constexpr int64_t CHAR = (static_cast<int64_t>(tokenSetID) << 32) | 47;
constexpr int64_t STRING = (static_cast<int64_t>(tokenSetID) << 32) | 48;
constexpr int64_t POINTER = (static_cast<int64_t>(tokenSetID) << 32) | 49;
constexpr int64_t VIRTUAL = (static_cast<int64_t>(tokenSetID) << 32) | 50;
constexpr int64_t OVERRIDE = (static_cast<int64_t>(tokenSetID) << 32) | 51;
constexpr int64_t KEYWORD_TOKEN = (static_cast<int64_t>(tokenSetID) << 32) | 52;
constexpr int64_t IDENTIFIER_TOKEN = (static_cast<int64_t>(tokenSetID) << 32) | 53;
constexpr int64_t NUMBER_TOKEN = (static_cast<int64_t>(tokenSetID) << 32) | 54;
constexpr int64_t STRING_TOKEN = (static_cast<int64_t>(tokenSetID) << 32) | 55;
constexpr int64_t START_BRACE_COMMENT_TOKEN = (static_cast<int64_t>(tokenSetID) << 32) | 56;
constexpr int64_t END_BRACE_COMMENT_TOKEN = (static_cast<int64_t>(tokenSetID) << 32) | 57;
constexpr int64_t START_BLOCK_COMMENT_TOKEN = (static_cast<int64_t>(tokenSetID) << 32) | 58;
constexpr int64_t END_BLOCK_COMMENT_TOKEN = (static_cast<int64_t>(tokenSetID) << 32) | 59;
constexpr int64_t OTHER_TOKEN = (static_cast<int64_t>(tokenSetID) << 32) | 60;
constexpr int64_t WS_TOKEN = (static_cast<int64_t>(tokenSetID) << 32) | 61;

} // p::line::token
