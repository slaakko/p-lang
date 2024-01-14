// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.unicode;

import std.core;

export namespace util {

class UnicodeException : public std::runtime_error
{
public:
    UnicodeException(const std::string& message_);
};

void ThrowInvalidUtf8Sequence();
void ThrowUnicodeException(const std::string& message_);

std::u32string ToUtf32(const std::string& utf8Str);
std::u32string ToUtf32(const std::u16string& utf16Str);
std::u32string ToUtf32(const std::u32string& utf32Str) { return utf32Str; }
std::u16string ToUtf16(const std::u32string& utf32Str);
std::u16string ToUtf16(const std::u16string& utf16Str) { return utf16Str; }
std::u16string ToUtf16(const std::string& u8str);
std::string ToUtf8(const std::u32string& utf32Str);
std::string ToUtf8(const std::u16string& utf16Str);
std::string ToUtf8(const std::string& utf8Str) { return utf8Str; }

class Utf8ToUtf32Engine
{
public:
    Utf8ToUtf32Engine();
    void Put(uint8_t x);
    bool ResultReady() const { return resultReady; }
    char32_t Result() const { return result; }
private:
    int state;
    bool resultReady;
    char32_t result;
    uint8_t bytes[4];
};

} // util
