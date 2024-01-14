// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.writer;

namespace p {

Writer::Writer(const std::string& filePath_) :
    filePath(filePath_),
    file(filePath, util::OpenMode::binary | util::OpenMode::write),
    buffered(file),
    binaryWriter(buffered)
{
}

} // namespace p
