// this file has been automatically generated from 'C:/work/springpp/p/p.lexer' using soul lexer generator slg version 5.0.0

module p.lexer;

using namespace p::token;

namespace p::lexer {

soul::ast::slg::TokenCollection* GetTokens()
{
    static soul::ast::slg::TokenCollection tokens("p.lexer.tokens");
    if (!tokens.Initialized())
    {
        tokens.SetInitialized();
        tokens.AddToken(new soul::ast::slg::Token(AND, "AND", "'and'"));
        tokens.AddToken(new soul::ast::slg::Token(ARRAY, "ARRAY", "'array'"));
        tokens.AddToken(new soul::ast::slg::Token(BASE, "BASE", "'base'"));
        tokens.AddToken(new soul::ast::slg::Token(BEGIN, "BEGIN", "'begin'"));
        tokens.AddToken(new soul::ast::slg::Token(CASE, "CASE", "'case'"));
        tokens.AddToken(new soul::ast::slg::Token(CONST, "CONST", "'const'"));
        tokens.AddToken(new soul::ast::slg::Token(CONSTRUCTOR, "CONSTRUCTOR", "'constructor'"));
        tokens.AddToken(new soul::ast::slg::Token(DIV, "DIV", "'div'"));
        tokens.AddToken(new soul::ast::slg::Token(DO, "DO", "'do'"));
        tokens.AddToken(new soul::ast::slg::Token(DOWNTO, "DOWNTO", "'downto'"));
        tokens.AddToken(new soul::ast::slg::Token(ELSE, "ELSE", "'else'"));
        tokens.AddToken(new soul::ast::slg::Token(END, "END", "'end'"));
        tokens.AddToken(new soul::ast::slg::Token(EXTERNAL, "EXTERNAL", "'external'"));
        tokens.AddToken(new soul::ast::slg::Token(FALSE, "FALSE", "'false'"));
        tokens.AddToken(new soul::ast::slg::Token(FOR, "FOR", "'for'"));
        tokens.AddToken(new soul::ast::slg::Token(FORWARD, "FORWARD", "'forward'"));
        tokens.AddToken(new soul::ast::slg::Token(FUNCTION, "FUNCTION", "'function'"));
        tokens.AddToken(new soul::ast::slg::Token(THIS, "THIS", "'this'"));
        tokens.AddToken(new soul::ast::slg::Token(NEW, "NEW", "'new'"));
        tokens.AddToken(new soul::ast::slg::Token(IF, "IF", "'if'"));
        tokens.AddToken(new soul::ast::slg::Token(IMPLEMENTATION, "IMPLEMENTATION", "implementation"));
        tokens.AddToken(new soul::ast::slg::Token(INTERFACE, "INTERFACE", "'interface'"));
        tokens.AddToken(new soul::ast::slg::Token(MOD, "MOD", "'mod'"));
        tokens.AddToken(new soul::ast::slg::Token(NIL, "NIL", "'nil'"));
        tokens.AddToken(new soul::ast::slg::Token(NOT, "NOT", "'not'"));
        tokens.AddToken(new soul::ast::slg::Token(OBJECT, "OBJECT", "'object'"));
        tokens.AddToken(new soul::ast::slg::Token(OF, "OF", "'of'"));
        tokens.AddToken(new soul::ast::slg::Token(OR, "OR", "'or'"));
        tokens.AddToken(new soul::ast::slg::Token(PROCEDURE, "PROCEDURE", "'procedure'"));
        tokens.AddToken(new soul::ast::slg::Token(PROGRAM, "PROGRAM", "'program'"));
        tokens.AddToken(new soul::ast::slg::Token(REPEAT, "REPEAT", "'repeat'"));
        tokens.AddToken(new soul::ast::slg::Token(SHL, "SHL", "'shl'"));
        tokens.AddToken(new soul::ast::slg::Token(SHR, "SHR", "'shr'"));
        tokens.AddToken(new soul::ast::slg::Token(THEN, "THEN", "'then'"));
        tokens.AddToken(new soul::ast::slg::Token(TO, "TO", "'to'"));
        tokens.AddToken(new soul::ast::slg::Token(TRUE, "TRUE", "'true'"));
        tokens.AddToken(new soul::ast::slg::Token(TYPE, "TYPE", "'type'"));
        tokens.AddToken(new soul::ast::slg::Token(UNTIL, "UNTIL", "'until'"));
        tokens.AddToken(new soul::ast::slg::Token(UNIT, "UNIT", "unit"));
        tokens.AddToken(new soul::ast::slg::Token(USES, "USES", "uses"));
        tokens.AddToken(new soul::ast::slg::Token(VAR, "VAR", "'var'"));
        tokens.AddToken(new soul::ast::slg::Token(WHILE, "WHILE", "'while'"));
        tokens.AddToken(new soul::ast::slg::Token(XOR, "XOR", "'xor'"));
        tokens.AddToken(new soul::ast::slg::Token(INTEGER, "INTEGER", "'integer'"));
        tokens.AddToken(new soul::ast::slg::Token(REAL, "REAL", "'real'"));
        tokens.AddToken(new soul::ast::slg::Token(BOOLEAN, "BOOLEAN", "'boolean'"));
        tokens.AddToken(new soul::ast::slg::Token(CHAR, "CHAR", "'char'"));
        tokens.AddToken(new soul::ast::slg::Token(STRING, "STRING", "'string'"));
        tokens.AddToken(new soul::ast::slg::Token(POINTER, "POINTER", "'pointer'"));
        tokens.AddToken(new soul::ast::slg::Token(ID, "ID", "identifier"));
        tokens.AddToken(new soul::ast::slg::Token(REAL_LITERAL, "REAL_LITERAL", "real literal"));
        tokens.AddToken(new soul::ast::slg::Token(INTEGER_LITERAL, "INTEGER_LITERAL", "integer literal"));
        tokens.AddToken(new soul::ast::slg::Token(QUOTED_STRING, "QUOTED_STRING", "quoted string"));
        tokens.AddToken(new soul::ast::slg::Token(CONTROL_STRING, "CONTROL_STRING", "control string"));
        tokens.AddToken(new soul::ast::slg::Token(EQUAL, "EQUAL", "'='"));
        tokens.AddToken(new soul::ast::slg::Token(NOT_EQUAL, "NOT_EQUAL", "'<>'"));
        tokens.AddToken(new soul::ast::slg::Token(LESS, "LESS", "'<'"));
        tokens.AddToken(new soul::ast::slg::Token(GREATER, "GREATER", "'>'"));
        tokens.AddToken(new soul::ast::slg::Token(LESS_EQUAL, "LESS_EQUAL", "'<='"));
        tokens.AddToken(new soul::ast::slg::Token(GREATER_EQUAL, "GREATER_EQUAL", "'>='"));
        tokens.AddToken(new soul::ast::slg::Token(PLUS, "PLUS", "'+'"));
        tokens.AddToken(new soul::ast::slg::Token(MINUS, "MINUS", "'-'"));
        tokens.AddToken(new soul::ast::slg::Token(STAR, "STAR", "'*'"));
        tokens.AddToken(new soul::ast::slg::Token(SLASH, "SLASH", "'/'"));
        tokens.AddToken(new soul::ast::slg::Token(DOTDOT, "DOTDOT", ".."));
        tokens.AddToken(new soul::ast::slg::Token(DOT, "DOT", "'.'"));
        tokens.AddToken(new soul::ast::slg::Token(LBRACKET, "LBRACKET", "'['"));
        tokens.AddToken(new soul::ast::slg::Token(RBRACKET, "RBRACKET", "']'"));
        tokens.AddToken(new soul::ast::slg::Token(LPAREN, "LPAREN", "'('"));
        tokens.AddToken(new soul::ast::slg::Token(RPAREN, "RPAREN", "')'"));
        tokens.AddToken(new soul::ast::slg::Token(LBRACE, "LBRACE", "'{'"));
        tokens.AddToken(new soul::ast::slg::Token(RBRACE, "RBRACE", "'}'"));
        tokens.AddToken(new soul::ast::slg::Token(COMMA, "COMMA", "','"));
        tokens.AddToken(new soul::ast::slg::Token(ASSIGN, "ASSIGN", "':='"));
        tokens.AddToken(new soul::ast::slg::Token(COLON, "COLON", "':'"));
        tokens.AddToken(new soul::ast::slg::Token(SEMICOLON, "SEMICOLON", "';'"));
        tokens.AddToken(new soul::ast::slg::Token(WS, "WS", "ws"));
        tokens.AddToken(new soul::ast::slg::Token(VIRTUAL, "VIRTUAL", "virtual"));
        tokens.AddToken(new soul::ast::slg::Token(OVERRIDE, "OVERRIDE", "override"));
    }
    return &tokens;
}

PLexer_Variables::PLexer_Variables()
{
}

std::mutex mtx;

std::mutex& MakeLexerMtx() { return mtx; }

template<>
soul::lexer::KeywordMap<char>* GetKeywords<char>()
{
    static const soul::lexer::Keyword<char> keywords[] = {
         { "and", AND },
         { "array", ARRAY },
         { "base", BASE },
         { "begin", BEGIN },
         { "boolean", BOOLEAN },
         { "case", CASE },
         { "char", CHAR },
         { "const", CONST },
         { "constructor", CONSTRUCTOR },
         { "this", THIS },
         { "new", NEW },
         { "div", DIV },
         { "do", DO },
         { "downto", DOWNTO },
         { "else", ELSE },
         { "end", END },
         { "external", EXTERNAL },
         { "false", FALSE },
         { "for", FOR },
         { "forward", FORWARD },
         { "function", FUNCTION },
         { "if", IF },
         { "implementation", IMPLEMENTATION },
         { "integer", INTEGER },
         { "interface", INTERFACE },
         { "mod", MOD },
         { "nil", NIL },
         { "not", NOT },
         { "object", OBJECT },
         { "of", OF },
         { "or", OR },
         { "procedure", PROCEDURE },
         { "pointer", POINTER },
         { "program", PROGRAM },
         { "real", REAL },
         { "repeat", REPEAT },
         { "shl", SHL },
         { "shr", SHR },
         { "string", STRING },
         { "then", THEN },
         { "to", TO },
         { "true", TRUE },
         { "type", TYPE },
         { "until", UNTIL },
         { "unit", UNIT },
         { "uses", USES },
         { "var", VAR },
         { "while", WHILE },
         { "xor", XOR },
         { "virtual", VIRTUAL },
         { "override", OVERRIDE },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char8_t>* GetKeywords<char8_t>()
{
    static const soul::lexer::Keyword<char8_t> keywords[] = {
         { u8"and", AND },
         { u8"array", ARRAY },
         { u8"base", BASE },
         { u8"begin", BEGIN },
         { u8"boolean", BOOLEAN },
         { u8"case", CASE },
         { u8"char", CHAR },
         { u8"const", CONST },
         { u8"constructor", CONSTRUCTOR },
         { u8"this", THIS },
         { u8"new", NEW },
         { u8"div", DIV },
         { u8"do", DO },
         { u8"downto", DOWNTO },
         { u8"else", ELSE },
         { u8"end", END },
         { u8"external", EXTERNAL },
         { u8"false", FALSE },
         { u8"for", FOR },
         { u8"forward", FORWARD },
         { u8"function", FUNCTION },
         { u8"if", IF },
         { u8"implementation", IMPLEMENTATION },
         { u8"integer", INTEGER },
         { u8"interface", INTERFACE },
         { u8"mod", MOD },
         { u8"nil", NIL },
         { u8"not", NOT },
         { u8"object", OBJECT },
         { u8"of", OF },
         { u8"or", OR },
         { u8"procedure", PROCEDURE },
         { u8"pointer", POINTER },
         { u8"program", PROGRAM },
         { u8"real", REAL },
         { u8"repeat", REPEAT },
         { u8"shl", SHL },
         { u8"shr", SHR },
         { u8"string", STRING },
         { u8"then", THEN },
         { u8"to", TO },
         { u8"true", TRUE },
         { u8"type", TYPE },
         { u8"until", UNTIL },
         { u8"unit", UNIT },
         { u8"uses", USES },
         { u8"var", VAR },
         { u8"while", WHILE },
         { u8"xor", XOR },
         { u8"virtual", VIRTUAL },
         { u8"override", OVERRIDE },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char8_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char16_t>* GetKeywords<char16_t>()
{
    static const soul::lexer::Keyword<char16_t> keywords[] = {
         { u"and", AND },
         { u"array", ARRAY },
         { u"base", BASE },
         { u"begin", BEGIN },
         { u"boolean", BOOLEAN },
         { u"case", CASE },
         { u"char", CHAR },
         { u"const", CONST },
         { u"constructor", CONSTRUCTOR },
         { u"this", THIS },
         { u"new", NEW },
         { u"div", DIV },
         { u"do", DO },
         { u"downto", DOWNTO },
         { u"else", ELSE },
         { u"end", END },
         { u"external", EXTERNAL },
         { u"false", FALSE },
         { u"for", FOR },
         { u"forward", FORWARD },
         { u"function", FUNCTION },
         { u"if", IF },
         { u"implementation", IMPLEMENTATION },
         { u"integer", INTEGER },
         { u"interface", INTERFACE },
         { u"mod", MOD },
         { u"nil", NIL },
         { u"not", NOT },
         { u"object", OBJECT },
         { u"of", OF },
         { u"or", OR },
         { u"procedure", PROCEDURE },
         { u"pointer", POINTER },
         { u"program", PROGRAM },
         { u"real", REAL },
         { u"repeat", REPEAT },
         { u"shl", SHL },
         { u"shr", SHR },
         { u"string", STRING },
         { u"then", THEN },
         { u"to", TO },
         { u"true", TRUE },
         { u"type", TYPE },
         { u"until", UNTIL },
         { u"unit", UNIT },
         { u"uses", USES },
         { u"var", VAR },
         { u"while", WHILE },
         { u"xor", XOR },
         { u"virtual", VIRTUAL },
         { u"override", OVERRIDE },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char16_t> keywordMap(keywords);
    return &keywordMap;
}

template<>
soul::lexer::KeywordMap<char32_t>* GetKeywords<char32_t>()
{
    static const soul::lexer::Keyword<char32_t> keywords[] = {
         { U"and", AND },
         { U"array", ARRAY },
         { U"base", BASE },
         { U"begin", BEGIN },
         { U"boolean", BOOLEAN },
         { U"case", CASE },
         { U"char", CHAR },
         { U"const", CONST },
         { U"constructor", CONSTRUCTOR },
         { U"this", THIS },
         { U"new", NEW },
         { U"div", DIV },
         { U"do", DO },
         { U"downto", DOWNTO },
         { U"else", ELSE },
         { U"end", END },
         { U"external", EXTERNAL },
         { U"false", FALSE },
         { U"for", FOR },
         { U"forward", FORWARD },
         { U"function", FUNCTION },
         { U"if", IF },
         { U"implementation", IMPLEMENTATION },
         { U"integer", INTEGER },
         { U"interface", INTERFACE },
         { U"mod", MOD },
         { U"nil", NIL },
         { U"not", NOT },
         { U"object", OBJECT },
         { U"of", OF },
         { U"or", OR },
         { U"procedure", PROCEDURE },
         { U"pointer", POINTER },
         { U"program", PROGRAM },
         { U"real", REAL },
         { U"repeat", REPEAT },
         { U"shl", SHL },
         { U"shr", SHR },
         { U"string", STRING },
         { U"then", THEN },
         { U"to", TO },
         { U"true", TRUE },
         { U"type", TYPE },
         { U"until", UNTIL },
         { U"unit", UNIT },
         { U"uses", USES },
         { U"var", VAR },
         { U"while", WHILE },
         { U"xor", XOR },
         { U"virtual", VIRTUAL },
         { U"override", OVERRIDE },
         { nullptr, -1 }
    };
    static soul::lexer::KeywordMap<char32_t> keywordMap(keywords);
    return &keywordMap;
}

} // namespace p::lexer
