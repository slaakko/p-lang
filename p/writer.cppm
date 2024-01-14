// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.writer;

import util;
import std.core;

export namespace p {

class Writer
{
public:
    Writer(const std::string& filePath_);
    util::BinaryStreamWriter& GetBinaryWriter() { return binaryWriter; }
private:
    std::string filePath;
    util::FileStream file;
    util::BufferedStream buffered; 
    util::BinaryStreamWriter binaryWriter;
};

} // namespace p
