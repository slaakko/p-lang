// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module p.core.writer;

import p.core.symbol;
import p.core.value;

namespace p {

SymbolWriter::SymbolWriter(const std::string& filePath_) : AstWriter(filePath_)
{
}

void SymbolWriter::WriteHeader()
{
    GetBinaryWriter().Write(pcodeHeaderMagic, false);
}

void SymbolWriter::WriteSymbol(Symbol* symbol)
{
    if (symbol)
    {
        GetBinaryWriter().Write(static_cast<uint8_t>(symbol->Kind()));
        WriteSpan(symbol->Span());
        GetBinaryWriter().Write(symbol->Name());
        symbol->Write(*this);
    }
    else
    {
        GetBinaryWriter().Write(static_cast<uint8_t>(SymbolKind::none));
    }
}

void SymbolWriter::WriteValue(Value* value)
{
    if (value)
    {
        GetBinaryWriter().Write(static_cast<uint8_t>(value->Kind()));
        value->Write(*this);
    }
    else
    {
        GetBinaryWriter().Write(static_cast<uint8_t>(ValueKind::none));
    }
}

} // namespace p
