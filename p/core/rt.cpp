// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>
#include <gdiplus.h>
#undef min
#undef max
#undef GetObject

module p.core.rt;

import p.core.value;
import p.core.execution_context;
import p.core.heap;
import p.core.object_type_symbol;
import p.core.symbol_table;

namespace p {

#pragma comment (lib,"Gdiplus.lib")

enum class ImageFormat
{
    bmp, jpeg, gif, tiff, png
};

std::string GetImageFormatStr(ImageFormat imageFormat)
{
    switch (imageFormat)
    {
    case ImageFormat::bmp: return "image/bmp";
    case ImageFormat::jpeg: return "image/jpeg";
    case ImageFormat::gif: return "image/gif";
    case ImageFormat::tiff: return "image/tiff";
    case ImageFormat::png: return "image/png";
    }
    return std::string();
}

int GetEncoderClsId(ImageFormat imageFormat, CLSID& guid)
{
    std::string imageFormatName = GetImageFormatStr(imageFormat);
    std::u16string imageFormatStr = util::ToUtf16(imageFormatName);
    Gdiplus::ImageCodecInfo* imageCodecInfo = nullptr;
    uint32_t n = 0;
    uint32_t size = 0;
    int result = Gdiplus::GetImageEncodersSize(&n, &size);
    if (result != 0 || size == 0)
    {
        return result;
    }
    imageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
    if (!imageCodecInfo) return 3;
    result = Gdiplus::GetImageEncoders(n, size, imageCodecInfo);
    if (result != 0)
    {
        return result;
    }
    for (uint32_t i = 0; i < n; ++i)
    {
        std::u16string mimetype = reinterpret_cast<const char16_t*>(imageCodecInfo[i].MimeType);
        if (mimetype == imageFormatStr)
        {
            guid = imageCodecInfo[i].Clsid;
            free(imageCodecInfo);
            return 0;
        }
    }
    free(imageCodecInfo);
    return -1;
}

struct PenKey
{
    PenKey(Gdiplus::Color color_, float width_) : color(color_), width(width_) {}
    Gdiplus::Color color;
    float width;
};

bool operator==(const PenKey& left, const PenKey& right)
{
    return left.color.GetValue() == right.color.GetValue() && left.width == right.width;
}

bool operator<(const PenKey& left, const PenKey& right)
{
    if (left.color.GetValue() < right.color.GetValue()) return true;
    if (left.color.GetValue() > right.color.GetValue()) return false;
    return left.width < right.width;
}

struct FontKey
{
    FontKey(const std::string& family_, float size_, int32_t style_);
    std::string family;
    float size;
    int32_t style;
};

FontKey::FontKey(const std::string& family_, float size_, int32_t style_) : family(family_), size(size_), style(style_)
{
}

bool operator==(const FontKey& left, const FontKey& right)
{
    return left.family == right.family && left.size == right.size && left.style == right.style;
}

bool operator<(const FontKey& left, const FontKey& right)
{
    if (left.family < right.family) return true;
    if (left.family > right.family) return false;
    if (left.size < right.size) return true;
    if (left.size > right.size) return false;
    return left.style < right.style;
}

struct BrushKey
{
    BrushKey(const Gdiplus::Color color_) : color(color_) {}
    Gdiplus::Color color;
};

bool operator==(const BrushKey& left, const BrushKey& right)
{
    return left.color.GetValue() == right.color.GetValue();
}

bool operator<(const BrushKey& left, const BrushKey& right)
{
    return left.color.GetValue() < right.color.GetValue();
}

ExternalSubroutineSymbol::ExternalSubroutineSymbol(const soul::ast::Span& span_, const std::string& name_, const util::uuid& id_) : 
    SubroutineSymbol(SymbolKind::externalSubroutineSymbol, span_, name_, id_)
{
}

class Rt
{
public:
    static Rt& Instance();
    const std::string& FilePath() const { return filePath; }
    void Init(const std::string& filePath_, const soul::ast::Span& span);
    void Done();
    void AddExternalSubroutine(ExternalSubroutineSymbol* externalSubroutine);
    ExternalSubroutineSymbol* GetExternalSubroutine(const std::string& externalSubroutineName, Node* node) const;
    ExternalSubroutineSymbol* GetExternalSubroutineNoThrow(const std::string& externalSubroutineName) const;
    ExternalSubroutineSymbol* GetExternalSubroutine(const util::uuid& id) const;
    Gdiplus::Pen* GetPen(const PenKey& penKey);
    Gdiplus::Font* GetFont(const FontKey& fontKey);
    Gdiplus::Brush* GetBrush(const Gdiplus::Color color);
private:
    Rt();
    std::string filePath;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    std::map<std::string, ExternalSubroutineSymbol*> externalSubroutineMap;
    std::map<util::uuid, ExternalSubroutineSymbol*> externalSubroutineIdMap;
    std::vector<std::unique_ptr<ExternalSubroutineSymbol>> externalSubroutines;
    std::map<PenKey, Gdiplus::Pen*> penMap;
    std::vector<std::unique_ptr<Gdiplus::Pen>> pens;
    std::map<FontKey, Gdiplus::Font*> fontMap;
    std::vector<std::unique_ptr<Gdiplus::Font>> fonts;
    std::map<BrushKey, Gdiplus::Brush*> brushMap;
    std::vector<std::unique_ptr<Gdiplus::Brush>> brushes;
};

Rt& Rt::Instance()
{
    static Rt instance;
    return instance;
}

Rt::Rt()
{
}

Gdiplus::Pen* Rt::GetPen(const PenKey& penKey)
{
    auto it = penMap.find(penKey);
    if (it != penMap.end())
    {
        return it->second;
    }
    else
    {
        Gdiplus::Pen* pen = new Gdiplus::Pen(penKey.color, penKey.width);
        pens.push_back(std::unique_ptr<Gdiplus::Pen>(pen));
        penMap[penKey] = pen;
        return pen;
    }
}

Gdiplus::Font* Rt::GetFont(const FontKey& fontKey)
{
    auto it = fontMap.find(fontKey);
    if (it != fontMap.end())
    {
        return it->second;
    }
    else
    {
        std::u16string familyName = util::ToUtf16(fontKey.family);
        Gdiplus::FontFamily family((const WCHAR*)familyName.c_str(), nullptr);
        Gdiplus::Font* font = new Gdiplus::Font(&family, fontKey.size, fontKey.style, Gdiplus::UnitPoint);
        fontMap[fontKey] = font;
        fonts.push_back(std::unique_ptr<Gdiplus::Font>(font));
        return font;
    }
}

Gdiplus::Brush* Rt::GetBrush(const Gdiplus::Color color)
{
    BrushKey key(color);
    auto it = brushMap.find(key);
    if (it != brushMap.end())
    {
        return it->second;
    }
    else
    {
        Gdiplus::Brush* brush = new Gdiplus::SolidBrush(color);
        brushes.push_back(std::unique_ptr<Gdiplus::Brush>(brush));
        brushMap[key] = brush;
        return brush;
    }
}

void Rt::AddExternalSubroutine(ExternalSubroutineSymbol* externalSubroutine)
{
    externalSubroutineMap[externalSubroutine->Name()] = externalSubroutine;
    externalSubroutineIdMap[externalSubroutine->Id()] = externalSubroutine;
    externalSubroutines.push_back(std::unique_ptr<ExternalSubroutineSymbol>(externalSubroutine));
}

util::uuid PanicId()
{
    util::uuid id;
    id.data[0] = 1;
    id.data[15] = 1;
    return id;
}

util::uuid IntToStringId()
{
    util::uuid id;
    id.data[0] = 1;
    id.data[15] = 2;
    return id;
}

util::uuid RealToStringId()
{
    util::uuid id;
    id.data[0] = 1;
    id.data[15] = 3;
    return id;
}

util::uuid ParseIntId()
{
    util::uuid id;
    id.data[0] = 1;
    id.data[15] = 4;
    return id;
}

util::uuid ParseRealId()
{
    util::uuid id;
    id.data[0] = 1;
    id.data[15] = 5;
    return id;
}

util::uuid InitStringBuilderId()
{
    util::uuid id;
    id.data[0] = 1;
    id.data[15] = 6;
    return id;
}

util::uuid StringBuilderClearId()
{
    util::uuid id;
    id.data[0] = 1;
    id.data[15] = 7;
    return id;
}

util::uuid StringBuilderAppendStrId()
{
    util::uuid id;
    id.data[0] = 1;
    id.data[15] = 8;
    return id;
}

util::uuid StringBuilderAppendCharId()
{
    util::uuid id;
    id.data[0] = 1;
    id.data[15] = 9;
    return id;
}

util::uuid StringBuilderGetStringId()
{
    util::uuid id;
    id.data[0] = 1;
    id.data[15] = 10;
    return id;
}

util::uuid StringBuilderDisposeId()
{
    util::uuid id;
    id.data[0] = 1;
    id.data[15] = 11;
    return id;
}

util::uuid BitmapGetGraphicsId()
{
    util::uuid id;
    id.data[0] = 1;
    id.data[15] = 12;
    return id;
}

util::uuid BitmapSaveId()
{
    util::uuid id;
    id.data[0] = 1;
    id.data[15] = 13;
    return id;
}

util::uuid GraphicsClearId()
{
    util::uuid id;
    id.data[0] = 1;
    id.data[15] = 14;
    return id;
}

util::uuid GraphicsDrawLineId()
{
    util::uuid id;
    id.data[0] = 1;
    id.data[15] = 15;
    return id;
}

util::uuid GraphicsDrawRectangleId()
{
    util::uuid id;
    id.data[0] = 1;
    id.data[15] = 16;
    return id;
}

util::uuid GraphicsMeasureStringId()
{
    util::uuid id;
    id.data[0] = 1;
    id.data[15] = 17;
    return id;
}

util::uuid GraphicsDrawStringId()
{
    util::uuid id;
    id.data[0] = 1;
    id.data[15] = 18;
    return id;
}

util::uuid GraphicsFillPolygonId()
{
    util::uuid id;
    id.data[0] = 1;
    id.data[15] = 19;
    return id;
}

util::uuid GraphicsDrawEllipseId()
{
    util::uuid id;
    id.data[0] = 1;
    id.data[15] = 20;
    return id;
}

util::uuid GraphicsDrawArcId()
{
    util::uuid id;
    id.data[0] = 1;
    id.data[15] = 21;
    return id;
}

class PanicProcedure : public ExternalSubroutineSymbol
{
public:
    PanicProcedure(const soul::ast::Span& span_);
    void Execute(ExecutionContext* context) override;
};

PanicProcedure::PanicProcedure(const soul::ast::Span& span_) : ExternalSubroutineSymbol(span_, "Panic", PanicId())
{
}

void PanicProcedure::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> object = stack->Pop();
    Object* obj = object->GetObject();
    std::string message = obj->ToString();
    throw std::runtime_error("panic: " + message);
}

class IntToStringFunction : public ExternalSubroutineSymbol
{
public:
    IntToStringFunction(const soul::ast::Span& span_);
    void Execute(ExecutionContext* context) override;
};

IntToStringFunction::IntToStringFunction(const soul::ast::Span& span_) : ExternalSubroutineSymbol(span_, "IntToString", IntToStringId())
{
}

void IntToStringFunction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    Heap* heap = context->GetHeap();
    std::unique_ptr<Object> object = stack->Pop();
    Object* obj = object->GetObject();
    if (obj->IsValueObject())
    {
        Value* value = static_cast<Value*>(obj);
        if (value->IsIntegerValue())
        {
            int32_t intValue = value->ToInteger();
            std::string s = std::to_string(intValue);
            StringObject* stringObject = heap->AllocateString(s, context);
            stack->Push(new StringObjectPtr(stringObject));
        }
        else
        {
            ThrowError("error: IntToString: integer value expected", Rt::Instance().FilePath(), Span());
        }
    }
    else
    {
        ThrowError("error: IntToString: value object expected", Rt::Instance().FilePath(), Span());
    }
}

class RealToStringFunction : public ExternalSubroutineSymbol
{
public:
    RealToStringFunction(const soul::ast::Span& span_);
    void Execute(ExecutionContext* context) override;
};

RealToStringFunction::RealToStringFunction(const soul::ast::Span& span_) : ExternalSubroutineSymbol(span_, "RealToString", RealToStringId())
{
}

void RealToStringFunction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    Heap* heap = context->GetHeap();
    std::unique_ptr<Object> object = stack->Pop();
    Object* obj = object->GetObject();
    if (obj->IsValueObject())
    {
        Value* value = static_cast<Value*>(obj);
        if (value->IsIntegerValue() || value->IsRealValue())
        {
            float realValue = value->ToReal();
            std::string s;
            if (realValue == static_cast<int32_t>(realValue))
            {
                s = std::to_string(static_cast<int32_t>(realValue));
            }
            else
            {
                s = std::to_string(realValue);
            }
            StringObject* stringObject = heap->AllocateString(s, context);
            stack->Push(new StringObjectPtr(stringObject));
        }
        else
        {
            ThrowError("error: RealToString: real or integer value expected", Rt::Instance().FilePath(), Span());
        }
    }
    else
    {
        ThrowError("error: RealToString: value object expected", Rt::Instance().FilePath(), Span());
    }
}

class ParseIntFunction : public ExternalSubroutineSymbol
{
public:
    ParseIntFunction(const soul::ast::Span& span_);
    void Execute(ExecutionContext* context) override;
};

ParseIntFunction::ParseIntFunction(const soul::ast::Span& span_) : ExternalSubroutineSymbol(span_, "ParseInt", ParseIntId())
{
}

void ParseIntFunction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> object = stack->Pop();
    Object* obj = object->GetObject();
    if (obj->IsStringObject() || obj->IsValueObject() && static_cast<Value*>(obj)->IsStringValue())
    {
        std::string s = obj->ToString();
        int32_t x = std::stoi(s);
        stack->Push(new IntegerValue(x));
    }
    else
    {
        ThrowError("error: ParseInt: string object expected", Rt::Instance().FilePath(), Span());
    }
}

class ParseRealFunction : public ExternalSubroutineSymbol
{
public:
    ParseRealFunction(const soul::ast::Span& span_);
    void Execute(ExecutionContext* context) override;
};

ParseRealFunction::ParseRealFunction(const soul::ast::Span& span_) : ExternalSubroutineSymbol(span_, "ParseReal", ParseRealId())
{
}

void ParseRealFunction::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> object = stack->Pop();
    Object* obj = object->GetObject();
    if (obj->IsStringObject() || obj->IsValueObject() && static_cast<Value*>(obj)->IsStringValue())
    {
        std::string s = obj->ToString();
        float x = std::stof(s);
        stack->Push(new RealValue(x));
    }
    else
    {
        ThrowError("error: ParseReal: string object expected", Rt::Instance().FilePath(), Span());
    }
}

class InitStringBuilderProcedure : public ExternalSubroutineSymbol
{
public:
    InitStringBuilderProcedure(const soul::ast::Span& span_);
    void Execute(ExecutionContext* context) override;
};

InitStringBuilderProcedure::InitStringBuilderProcedure(const soul::ast::Span& span_) : ExternalSubroutineSymbol(span_, "InitStringBuilder", InitStringBuilderId())
{
}

void InitStringBuilderProcedure::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> object = stack->Pop();
    Object* obj = object->GetObject();
    if (obj->IsHeapObject())
    {
        HeapObject* stringBuilderObject = static_cast<HeapObject*>(obj);
        ObjectTypeSymbol* objectType = stringBuilderObject->GetType();
        int32_t nativeIndex = objectType->GetFieldIndex("native");
        if (nativeIndex != -1)
        {
            std::string* string = new std::string();
            GenericPointerValue pointerValue(string);
            stringBuilderObject->SetField(nativeIndex, &pointerValue, context);
        }
        else
        {
            ThrowError("error: InitStringBuilder: 'native' field not found", Rt::Instance().FilePath(), Span());
        }
    }
    else
    {
        ThrowError("error: InitStringBuilder: heap object expected", Rt::Instance().FilePath(), Span());
    }
}

class StringBuilderClearMethod : public ExternalSubroutineSymbol
{
public:
    StringBuilderClearMethod(const soul::ast::Span& span_);
    void Execute(ExecutionContext* context) override;
};

StringBuilderClearMethod::StringBuilderClearMethod(const soul::ast::Span& span_) : ExternalSubroutineSymbol(span_, "StringBuilder.Clear", StringBuilderClearId())
{
}

void StringBuilderClearMethod::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> object = stack->Pop();
    Object* obj = object->GetObject();
    if (obj->IsHeapObject())
    {
        HeapObject* stringBuilderObject = static_cast<HeapObject*>(obj);
        ObjectTypeSymbol* objectType = stringBuilderObject->GetType();
        int32_t nativeIndex = objectType->GetFieldIndex("native");
        if (nativeIndex != -1)
        {
            Object* nativeObject = stringBuilderObject->GetField(nativeIndex);
            Object* native = nativeObject->GetObject();
            if (native->IsValueObject())
            {
                Value* nativeValue = static_cast<Value*>(native);
                if (nativeValue->IsGenericPointerValue())
                {
                    GenericPointerValue* nativePtr = static_cast<GenericPointerValue*>(nativeValue);
                    void* np = nativePtr->Pointer();
                    if (np)
                    {
                        std::string* string = static_cast<std::string*>(np);
                        string->clear();
                    }
                }
                else
                {
                    ThrowError("error: StringBuilder.Clear: pointer value expected", Rt::Instance().FilePath(), Span());
                }
            }
            else
            {
                ThrowError("error: StringBuilder.Clear: value object expected", Rt::Instance().FilePath(), Span());
            }
        }
        else
        {
            ThrowError("error: StringBuilder.Clear: 'native' field not found", Rt::Instance().FilePath(), Span());
        }
    }
    else
    {
        ThrowError("error: StringBuilder.Clear: heap object expected", Rt::Instance().FilePath(), Span());
    }
}

class StringBuilderAppendStrMethod : public ExternalSubroutineSymbol
{
public:
    StringBuilderAppendStrMethod(const soul::ast::Span& span_);
    void Execute(ExecutionContext* context) override;
};

StringBuilderAppendStrMethod::StringBuilderAppendStrMethod(const soul::ast::Span& span_) : ExternalSubroutineSymbol(span_, "StringBuilder.AppendStr", StringBuilderAppendStrId())
{
}

void StringBuilderAppendStrMethod::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> object = stack->Pop();
    Object* obj = object->GetObject();
    if (obj->IsStringObject() || obj->IsValueObject() && static_cast<Value*>(obj)->IsStringValue())
    {
        std::string s = obj->ToString();
        std::unique_ptr<Object> object = stack->Pop();
        Object* obj = object->GetObject();
        if (obj->IsHeapObject())
        {
            HeapObject* stringBuilderObject = static_cast<HeapObject*>(obj);
            ObjectTypeSymbol* objectType = stringBuilderObject->GetType();
            int32_t nativeIndex = objectType->GetFieldIndex("native");
            if (nativeIndex != -1)
            {
                Object* nativeObject = stringBuilderObject->GetField(nativeIndex);
                Object* native = nativeObject->GetObject();
                if (native->IsValueObject())
                {
                    Value* nativeValue = static_cast<Value*>(native);
                    if (nativeValue->IsGenericPointerValue())
                    {
                        GenericPointerValue* nativePtr = static_cast<GenericPointerValue*>(nativeValue);
                        void* np = nativePtr->Pointer();
                        std::string* string = static_cast<std::string*>(np);
                        string->append(s);
                    }
                    else
                    {
                        ThrowError("error: StringBuilder.AppendStr: pointer value expected", Rt::Instance().FilePath(), Span());
                    }
                }
                else
                {
                    ThrowError("error: StringBuilder.AppendStr: value object expected", Rt::Instance().FilePath(), Span());
                }
            }
            else
            {
                ThrowError("error: StringBuilder.AppendStr: 'native' field not found", Rt::Instance().FilePath(), Span());
            }
        }
        else
        {
            ThrowError("error: StringBuilder.AppendStr: heap object expected", Rt::Instance().FilePath(), Span());
        }
    }
    else
    {
        ThrowError("error: StringBuilder.AppendStr: string object expected", Rt::Instance().FilePath(), Span());
    }
}

class StringBuilderAppendCharMethod : public ExternalSubroutineSymbol
{
public:
    StringBuilderAppendCharMethod(const soul::ast::Span& span_);
    void Execute(ExecutionContext* context) override;
};

StringBuilderAppendCharMethod::StringBuilderAppendCharMethod(const soul::ast::Span& span_) : ExternalSubroutineSymbol(span_, "StringBuilder.AppendChar", StringBuilderAppendCharId())
{
}

void StringBuilderAppendCharMethod::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> object = stack->Pop();
    Object* obj = object->GetObject();
    if (obj->IsValueObject())
    {
        char chr = obj->ToChar();
        std::unique_ptr<Object> object = stack->Pop();
        Object* obj = object->GetObject();
        if (obj->IsHeapObject())
        {
            HeapObject* stringBuilderObject = static_cast<HeapObject*>(obj);
            ObjectTypeSymbol* objectType = stringBuilderObject->GetType();
            int32_t nativeIndex = objectType->GetFieldIndex("native");
            if (nativeIndex != -1)
            {
                Object* nativeObject = stringBuilderObject->GetField(nativeIndex);
                Object* native = nativeObject->GetObject();
                if (native->IsValueObject())
                {
                    Value* nativeValue = static_cast<Value*>(native);
                    if (nativeValue->IsGenericPointerValue())
                    {
                        GenericPointerValue* nativePtr = static_cast<GenericPointerValue*>(nativeValue);
                        void* np = nativePtr->Pointer();
                        std::string* string = static_cast<std::string*>(np);
                        string->append(1, chr);
                    }
                    else
                    {
                        ThrowError("error: StringBuilder.AppendChar: pointer value expected", Rt::Instance().FilePath(), Span());
                    }
                }
                else
                {
                    ThrowError("error: StringBuilder.AppendChar: value object expected", Rt::Instance().FilePath(), Span());
                }
            }
            else
            {
                ThrowError("error: StringBuilder.AppendChar: 'native' field not found", Rt::Instance().FilePath(), Span());
            }
        }
        else
        {
            ThrowError("error: StringBuilder.AppendChar: heap object expected", Rt::Instance().FilePath(), Span());
        }
    }
    else
    {
        ThrowError("error: StringBuilder.AppendChar: value object expected", Rt::Instance().FilePath(), Span());
    }
}

class StringBuilderGetStringMethod : public ExternalSubroutineSymbol
{
public:
    StringBuilderGetStringMethod(const soul::ast::Span& span_);
    void Execute(ExecutionContext* context) override;
};

StringBuilderGetStringMethod::StringBuilderGetStringMethod(const soul::ast::Span& span_) : ExternalSubroutineSymbol(span_, "StringBuilder.GetString", StringBuilderGetStringId())
{
}

void StringBuilderGetStringMethod::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    Heap* heap = context->GetHeap();
    std::unique_ptr<Object> object = stack->Pop();
    Object* obj = object->GetObject();
    if (obj->IsHeapObject())
    {
        HeapObject* stringBuilderObject = static_cast<HeapObject*>(obj);
        ObjectTypeSymbol* objectType = stringBuilderObject->GetType();
        int32_t nativeIndex = objectType->GetFieldIndex("native");
        if (nativeIndex != -1)
        {
            Object* nativeObject = stringBuilderObject->GetField(nativeIndex);
            Object* native = nativeObject->GetObject();
            if (native->IsValueObject())
            {
                Value* nativeValue = static_cast<Value*>(native);
                if (nativeValue->IsGenericPointerValue())
                {
                    GenericPointerValue* nativePtr = static_cast<GenericPointerValue*>(nativeValue);
                    void* np = nativePtr->Pointer();
                    std::string* string = static_cast<std::string*>(np);
                    StringObject* stringObject = heap->AllocateString(*string, context);
                    stack->Push(new StringObjectPtr(stringObject));
                }
                else
                {
                    ThrowError("error: StringBuilder.GetString: pointer value expected", Rt::Instance().FilePath(), Span());
                }
            }
            else
            {
                ThrowError("error: StringBuilder.GetString: value object expected", Rt::Instance().FilePath(), Span());
            }
        }
        else
        {
            ThrowError("error: StringBuilder.GetString: 'native' field not found", Rt::Instance().FilePath(), Span());
        }
    }
    else
    {
        ThrowError("error: StringBuilder.GetString: heap object expected", Rt::Instance().FilePath(), Span());
    }
}

class StringBuilderDisposeMethod : public ExternalSubroutineSymbol
{
public:
    StringBuilderDisposeMethod(const soul::ast::Span& span_);
    void Execute(ExecutionContext* context) override;
};

StringBuilderDisposeMethod::StringBuilderDisposeMethod(const soul::ast::Span& span_) : ExternalSubroutineSymbol(span_, "StringBuilder.Dispose", StringBuilderDisposeId())
{
}

void StringBuilderDisposeMethod::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> object = stack->Pop();
    Object* obj = object->GetObject();
    if (obj->IsHeapObject())
    {
        HeapObject* stringBuilderObject = static_cast<HeapObject*>(obj);
        ObjectTypeSymbol* objectType = stringBuilderObject->GetType();
        int32_t nativeIndex = objectType->GetFieldIndex("native");
        if (nativeIndex != -1)
        {
            Object* nativeObject = stringBuilderObject->GetField(nativeIndex);
            Object* native = nativeObject->GetObject();
            if (native->IsValueObject())
            {
                Value* nativeValue = static_cast<Value*>(native);
                if (nativeValue->IsGenericPointerValue())
                {
                    GenericPointerValue* nativePtr = static_cast<GenericPointerValue*>(nativeValue);
                    void* np = nativePtr->Pointer();
                    std::string* string = static_cast<std::string*>(np);
                    if (string)
                    {
                        delete string;
                        GenericPointerValue nullPtr(nullptr);
                        stringBuilderObject->SetField(nativeIndex, &nullPtr, context);
                    }
                }
                else
                {
                    ThrowError("error: StringBuilder.Dispose: pointer value expected", Rt::Instance().FilePath(), Span());
                }
            }
            else
            {
                ThrowError("error: StringBuilder.Dispose: value object expected", Rt::Instance().FilePath(), Span());
            }
        }
        else
        {
            ThrowError("error: StringBuilder.Dispose: 'native' field not found", Rt::Instance().FilePath(), Span());
        }
    }
    else
    {
        ThrowError("error: StringBuilder.Dispose: heap object expected", Rt::Instance().FilePath(), Span());
    }
}

class BitmapGetGraphicsMethod : public ExternalSubroutineSymbol
{
public:
    BitmapGetGraphicsMethod(const soul::ast::Span& span_);
    void Execute(ExecutionContext* context) override;
};

BitmapGetGraphicsMethod::BitmapGetGraphicsMethod(const soul::ast::Span& span_) : ExternalSubroutineSymbol(span_, "Bitmap.GetGraphics", BitmapGetGraphicsId())
{
}

void BitmapGetGraphicsMethod::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> object = stack->Pop();
    Object* obj = object->GetObject();
    int32_t x = -1;
    int32_t y = -1;
    HeapObject* heapObject = nullptr;
    ObjectTypeSymbol* objectType = nullptr;
    if (obj->IsHeapObject())
    {
        heapObject = static_cast<HeapObject*>(obj);
        objectType = heapObject->GetType();
        int32_t x_index = objectType->GetFieldIndex("x");
        if (x_index != -1)
        {
            Object* x_object = heapObject->GetField(x_index);
            if (x_object->IsValueObject())
            {
                Value* x_value = static_cast<Value*>(x_object);
                x = x_value->ToInteger();
            }
            else
            {
                ThrowError("error: Bitmap.GetGraphics: value object expected", Rt::Instance().FilePath(), Span());
            }
        }
        else
        {
            ThrowError("error: Bitmap.GetGraphics: 'x' not found", Rt::Instance().FilePath(), Span());
        }
        int32_t y_index = objectType->GetFieldIndex("y");
        if (y_index != -1)
        {
            Object* y_object = heapObject->GetField(y_index);
            if (y_object->IsValueObject())
            {
                Value* y_value = static_cast<Value*>(y_object);
                y = y_value->ToInteger();
            }
            else
            {
                ThrowError("error: Bitmap.GetGraphics: value object expected", Rt::Instance().FilePath(), Span());
            }
        }
        else
        {
            ThrowError("error: Bitmap.GetGraphics: 'y' not found", Rt::Instance().FilePath(), Span());
        }
    }
    else
    {
        ThrowError("error: Bitmap.GetGraphics: heap object expected", Rt::Instance().FilePath(), Span());
    }
    Gdiplus::Bitmap* bm = new Gdiplus::Bitmap(x, y, PixelFormat32bppARGB);
    int32_t native_index = objectType->GetFieldIndex("native");
    if (native_index != -1)
    {
        GenericPointerValue pointerValue(bm);
        heapObject->SetField(native_index, &pointerValue, context);
    }
    else
    {
        ThrowError("error: Bitmap.GetGraphics: 'native' not found", Rt::Instance().FilePath(), Span());
    }
    SymbolTable* symbolTable = context->GetSymbolTable();
    TypeSymbol* graphicsType = symbolTable->GetType("Graphics", Rt::Instance().FilePath(), Span());
    if (graphicsType->IsObjectTypeSymbol())
    {
        ObjectTypeSymbol* graphicsObjectType = static_cast<ObjectTypeSymbol*>(graphicsType);
        int32_t native_index = graphicsObjectType->GetFieldIndex("native");
        if (native_index != -1)
        {
            Heap* heap = context->GetHeap();
            HeapObject* graphics = heap->Allocate(graphicsObjectType, context);
            Gdiplus::Graphics* nativeGraphics = Gdiplus::Graphics::FromImage(bm);
            nativeGraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintClearTypeGridFit);
            nativeGraphics->SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
            nativeGraphics->SetPageUnit(Gdiplus::UnitMillimeter);
            GenericPointerValue pointerValue(nativeGraphics);
            graphics->SetField(native_index, &pointerValue, context);
            float native_dpiX = nativeGraphics->GetDpiX();
            float native_dpiY = nativeGraphics->GetDpiY();
            int32_t dpix_index = graphicsObjectType->GetFieldIndex("dpiX");
            if (dpix_index != -1)
            {
                RealValue dpiX(native_dpiX);
                graphics->SetField(dpix_index, &dpiX, context);
            }
            else
            {
                ThrowError("error: Bitmap.GetGraphics: 'dpiX' not found", Rt::Instance().FilePath(), Span());
            }
            int32_t dpiy_index = graphicsObjectType->GetFieldIndex("dpiY");
            if (dpiy_index != -1)
            {
                RealValue dpiY(native_dpiY);
                graphics->SetField(dpiy_index, &dpiY, context);
            }
            else
            {
                ThrowError("error: Bitmap.GetGraphics: 'dpiY' not found", Rt::Instance().FilePath(), Span());
            }
            stack->Push(graphics->Clone());
        }
        else
        {
            ThrowError("error: Bitmap.GetGraphics: 'native' not found", Rt::Instance().FilePath(), Span());
        }
    }
    else
    {
        ThrowError("error: Bitmap.GetGraphics: object type expected", Rt::Instance().FilePath(), Span());
    }
}

class BitmapSaveMethod : public ExternalSubroutineSymbol
{
public:
    BitmapSaveMethod(const soul::ast::Span& span_);
    void Execute(ExecutionContext* context) override;
};

BitmapSaveMethod::BitmapSaveMethod(const soul::ast::Span& span_) : ExternalSubroutineSymbol(span_, "Bitmap.Save", BitmapSaveId())
{
}

void BitmapSaveMethod::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> fileName = stack->Pop();
    Object* fileNameObject = fileName->GetObject();
    StringObject* fileNameStringObject = fileNameObject->ToStringObject(context);
    std::string imageFileName = fileNameStringObject->Value();
    std::unique_ptr<Object> bitmap = stack->Pop();
    Object* bitmapObj = bitmap->GetObject();
    if (bitmapObj->IsHeapObject())
    {
        HeapObject* bitmapHeapObject = static_cast<HeapObject*>(bitmapObj);
        ObjectTypeSymbol* bitmapType = bitmapHeapObject->GetType();
        int32_t nativeIndex = bitmapType->GetFieldIndex("native");
        if (nativeIndex != -1)
        {
            Object* native = bitmapHeapObject->GetField(nativeIndex);
            if (native->IsValueObject())
            {
                Value* nativeValue = static_cast<Value*>(native);
                if (nativeValue->IsGenericPointerValue())
                {
                    GenericPointerValue* nativePtr = static_cast<GenericPointerValue*>(nativeValue);
                    void* np = nativePtr->Pointer();
                    Gdiplus::Bitmap* bm = static_cast<Gdiplus::Bitmap*>(np);
                    std::u16string fileName = util::ToUtf16(imageFileName);
                    CLSID guid;
                    ImageFormat imageFormat = ImageFormat::png;
                    int result = GetEncoderClsId(imageFormat, guid);
                    if (result == 0)
                    {
                        Gdiplus::Status status = bm->Save((const WCHAR*)fileName.c_str(), &guid, nullptr);
                        if (status != Gdiplus::Ok)
                        {
                            ThrowError("error: Bitmap.Save: could not save bitmap", Rt::Instance().FilePath(), Span());
                        }
                    }
                    else
                    {
                        ThrowError("error: Bitmap.Save: encoder CLSID not found for encoding '" + GetImageFormatStr(imageFormat) + "'", Rt::Instance().FilePath(), Span());
                    }
                }
                else
                {
                    ThrowError("error: Bitmap.Save: pointer object expected", Rt::Instance().FilePath(), Span());
                }
            }
            else
            {
                ThrowError("error: Bitmap.Save: value object expected", Rt::Instance().FilePath(), Span());
            }
        }
        else
        {
            ThrowError("error: Bitmap.Save: 'native' not found", Rt::Instance().FilePath(), Span());
        }
    }
    else
    {
        ThrowError("error: Bitmap.Save: heap object expected", Rt::Instance().FilePath(), Span());
    }
}

class GraphicsClearMethod : public ExternalSubroutineSymbol
{
public:
    GraphicsClearMethod(const soul::ast::Span& span_);
    void Execute(ExecutionContext* context) override;
};

GraphicsClearMethod::GraphicsClearMethod(const soul::ast::Span& span_) : ExternalSubroutineSymbol(span_, "Graphics.Clear", GraphicsClearId())
{
}

void GraphicsClearMethod::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> colorObject = stack->Pop();
    Object* colorObj = colorObject->GetObject();
    Gdiplus::Color color;
    if (colorObj->IsHeapObject())
    {
        HeapObject* colorObject = static_cast<HeapObject*>(colorObj);
        ObjectTypeSymbol* colorObjectType = colorObject->GetType();
        int32_t a = -1;
        int32_t r = -1;
        int32_t g = -1;
        int32_t b = -1;
        int32_t a_field_index = colorObjectType->GetFieldIndex("a");
        if (a_field_index != -1)
        {
            a = colorObject->GetField(a_field_index)->ToInteger();
        }
        else
        {
            ThrowError("error: Graphics.Clear: 'a' not found", Rt::Instance().FilePath(), Span());
        }
        int32_t r_field_index = colorObjectType->GetFieldIndex("r");
        if (r_field_index != -1)
        {
            r = colorObject->GetField(r_field_index)->ToInteger();
        }
        else
        {
            ThrowError("error: Graphics.Clear: 'r' not found", Rt::Instance().FilePath(), Span());
        }
        int32_t g_field_index = colorObjectType->GetFieldIndex("g");
        if (g_field_index != -1)
        {
            g = colorObject->GetField(g_field_index)->ToInteger();
        }
        else
        {
            ThrowError("error: Graphics.Clear: 'g' not found", Rt::Instance().FilePath(), Span());
        }
        int32_t b_field_index = colorObjectType->GetFieldIndex("b");
        if (b_field_index != -1)
        {
            b = colorObject->GetField(g_field_index)->ToInteger();
        }
        else
        {
            ThrowError("error: Graphics.Clear: 'b' not found", Rt::Instance().FilePath(), Span());
        }
        color = Gdiplus::Color(a, r, g, b);
    }
    std::unique_ptr<Object> graphics = stack->Pop();
    Object* graphicsObj = graphics->GetObject();
    if (graphicsObj->IsHeapObject())
    {
        HeapObject* graphicsObject = static_cast<HeapObject*>(graphicsObj);
        ObjectTypeSymbol* graphicsType = graphicsObject->GetType();
        int32_t nativeIndex = graphicsType->GetFieldIndex("native");
        if (nativeIndex != -1)
        {
            Object* native = graphicsObject->GetField(nativeIndex);
            if (native->IsValueObject())
            {
                Value* nativeValue = static_cast<Value*>(native);
                if (nativeValue->IsGenericPointerValue())
                {
                    GenericPointerValue* nativePtr = static_cast<GenericPointerValue*>(nativeValue);
                    void* np = nativePtr->Pointer();
                    Gdiplus::Graphics* graphics = static_cast<Gdiplus::Graphics*>(np);
                    Gdiplus::Status status = graphics->Clear(color);
                    if (status != Gdiplus::Ok)
                    {
                        ThrowError("error: Graphics.Clear: could not clear", Rt::Instance().FilePath(), Span());
                    }
                }
            }
        }
        else
        {
            ThrowError("error: Graphics.Clear: 'native' not found", Rt::Instance().FilePath(), Span());
        }
    }
    else
    {
        ThrowError("error: Graphics.Clear: heap object expected", Rt::Instance().FilePath(), Span());
    }
}

class GraphicsDrawLineMethod : public ExternalSubroutineSymbol
{
public:
    GraphicsDrawLineMethod(const soul::ast::Span& span_);
    void Execute(ExecutionContext* context) override;
};

GraphicsDrawLineMethod::GraphicsDrawLineMethod(const soul::ast::Span& span_) : ExternalSubroutineSymbol(span_, "Graphics.DrawLine", GraphicsDrawLineId())
{
}

void GraphicsDrawLineMethod::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> point2 = stack->Pop();
    std::unique_ptr<Object> point1 = stack->Pop();
    std::unique_ptr<Object> pen = stack->Pop();
    std::unique_ptr<Object> graphics = stack->Pop();
    Object* penObj = pen->GetObject();
    if (penObj->IsHeapObject())
    {
        HeapObject* penHeapObject = static_cast<HeapObject*>(penObj);
        ObjectTypeSymbol* penObjectType = penHeapObject->GetType();
        int32_t colorIndex = penObjectType->GetFieldIndex("color");
        if (colorIndex != -1)
        {
            Object* color = penHeapObject->GetField(colorIndex);
            Object* colorObj = color->GetObject();
            if (colorObj->IsHeapObject())
            {
                HeapObject* colorObject = static_cast<HeapObject*>(colorObj);
                ObjectTypeSymbol* colorObjectType = colorObject->GetType();
                int32_t a = -1;
                int32_t r = -1;
                int32_t g = -1;
                int32_t b = -1;
                int32_t a_field_index = colorObjectType->GetFieldIndex("a");
                if (a_field_index != -1)
                {
                    a = colorObject->GetField(a_field_index)->ToInteger();
                }
                else
                {
                    ThrowError("error: Graphics.DrawLine: 'a' not found", Rt::Instance().FilePath(), Span());
                }
                int32_t r_field_index = colorObjectType->GetFieldIndex("r");
                if (r_field_index != -1)
                {
                    r = colorObject->GetField(r_field_index)->ToInteger();
                }
                else
                {
                    ThrowError("error: Graphics.DrawLine: 'r' not found", Rt::Instance().FilePath(), Span());
                }
                int32_t g_field_index = colorObjectType->GetFieldIndex("g");
                if (g_field_index != -1)
                {
                    g = colorObject->GetField(g_field_index)->ToInteger();
                }
                else
                {
                    ThrowError("error: Graphics.DrawLine: 'g' not found", Rt::Instance().FilePath(), Span());
                }
                int32_t b_field_index = colorObjectType->GetFieldIndex("b");
                if (b_field_index != -1)
                {
                    b = colorObject->GetField(g_field_index)->ToInteger();
                }
                else
                {
                    ThrowError("error: Graphics.DrawLine: 'b' not found", Rt::Instance().FilePath(), Span());
                }
                Gdiplus::Color color(a, r, g, b);
                float width = 0;
                int32_t width_index = penObjectType->GetFieldIndex("width");
                if (width_index != -1)
                {
                    Object* widthObject = penHeapObject->GetField(width_index);
                    if (widthObject->IsValueObject())
                    {
                        Value* widthValue = static_cast<Value*>(widthObject);
                        if (widthValue->IsRealValue())
                        {
                            width = widthValue->ToReal();
                        }
                    }
                }
                else
                {
                    ThrowError("error: Graphics.DrawLine: 'width' not found", Rt::Instance().FilePath(), Span());
                }
                Gdiplus::Pen* pen = Rt::Instance().GetPen(PenKey(color, width));
                Gdiplus::PointF p1;
                Gdiplus::PointF p2;
                float x = 0;
                float y = 0;
                int32_t x_index = -1;
                int32_t y_index = -1;
                Object* point1Obj = point1->GetObject();
                if (point1Obj->IsHeapObject())
                {
                    HeapObject* point1Object = static_cast<HeapObject*>(point1Obj);
                    ObjectTypeSymbol* point1ObjectType = point1Object->GetType();
                    x_index = point1ObjectType->GetFieldIndex("x");
                    if (x_index != -1)
                    {
                        x = point1Object->GetField(x_index)->ToReal();
                    }
                    else
                    {
                        ThrowError("error: Graphics.DrawLine: 'x' not found", Rt::Instance().FilePath(), Span());
                    }
                    y_index = point1ObjectType->GetFieldIndex("y");
                    if (y_index != -1)
                    {
                        y = point1Object->GetField(y_index)->ToReal();
                    }
                    else
                    {
                        ThrowError("error: Graphics.DrawLine: 'y' not found", Rt::Instance().FilePath(), Span());
                    }
                    p1 = Gdiplus::PointF(x, y);
                }
                Object* point2Obj = point2->GetObject();
                if (point2Obj->IsHeapObject())
                {
                    HeapObject* point2Object = static_cast<HeapObject*>(point2Obj);
                    x = point2Object->GetField(x_index)->ToReal();
                    y = point2Object->GetField(y_index)->ToReal();
                    p2 = Gdiplus::PointF(x, y);
                }
                Object* graphicsObj = graphics->GetObject();
                if (graphicsObj->IsHeapObject())
                {
                    HeapObject* graphicsObject = static_cast<HeapObject*>(graphicsObj);
                    ObjectTypeSymbol* graphicsType = graphicsObject->GetType();
                    int32_t nativeIndex = graphicsType->GetFieldIndex("native");
                    if (nativeIndex != -1)
                    {
                        Object* native = graphicsObject->GetField(nativeIndex);
                        if (native->IsValueObject())
                        {
                            Value* nativeValue = static_cast<Value*>(native);
                            if (nativeValue->IsGenericPointerValue())
                            {
                                GenericPointerValue* nativePtr = static_cast<GenericPointerValue*>(nativeValue);
                                void* np = nativePtr->Pointer();
                                Gdiplus::Graphics* graphics = static_cast<Gdiplus::Graphics*>(np);
                                Gdiplus::Status status = graphics->DrawLine(pen, p1, p2);
                                if (status != Gdiplus::Ok)
                                {
                                    ThrowError("error: Graphics.DrawLine: cound not draw line", Rt::Instance().FilePath(), Span());
                                }
                            }
                        }
                    }
                    else
                    {
                        ThrowError("error: Graphics.DrawLine: 'native' not found", Rt::Instance().FilePath(), Span());
                    }
                }
                else
                {
                    ThrowError("error: Graphics.DrawLine: heap object expected", Rt::Instance().FilePath(), Span());
                }
            }
            else
            {
                ThrowError("error: Graphics.DrawLine: heap object expected", Rt::Instance().FilePath(), Span());
            }
        }
        else
        {
            ThrowError("error: Graphics.DrawLine: 'color' not found", Rt::Instance().FilePath(), Span());
        }
    }
    else
    {
        ThrowError("error: Graphics.DrawLine: heap object expected", Rt::Instance().FilePath(), Span());
    }
}

class GraphicsDrawRectangleMethod : public ExternalSubroutineSymbol
{
public:
    GraphicsDrawRectangleMethod(const soul::ast::Span& span_);
    void Execute(ExecutionContext* context) override;
};

GraphicsDrawRectangleMethod::GraphicsDrawRectangleMethod(const soul::ast::Span& span_) : ExternalSubroutineSymbol(span_, "Graphics.DrawRectangle", GraphicsDrawRectangleId())
{
}

void GraphicsDrawRectangleMethod::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> rectObject = stack->Pop();
    std::unique_ptr<Object> penObject = stack->Pop();
    std::unique_ptr<Object> graphics = stack->Pop();
    Object* penObj = penObject->GetObject();
    Gdiplus::Pen* pen = nullptr;
    if (penObj->IsHeapObject())
    {
        HeapObject* penHeapObject = static_cast<HeapObject*>(penObj);
        ObjectTypeSymbol* penObjectType = penHeapObject->GetType();
        int32_t colorIndex = penObjectType->GetFieldIndex("color");
        if (colorIndex != -1)
        {
            Object* color = penHeapObject->GetField(colorIndex);
            Object* colorObj = color->GetObject();
            if (colorObj->IsHeapObject())
            {
                HeapObject* colorObject = static_cast<HeapObject*>(colorObj);
                ObjectTypeSymbol* colorObjectType = colorObject->GetType();
                int32_t a = -1;
                int32_t r = -1;
                int32_t g = -1;
                int32_t b = -1;
                int32_t a_field_index = colorObjectType->GetFieldIndex("a");
                if (a_field_index != -1)
                {
                    a = colorObject->GetField(a_field_index)->ToInteger();
                }
                else
                {
                    ThrowError("error: Graphics.DrawRectangle: 'a' not found", Rt::Instance().FilePath(), Span());
                }
                int32_t r_field_index = colorObjectType->GetFieldIndex("r");
                if (r_field_index != -1)
                {
                    r = colorObject->GetField(r_field_index)->ToInteger();
                }
                else
                {
                    ThrowError("error: Graphics.DrawRectangle: 'r' not found", Rt::Instance().FilePath(), Span());
                }
                int32_t g_field_index = colorObjectType->GetFieldIndex("g");
                if (g_field_index != -1)
                {
                    g = colorObject->GetField(g_field_index)->ToInteger();
                }
                else
                {
                    ThrowError("error: Graphics.DrawRectangle: 'g' not found", Rt::Instance().FilePath(), Span());
                }
                int32_t b_field_index = colorObjectType->GetFieldIndex("b");
                if (b_field_index != -1)
                {
                    b = colorObject->GetField(g_field_index)->ToInteger();
                }
                else
                {
                    ThrowError("error: Graphics.DrawRectangle: 'b' not found", Rt::Instance().FilePath(), Span());
                }
                Gdiplus::Color color(a, r, g, b);
                float width = 0;
                int32_t width_index = penObjectType->GetFieldIndex("width");
                if (width_index != -1)
                {
                    Object* widthObject = penHeapObject->GetField(width_index);
                    if (widthObject->IsValueObject())
                    {
                        Value* widthValue = static_cast<Value*>(widthObject);
                        if (widthValue->IsRealValue())
                        {
                            width = widthValue->ToReal();
                        }
                    }
                }
                else
                {
                    ThrowError("error: Graphics.DrawRectangle: 'width' not found", Rt::Instance().FilePath(), Span());
                }
                pen = Rt::Instance().GetPen(PenKey(color, width));
            }
        }
    }
    float x = 0;
    float y = 0;
    float w = 0;
    float h = 0;
    Object* rectObj = rectObject->GetObject();
    if (rectObj->IsHeapObject())
    {
        HeapObject* rectHeapObject = static_cast<HeapObject*>(rectObj);
        ObjectTypeSymbol* rectObjectType = rectHeapObject->GetType();
        int32_t location_index = rectObjectType->GetFieldIndex("location");
        if (location_index != -1)
        {
            Object* locationObject = rectHeapObject->GetField(location_index);
            Object* locationObj = locationObject->GetObject();
            if (locationObj->IsHeapObject())
            {
                HeapObject* locationHeapObject = static_cast<HeapObject*>(locationObj);
                ObjectTypeSymbol* locationType = locationHeapObject->GetType();
                int32_t x_index = locationType->GetFieldIndex("x");
                if (x_index != -1)
                {
                    Object* xObject = locationHeapObject->GetField(x_index);
                    Object* xObj = xObject->GetObject();
                    if (xObj->IsValueObject())
                    {
                        Value* xValue = static_cast<Value*>(xObj);
                        x = xValue->ToReal();
                    }
                    else
                    {
                        ThrowError("error: Graphics.DrawRectangle: value object expected", Rt::Instance().FilePath(), Span());
                    }
                }
                else
                {
                    ThrowError("error: Graphics.DrawRectangle: 'x' not found", Rt::Instance().FilePath(), Span());
                }
                int32_t y_index = locationType->GetFieldIndex("y");
                if (y_index != -1)
                {
                    Object* yObject = locationHeapObject->GetField(y_index);
                    Object* yObj = yObject->GetObject();
                    if (yObj->IsValueObject())
                    {
                        Value* yValue = static_cast<Value*>(yObj);
                        y = yValue->ToReal();
                    }
                    else
                    {
                        ThrowError("error: Graphics.DrawRectangle: value object expected", Rt::Instance().FilePath(), Span());
                    }
                }
                else
                {
                    ThrowError("error: Graphics.DrawRectangle: 'y' not found", Rt::Instance().FilePath(), Span());
                }
            }
        }
        int32_t size_index = rectObjectType->GetFieldIndex("size");
        if (size_index != -1)
        {
            Object* sizeObject = rectHeapObject->GetField(size_index);
            Object* sizeObj = sizeObject->GetObject();
            if (sizeObj->IsHeapObject())
            {
                HeapObject* sizeHeapObject = static_cast<HeapObject*>(sizeObj);
                ObjectTypeSymbol* sizeType = sizeHeapObject->GetType();
                int32_t w_index = sizeType->GetFieldIndex("w");
                if (w_index != -1)
                {
                    Object* wObject = sizeHeapObject->GetField(w_index);
                    Object* wObj = wObject->GetObject();
                    if (wObj->IsValueObject())
                    {
                        Value* wValue = static_cast<Value*>(wObj);
                        w = wValue->ToReal();
                    }
                    else
                    {
                        ThrowError("error: Graphics.DrawRectangle: value object expected", Rt::Instance().FilePath(), Span());
                    }
                }
                else
                {
                    ThrowError("error: Graphics.DrawRectangle: 'w' not found", Rt::Instance().FilePath(), Span());
                }
                int32_t h_index = sizeType->GetFieldIndex("h");
                if (h_index != -1)
                {
                    Object* hObject = sizeHeapObject->GetField(h_index);
                    Object* hObj = hObject->GetObject();
                    if (hObj->IsValueObject())
                    {
                        Value* hValue = static_cast<Value*>(hObj);
                        h = hValue->ToReal();
                    }
                    else
                    {
                        ThrowError("error: Graphics.DrawRectangle: value object expected", Rt::Instance().FilePath(), Span());
                    }
                }
                else
                {
                    ThrowError("error: Graphics.DrawRectangle: 'h' not found", Rt::Instance().FilePath(), Span());
                }
            }
        }
        else
        {
            ThrowError("error: Graphics.DrawRectangle: 'size' not found", Rt::Instance().FilePath(), Span());
        }
    }
    Gdiplus::RectF rect(x, y, w, h);
    Object* graphicsObj = graphics->GetObject();
    if (graphicsObj->IsHeapObject())
    {
        HeapObject* graphicsObject = static_cast<HeapObject*>(graphicsObj);
        ObjectTypeSymbol* graphicsType = graphicsObject->GetType();
        int32_t nativeIndex = graphicsType->GetFieldIndex("native");
        if (nativeIndex != -1)
        {
            Object* native = graphicsObject->GetField(nativeIndex);
            if (native->IsValueObject())
            {
                Value* nativeValue = static_cast<Value*>(native);
                if (nativeValue->IsGenericPointerValue())
                {
                    GenericPointerValue* nativePtr = static_cast<GenericPointerValue*>(nativeValue);
                    void* np = nativePtr->Pointer();
                    Gdiplus::Graphics* graphics = static_cast<Gdiplus::Graphics*>(np);
                    Gdiplus::Status status = graphics->DrawRectangle(pen, rect);
                    if (status != Gdiplus::Ok)
                    {
                        ThrowError("error: Graphics.DrawRectangle: could not draw rectangle", Rt::Instance().FilePath(), Span());
                    }
                }
            }
        }
        else
        {
            ThrowError("error: Graphics.DrawRectangle: 'native' not found", Rt::Instance().FilePath(), Span());
        }
    }
    else
    {
        ThrowError("error: Graphics.DrawRectangle: heap object expected", Rt::Instance().FilePath(), Span());
    }
}

class GraphicsMeasureStringMethod : public ExternalSubroutineSymbol
{
public:
    GraphicsMeasureStringMethod(const soul::ast::Span& span_);
    void Execute(ExecutionContext* context) override;
};

GraphicsMeasureStringMethod::GraphicsMeasureStringMethod(const soul::ast::Span& span_) : ExternalSubroutineSymbol(span_, "Graphics.MeasureString", GraphicsMeasureStringId())
{
}

void GraphicsMeasureStringMethod::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> fontObject = stack->Pop();
    std::unique_ptr<Object> textObject = stack->Pop();
    std::unique_ptr<Object> graphicsObject = stack->Pop();
    Object* fontObj = fontObject->GetObject();
    Gdiplus::Font* font = nullptr;
    std::string familyName;
    if (fontObj->IsHeapObject())
    {
        HeapObject* fontHeapObject = static_cast<HeapObject*>(fontObj);
        ObjectTypeSymbol* fontType = fontHeapObject->GetType();
        int32_t family_index = fontType->GetFieldIndex("family");
        if (family_index != -1)
        {
            Object* familyObject = fontHeapObject->GetField(family_index);
            Object* familyObj = familyObject->GetObject();
            if (familyObj->IsStringObject())
            {
                StringObject* stringObject = static_cast<StringObject*>(familyObj);
                familyName = stringObject->Value();
            }
            else
            {
                ThrowError("error: Graphics.MeasureString: string object expected", Rt::Instance().FilePath(), Span());
            }
        }
        else
        {
            ThrowError("error: Graphics.MeasureString: 'family' not found", Rt::Instance().FilePath(), Span());
        }
        int32_t size_index = fontType->GetFieldIndex("size");
        float size = 0;
        if (size_index != -1)
        {
            Object* sizeObject = fontHeapObject->GetField(size_index);
            if (sizeObject->IsValueObject())
            {
                Value* sizeValue = static_cast<Value*>(sizeObject);
                size = sizeValue->ToReal();
            }
            else
            {
                ThrowError("error: Graphics.MeasureString: value object expected", Rt::Instance().FilePath(), Span());
            }
        }
        else
        {
            ThrowError("error: Graphics.MeasureString: 'size' not found", Rt::Instance().FilePath(), Span());
        }
        int32_t style_index = fontType->GetFieldIndex("style");
        int32_t style = 0;
        if (style_index != -1)
        {
            Object* styleObject = fontHeapObject->GetField(style_index);
            if (styleObject->IsValueObject())
            {
                Value* styleValue = static_cast<Value*>(styleObject);
                style = styleValue->ToInteger();
            }
            else
            {
                ThrowError("error: Graphics.MeasureString: value object expected", Rt::Instance().FilePath(), Span());
            }
        }
        else
        {
            ThrowError("error: Graphics.MeasureString: 'style' not found", Rt::Instance().FilePath(), Span());
        }
        FontKey fontKey(familyName, size, style);
        font = Rt::Instance().GetFont(fontKey);
        Object* textObj = textObject->GetObject();
        std::u16string text = util::ToUtf16(textObj->ToStringObject(context)->Value());
        Object* graphicsObj = graphicsObject->GetObject();
        if (graphicsObj->IsHeapObject())
        {
            HeapObject* graphicsObject = static_cast<HeapObject*>(graphicsObj);
            ObjectTypeSymbol* graphicsType = graphicsObject->GetType();
            int32_t nativeIndex = graphicsType->GetFieldIndex("native");
            if (nativeIndex != -1)
            {
                Object* native = graphicsObject->GetField(nativeIndex);
                if (native->IsValueObject())
                {
                    Value* nativeValue = static_cast<Value*>(native);
                    if (nativeValue->IsGenericPointerValue())
                    {
                        GenericPointerValue* nativePtr = static_cast<GenericPointerValue*>(nativeValue);
                        void* np = nativePtr->Pointer();
                        Gdiplus::Graphics* graphics = static_cast<Gdiplus::Graphics*>(np);
                        Gdiplus::PointF origin(0, 0);
                        Gdiplus::RectF rect;
                        Gdiplus::Status status = graphics->MeasureString((const WCHAR*)text.c_str(), -1, font, origin, &rect);
                        if (status == Gdiplus::Ok)
                        {
                            float w = rect.Width;
                            float h = rect.Height;
                            SymbolTable* symbolTable = context->GetSymbolTable();
                            TypeSymbol* sizeType = symbolTable->GetType("Size", Rt::Instance().FilePath(), Span());
                            if (sizeType && sizeType->IsObjectTypeSymbol())
                            {
                                ObjectTypeSymbol* sizeObjectType = static_cast<ObjectTypeSymbol*>(sizeType);
                                Heap* heap = context->GetHeap();
                                HeapObject* sizeObject = heap->Allocate(sizeObjectType, context);
                                int32_t w_index = sizeObjectType->GetFieldIndex("w");
                                if (w_index != -1)
                                {
                                    RealValue wValue(w);
                                    sizeObject->SetField(w_index, &wValue, context);
                                }
                                else
                                {
                                    ThrowError("error: Graphics.MeasureString: 'w' not found", Rt::Instance().FilePath(), Span());
                                }
                                int32_t h_index = sizeObjectType->GetFieldIndex("h");
                                if (h_index != -1)
                                {
                                    RealValue hValue(h);
                                    sizeObject->SetField(h_index, &hValue, context);
                                }
                                else
                                {
                                    ThrowError("error: Graphics.MeasureString: 'h' not found", Rt::Instance().FilePath(), Span());
                                }
                                stack->Push(sizeObject->Clone());
                            }
                        }
                        else
                        {
                            ThrowError("error: Graphics.MeasureString: could not measure string", Rt::Instance().FilePath(), Span());
                        }
                    }
                }
            }
            else
            {
                ThrowError("error: Graphics.MeasureString: 'native' not found", Rt::Instance().FilePath(), Span());
            }
        }
        else
        {
            ThrowError("error: Graphics.MeasureString: heap object expected", Rt::Instance().FilePath(), Span());
        }
    }
    else
    {
        ThrowError("error: Graphics.MeasureString: heap object expected", Rt::Instance().FilePath(), Span());
    }
}

class GraphicsDrawStringMethod : public ExternalSubroutineSymbol
{
public:
    GraphicsDrawStringMethod(const soul::ast::Span& span_);
    void Execute(ExecutionContext* context) override;
};

GraphicsDrawStringMethod::GraphicsDrawStringMethod(const soul::ast::Span& span_) : ExternalSubroutineSymbol(span_, "Graphics.DrawString", GraphicsDrawStringId())
{
}

void GraphicsDrawStringMethod::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> locationObject = stack->Pop();
    std::unique_ptr<Object> brushObject = stack->Pop();
    std::unique_ptr<Object> fontObject = stack->Pop();
    std::unique_ptr<Object> textObject = stack->Pop();
    std::unique_ptr<Object> graphicsObject = stack->Pop();
    Object* locationObj = locationObject->GetObject();
    float x = 0;
    float y = 0;
    Gdiplus::Brush* brush = nullptr;
    if (locationObj->IsHeapObject())
    {
        HeapObject* locationHeapObject = static_cast<HeapObject*>(locationObj);
        ObjectTypeSymbol* locationType = locationHeapObject->GetType();
        int32_t x_index = locationType->GetFieldIndex("x");
        if (x_index != -1)
        {
            Object* xObjext = locationHeapObject->GetField(x_index);
            Object* xObj = xObjext->GetObject();
            if (xObj->IsValueObject())
            {
                Value* xValue = static_cast<Value*>(xObj);
                x = xValue->ToReal();
            }
            else
            {
                ThrowError("error: Graphics.DrawString: value object expected", Rt::Instance().FilePath(), Span());
            }
        }
        else
        {
            ThrowError("error: Graphics.DrawString: 'x' not found", Rt::Instance().FilePath(), Span());
        }
        int32_t y_index = locationType->GetFieldIndex("y");
        if (y_index != -1)
        {
            Object* yObjext = locationHeapObject->GetField(y_index);
            Object* yObj = yObjext->GetObject();
            if (yObj->IsValueObject())
            {
                Value* yValue = static_cast<Value*>(yObj);
                y = yValue->ToReal();
            }
            else
            {
                ThrowError("error: Graphics.DrawString: value object expected", Rt::Instance().FilePath(), Span());
            }
        }
        else
        {
            ThrowError("error: Graphics.DrawString: 'y' not found", Rt::Instance().FilePath(), Span());
        }
    }
    else
    {
        ThrowError("error: Graphics.DrawString: heap object expected", Rt::Instance().FilePath(), Span());
    }
    Object* brushObj = brushObject->GetObject();
    if (brushObj->IsHeapObject())
    {
        HeapObject* heapBrushObject = static_cast<HeapObject*>(brushObj);
        ObjectTypeSymbol* brushType = heapBrushObject->GetType();
        int32_t color_index = brushType->GetFieldIndex("color");
        if (color_index != -1)
        {
            Object* color = heapBrushObject->GetField(color_index);
            Object* colorObj = color->GetObject();
            if (colorObj->IsHeapObject())
            {
                HeapObject* colorObject = static_cast<HeapObject*>(colorObj);
                ObjectTypeSymbol* colorObjectType = colorObject->GetType();
                int32_t a = -1;
                int32_t r = -1;
                int32_t g = -1;
                int32_t b = -1;
                int32_t a_field_index = colorObjectType->GetFieldIndex("a");
                if (a_field_index != -1)
                {
                    a = colorObject->GetField(a_field_index)->ToInteger();
                }
                else
                {
                    ThrowError("error: Graphics.DrawString: 'a' not found", Rt::Instance().FilePath(), Span());
                }
                int32_t r_field_index = colorObjectType->GetFieldIndex("r");
                if (r_field_index != -1)
                {
                    r = colorObject->GetField(r_field_index)->ToInteger();
                }
                else
                {
                    ThrowError("error: Graphics.DrawString: 'r' not found", Rt::Instance().FilePath(), Span());
                }
                int32_t g_field_index = colorObjectType->GetFieldIndex("g");
                if (g_field_index != -1)
                {
                    g = colorObject->GetField(g_field_index)->ToInteger();
                }
                else
                {
                    ThrowError("error: Graphics.DrawString: 'g' not found", Rt::Instance().FilePath(), Span());
                }
                int32_t b_field_index = colorObjectType->GetFieldIndex("b");
                if (b_field_index != -1)
                {
                    b = colorObject->GetField(g_field_index)->ToInteger();
                }
                else
                {
                    ThrowError("error: Graphics.DrawString: 'b' not found", Rt::Instance().FilePath(), Span());
                }
                Gdiplus::Color color(a, r, g, b);
                brush = Rt::Instance().GetBrush(color);
            }
            else
            {
                ThrowError("error: Graphics.DrawString: heap object expected", Rt::Instance().FilePath(), Span());
            }
        }
        else
        {
            ThrowError("error: Graphics.DrawString: 'color' not found", Rt::Instance().FilePath(), Span());
        }
    }
    else
    {
        ThrowError("error: Graphics.DrawString: heap object expected", Rt::Instance().FilePath(), Span());
    }
    Object* fontObj = fontObject->GetObject();
    Gdiplus::Font* font = nullptr;
    if (fontObj->IsHeapObject())
    {
        HeapObject* fontHeapObject = static_cast<HeapObject*>(fontObj);
        std::string familyName;
        ObjectTypeSymbol* fontType = fontHeapObject->GetType();
        int32_t family_index = fontType->GetFieldIndex("family");
        if (family_index != -1)
        {
            Object* familyObject = fontHeapObject->GetField(family_index);
            Object* familyObj = familyObject->GetObject();
            if (familyObj->IsStringObject())
            {
                StringObject* stringObject = static_cast<StringObject*>(familyObj);
                familyName = stringObject->Value();
            }
            else
            {
                ThrowError("error: Graphics.DrawString: string object expected", Rt::Instance().FilePath(), Span());
            }
        }
        else
        {
            ThrowError("error: Graphics.DrawString: 'family' not found", Rt::Instance().FilePath(), Span());
        }
        int32_t size_index = fontType->GetFieldIndex("size");
        float size = 0;
        if (size_index != -1)
        {
            Object* sizeObject = fontHeapObject->GetField(size_index);
            if (sizeObject->IsValueObject())
            {
                Value* sizeValue = static_cast<Value*>(sizeObject);
                size = sizeValue->ToReal();
            }
            else
            {
                ThrowError("error: Graphics.DrawString: value object expected", Rt::Instance().FilePath(), Span());
            }
        }
        else
        {
            ThrowError("error: Graphics.DrawString: 'size' not found", Rt::Instance().FilePath(), Span());
        }
        int32_t style_index = fontType->GetFieldIndex("style");
        int32_t style = 0;
        if (style_index != -1)
        {
            Object* styleObject = fontHeapObject->GetField(style_index);
            if (styleObject->IsValueObject())
            {
                Value* styleValue = static_cast<Value*>(styleObject);
                style = styleValue->ToInteger();
            }
            else
            {
                ThrowError("error: Graphics.DrawString: value object expected", Rt::Instance().FilePath(), Span());
            }
        }
        else
        {
            ThrowError("error: Graphics.DrawString: 'style' not found", Rt::Instance().FilePath(), Span());
        }
        FontKey fontKey(familyName, size, style);
        font = Rt::Instance().GetFont(fontKey);
    }
    Object* textObj = textObject->GetObject();
    std::u16string text = util::ToUtf16(textObj->ToStringObject(context)->Value());
    Object* graphicsObj = graphicsObject->GetObject();
    if (graphicsObj->IsHeapObject())
    {
        HeapObject* graphicsObject = static_cast<HeapObject*>(graphicsObj);
        ObjectTypeSymbol* graphicsType = graphicsObject->GetType();
        int32_t nativeIndex = graphicsType->GetFieldIndex("native");
        if (nativeIndex != -1)
        {
            Object* native = graphicsObject->GetField(nativeIndex);
            if (native->IsValueObject())
            {
                Value* nativeValue = static_cast<Value*>(native);
                if (nativeValue->IsGenericPointerValue())
                {
                    GenericPointerValue* nativePtr = static_cast<GenericPointerValue*>(nativeValue);
                    void* np = nativePtr->Pointer();
                    Gdiplus::Graphics* graphics = static_cast<Gdiplus::Graphics*>(np);
                    Gdiplus::PointF origin(x, y);
                    Gdiplus::Status status = graphics->DrawString((const WCHAR*)text.c_str(), -1, font, origin, brush);
                    if (status != Gdiplus::Ok)
                    {
                        ThrowError("error: Graphics.DrawString: could not draw string", Rt::Instance().FilePath(), Span());
                    }
                }
            }
        }
    }
}

class GraphicsFillPolygonMethod : public ExternalSubroutineSymbol
{
public:
    GraphicsFillPolygonMethod(const soul::ast::Span& span_);
    void Execute(ExecutionContext* context) override;
};

GraphicsFillPolygonMethod::GraphicsFillPolygonMethod(const soul::ast::Span& span_) : ExternalSubroutineSymbol(span_, "Graphics.FillPolygon", GraphicsFillPolygonId())
{
}

void GraphicsFillPolygonMethod::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> pointsObject = stack->Pop();
    std::unique_ptr<Object> brushObject = stack->Pop();
    std::unique_ptr<Object> graphicsObject = stack->Pop();
    Object* pointsObj = pointsObject->GetObject();
    std::vector<Gdiplus::PointF> points;
    Gdiplus::Brush* brush = nullptr;
    if (pointsObj->IsArrayObject())
    {
        ArrayObject* arrayObject = static_cast<ArrayObject*>(pointsObj);
        int32_t length = arrayObject->Length();
        for (int32_t i = 0; i < length; ++i)
        {
            Object* pointObject = arrayObject->GetElement(i);
            Object* pointObj = pointObject->GetObject();
            if (pointObj->IsHeapObject())
            {
                HeapObject* pointHeapObject = static_cast<HeapObject*>(pointObj);
                ObjectTypeSymbol* pointType = pointHeapObject->GetType();
                float x = 0;
                float y = 0;
                int32_t x_index = pointType->GetFieldIndex("x");
                if (x_index != -1)
                {
                    Object* xObject = pointHeapObject->GetField(x_index);
                    Object* xObj = xObject->GetObject();
                    if (xObj->IsValueObject())
                    {
                        Value* xValue = static_cast<Value*>(xObj);
                        x = xValue->ToReal();
                    }
                    else
                    {
                        ThrowError("error: Graphics.FillPolygon: value object expected", Rt::Instance().FilePath(), Span());
                    }
                }
                else
                {
                    ThrowError("error: Graphics.FillPolygon: 'x' not found", Rt::Instance().FilePath(), Span());
                }
                int32_t y_index = pointType->GetFieldIndex("y");
                if (y_index != -1)
                {
                    Object* yObject = pointHeapObject->GetField(y_index);
                    Object* yObj = yObject->GetObject();
                    if (yObj->IsValueObject())
                    {
                        Value* yValue = static_cast<Value*>(yObj);
                        y = yValue->ToReal();
                    }
                    else
                    {
                        ThrowError("error: Graphics.FillPolygon: value object expected", Rt::Instance().FilePath(), Span());
                    }
                }
                else
                {
                    ThrowError("error: Graphics.FillPolygon: 'y' not found", Rt::Instance().FilePath(), Span());
                }
                points.push_back(Gdiplus::PointF(x, y));
            }
            else
            {
                ThrowError("error: Graphics.FillPolygon: heap object expected", Rt::Instance().FilePath(), Span());
            }
        }
    }
    else
    {
        ThrowError("error: Graphics.FillPolygon: array object expected", Rt::Instance().FilePath(), Span());
    }
    Object* brushObj = brushObject->GetObject();
    if (brushObj->IsHeapObject())
    {
        HeapObject* heapBrushObject = static_cast<HeapObject*>(brushObj);
        ObjectTypeSymbol* brushType = heapBrushObject->GetType();
        int32_t color_index = brushType->GetFieldIndex("color");
        if (color_index != -1)
        {
            Object* color = heapBrushObject->GetField(color_index);
            Object* colorObj = color->GetObject();
            if (colorObj->IsHeapObject())
            {
                HeapObject* colorObject = static_cast<HeapObject*>(colorObj);
                ObjectTypeSymbol* colorObjectType = colorObject->GetType();
                int32_t a = -1;
                int32_t r = -1;
                int32_t g = -1;
                int32_t b = -1;
                int32_t a_field_index = colorObjectType->GetFieldIndex("a");
                if (a_field_index != -1)
                {
                    a = colorObject->GetField(a_field_index)->ToInteger();
                }
                else
                {
                    ThrowError("error: Graphics.FillPolygon: 'a' not found", Rt::Instance().FilePath(), Span());
                }
                int32_t r_field_index = colorObjectType->GetFieldIndex("r");
                if (r_field_index != -1)
                {
                    r = colorObject->GetField(r_field_index)->ToInteger();
                }
                else
                {
                    ThrowError("error: Graphics.FillPolygon: 'r' not found", Rt::Instance().FilePath(), Span());
                }
                int32_t g_field_index = colorObjectType->GetFieldIndex("g");
                if (g_field_index != -1)
                {
                    g = colorObject->GetField(g_field_index)->ToInteger();
                }
                else
                {
                    ThrowError("error: Graphics.FillPolygon: 'g' not found", Rt::Instance().FilePath(), Span());
                }
                int32_t b_field_index = colorObjectType->GetFieldIndex("b");
                if (b_field_index != -1)
                {
                    b = colorObject->GetField(g_field_index)->ToInteger();
                }
                else
                {
                    ThrowError("error: Graphics.FillPolygon: 'b' not found", Rt::Instance().FilePath(), Span());
                }
                Gdiplus::Color color(a, r, g, b);
                brush = Rt::Instance().GetBrush(color);
            }
            else
            {
                ThrowError("error: Graphics.FillPolygon: heap object expected", Rt::Instance().FilePath(), Span());
            }
        }
        else
        {
            ThrowError("error: Graphics.FillPolygon: 'color' not found", Rt::Instance().FilePath(), Span());
        }
    }
    else
    {
        ThrowError("error: Graphics.FillPolygon: heap object expected", Rt::Instance().FilePath(), Span());
    }
    Object* graphicsObj = graphicsObject->GetObject();
    if (graphicsObj->IsHeapObject())
    {
        HeapObject* graphicsObject = static_cast<HeapObject*>(graphicsObj);
        ObjectTypeSymbol* graphicsType = graphicsObject->GetType();
        int32_t nativeIndex = graphicsType->GetFieldIndex("native");
        if (nativeIndex != -1)
        {
            Object* native = graphicsObject->GetField(nativeIndex);
            if (native->IsValueObject())
            {
                Value* nativeValue = static_cast<Value*>(native);
                if (nativeValue->IsGenericPointerValue())
                {
                    GenericPointerValue* nativePtr = static_cast<GenericPointerValue*>(nativeValue);
                    void* np = nativePtr->Pointer();
                    Gdiplus::Graphics* graphics = static_cast<Gdiplus::Graphics*>(np);
                    Gdiplus::Status status = graphics->FillPolygon(brush, points.data(), points.size());
                    if (status != Gdiplus::Ok)
                    {
                        ThrowError("error: Graphics.FillPolygon: could not fill polygon", Rt::Instance().FilePath(), Span());
                    }
                }
                else
                {
                    ThrowError("error: Graphics.FillPolygon: pointer object expected", Rt::Instance().FilePath(), Span());
                }
            }
            else
            {
                ThrowError("error: Graphics.FillPolygon: value object expected", Rt::Instance().FilePath(), Span());
            }
        }
        else
        {
            ThrowError("error: Graphics.FillPolygon: 'native' not found", Rt::Instance().FilePath(), Span());
        }
    }
}

class GraphicsDrawEllipseMethod : public ExternalSubroutineSymbol
{
public:
    GraphicsDrawEllipseMethod(const soul::ast::Span& span_);
    void Execute(ExecutionContext* context) override;
};

GraphicsDrawEllipseMethod::GraphicsDrawEllipseMethod(const soul::ast::Span& span_) : ExternalSubroutineSymbol(span_, "Graphics.DrawEllipse", GraphicsDrawEllipseId())
{
}

void GraphicsDrawEllipseMethod::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> rectObject = stack->Pop();
    std::unique_ptr<Object> penObject = stack->Pop();
    std::unique_ptr<Object> graphicsObject = stack->Pop();
    Gdiplus::Pen* pen = nullptr;
    Object* rectObj = rectObject->GetObject();
    float x = 0;
    float y = 0;
    float w = 0;
    float h = 0;
    if (rectObj->IsHeapObject())
    {
        HeapObject* rectHeapObject = static_cast<HeapObject*>(rectObj);
        ObjectTypeSymbol* rectObjectType = rectHeapObject->GetType();
        int32_t location_index = rectObjectType->GetFieldIndex("location");
        if (location_index != -1)
        {
            Object* locationObject = rectHeapObject->GetField(location_index);
            Object* locationObj = locationObject->GetObject();
            if (locationObj->IsHeapObject())
            {
                HeapObject* locationHeapObject = static_cast<HeapObject*>(locationObj);
                ObjectTypeSymbol* locationType = locationHeapObject->GetType();
                int32_t x_index = locationType->GetFieldIndex("x");
                if (x_index != -1)
                {
                    Object* xObject = locationHeapObject->GetField(x_index);
                    Object* xObj = xObject->GetObject();
                    if (xObj->IsValueObject())
                    {
                        Value* xValue = static_cast<Value*>(xObj);
                        x = xValue->ToReal();
                    }
                    else
                    {
                        ThrowError("error: Graphics.DrawEllipse: value object expected", Rt::Instance().FilePath(), Span());
                    }
                }
                else
                {
                    ThrowError("error: Graphics.DrawEllipse: 'x' not found", Rt::Instance().FilePath(), Span());
                }
                int32_t y_index = locationType->GetFieldIndex("y");
                if (y_index != -1)
                {
                    Object* yObject = locationHeapObject->GetField(y_index);
                    Object* yObj = yObject->GetObject();
                    if (yObj->IsValueObject())
                    {
                        Value* yValue = static_cast<Value*>(yObj);
                        y = yValue->ToReal();
                    }
                    else
                    {
                        ThrowError("error: Graphics.DrawEllipse: value object expected", Rt::Instance().FilePath(), Span());
                    }
                }
                else
                {
                    ThrowError("error: Graphics.DrawEllipse: 'y' not found", Rt::Instance().FilePath(), Span());
                }
            }
        }
        int32_t size_index = rectObjectType->GetFieldIndex("size");
        if (size_index != -1)
        {
            Object* sizeObject = rectHeapObject->GetField(size_index);
            Object* sizeObj = sizeObject->GetObject();
            if (sizeObj->IsHeapObject())
            {
                HeapObject* sizeHeapObject = static_cast<HeapObject*>(sizeObj);
                ObjectTypeSymbol* sizeType = sizeHeapObject->GetType();
                int32_t w_index = sizeType->GetFieldIndex("w");
                if (w_index != -1)
                {
                    Object* wObject = sizeHeapObject->GetField(w_index);
                    Object* wObj = wObject->GetObject();
                    if (wObj->IsValueObject())
                    {
                        Value* wValue = static_cast<Value*>(wObj);
                        w = wValue->ToReal();
                    }
                    else
                    {
                        ThrowError("error: Graphics.DrawEllipse: value object expected", Rt::Instance().FilePath(), Span());
                    }
                }
                else
                {
                    ThrowError("error: Graphics.DrawEllipse: 'w' not found", Rt::Instance().FilePath(), Span());
                }
                int32_t h_index = sizeType->GetFieldIndex("h");
                if (h_index != -1)
                {
                    Object* hObject = sizeHeapObject->GetField(h_index);
                    Object* hObj = hObject->GetObject();
                    if (hObj->IsValueObject())
                    {
                        Value* hValue = static_cast<Value*>(hObj);
                        h = hValue->ToReal();
                    }
                    else
                    {
                        ThrowError("error: Graphics.DrawEllipse: value object expected", Rt::Instance().FilePath(), Span());
                    }
                }
                else
                {
                    ThrowError("error: Graphics.DrawEllipse: 'h' not found", Rt::Instance().FilePath(), Span());
                }
            }
        }
        else
        {
            ThrowError("error: Graphics.DrawEllipse: 'size' not found", Rt::Instance().FilePath(), Span());
        }
    }
    Gdiplus::RectF rect(x, y, w, h);
    Object* penObj = penObject->GetObject();
    if (penObj->IsHeapObject())
    {
        HeapObject* penHeapObject = static_cast<HeapObject*>(penObj);
        ObjectTypeSymbol* penObjectType = penHeapObject->GetType();
        int32_t colorIndex = penObjectType->GetFieldIndex("color");
        if (colorIndex != -1)
        {
            Object* color = penHeapObject->GetField(colorIndex);
            Object* colorObj = color->GetObject();
            if (colorObj->IsHeapObject())
            {
                HeapObject* colorObject = static_cast<HeapObject*>(colorObj);
                ObjectTypeSymbol* colorObjectType = colorObject->GetType();
                int32_t a = -1;
                int32_t r = -1;
                int32_t g = -1;
                int32_t b = -1;
                int32_t a_field_index = colorObjectType->GetFieldIndex("a");
                if (a_field_index != -1)
                {
                    a = colorObject->GetField(a_field_index)->ToInteger();
                }
                else
                {
                    ThrowError("error: Graphics.DrawEllipse: 'a' not found", Rt::Instance().FilePath(), Span());
                }
                int32_t r_field_index = colorObjectType->GetFieldIndex("r");
                if (r_field_index != -1)
                {
                    r = colorObject->GetField(r_field_index)->ToInteger();
                }
                else
                {
                    ThrowError("error: Graphics.DrawEllipse: 'r' not found", Rt::Instance().FilePath(), Span());
                }
                int32_t g_field_index = colorObjectType->GetFieldIndex("g");
                if (g_field_index != -1)
                {
                    g = colorObject->GetField(g_field_index)->ToInteger();
                }
                else
                {
                    ThrowError("error: Graphics.DrawEllipse: 'g' not found", Rt::Instance().FilePath(), Span());
                }
                int32_t b_field_index = colorObjectType->GetFieldIndex("b");
                if (b_field_index != -1)
                {
                    b = colorObject->GetField(g_field_index)->ToInteger();
                }
                else
                {
                    ThrowError("error: Graphics.DrawEllipse: 'b' not found", Rt::Instance().FilePath(), Span());
                }
                Gdiplus::Color color(a, r, g, b);
                float width = 0;
                int32_t width_index = penObjectType->GetFieldIndex("width");
                if (width_index != -1)
                {
                    Object* widthObject = penHeapObject->GetField(width_index);
                    if (widthObject->IsValueObject())
                    {
                        Value* widthValue = static_cast<Value*>(widthObject);
                        if (widthValue->IsRealValue())
                        {
                            width = widthValue->ToReal();
                        }
                        else
                        {
                            ThrowError("error: Graphics.DrawEllipse: real value expected", Rt::Instance().FilePath(), Span());
                        }
                    }
                    else
                    {
                        ThrowError("error: Graphics.DrawEllipse: value object expected", Rt::Instance().FilePath(), Span());
                    }
                }
                else
                {
                    ThrowError("error: Graphics.DrawEllipse: 'width' not found", Rt::Instance().FilePath(), Span());
                }
                pen = Rt::Instance().GetPen(PenKey(color, width));
            }
        }
    }
    Object* graphicsObj = graphicsObject->GetObject();
    if (graphicsObj->IsHeapObject())
    {
        HeapObject* graphicsObject = static_cast<HeapObject*>(graphicsObj);
        ObjectTypeSymbol* graphicsType = graphicsObject->GetType();
        int32_t nativeIndex = graphicsType->GetFieldIndex("native");
        if (nativeIndex != -1)
        {
            Object* native = graphicsObject->GetField(nativeIndex);
            if (native->IsValueObject())
            {
                Value* nativeValue = static_cast<Value*>(native);
                if (nativeValue->IsGenericPointerValue())
                {
                    GenericPointerValue* nativePtr = static_cast<GenericPointerValue*>(nativeValue);
                    void* np = nativePtr->Pointer();
                    Gdiplus::Graphics* graphics = static_cast<Gdiplus::Graphics*>(np);
                    Gdiplus::Status status = graphics->DrawEllipse(pen, rect);
                    if (status != Gdiplus::Ok)
                    {
                        ThrowError("error: Graphics.DrawEllipse: could not draw ellipse", Rt::Instance().FilePath(), Span());
                    }
                }
            }
        }
        else
        {
            ThrowError("error: Graphics.DrawEllipse: 'native' not found", Rt::Instance().FilePath(), Span());
        }
    }
    else
    {
        ThrowError("error: Graphics.DrawEllipse: heap object expected", Rt::Instance().FilePath(), Span());
    }
}

class GraphicsDrawArcMethod : public ExternalSubroutineSymbol
{
public:
    GraphicsDrawArcMethod(const soul::ast::Span& span_);
    void Execute(ExecutionContext* context) override;
};

GraphicsDrawArcMethod::GraphicsDrawArcMethod(const soul::ast::Span& span_) : ExternalSubroutineSymbol(span_, "Graphics.DrawArc", GraphicsDrawArcId())
{
}

void GraphicsDrawArcMethod::Execute(ExecutionContext* context)
{
    Stack* stack = context->GetStack();
    std::unique_ptr<Object> sweepAngleObject = stack->Pop();
    std::unique_ptr<Object> startAngleObject = stack->Pop();
    std::unique_ptr<Object> rectObject = stack->Pop();
    std::unique_ptr<Object> penObject = stack->Pop();
    std::unique_ptr<Object> graphicsObject = stack->Pop();
    float startAngle = 0;
    float sweepAngle = 0;
    float x = 0;
    float y = 0;
    float w = 0;
    float h = 0;
    Object* sweepAngleObj = sweepAngleObject->GetObject();
    if (sweepAngleObj->IsValueObject())
    {
        sweepAngle = sweepAngleObj->ToReal();
    }
    Object* startAngleObj = startAngleObject->GetObject();
    if (startAngleObj->IsValueObject())
    {
        startAngle = startAngleObj->ToReal();
    }
    Object* rectObj = rectObject->GetObject();
    if (rectObj->IsHeapObject())
    {
        HeapObject* rectHeapObject = static_cast<HeapObject*>(rectObj);
        ObjectTypeSymbol* rectObjectType = rectHeapObject->GetType();
        int32_t location_index = rectObjectType->GetFieldIndex("location");
        if (location_index != -1)
        {
            Object* locationObject = rectHeapObject->GetField(location_index);
            Object* locationObj = locationObject->GetObject();
            if (locationObj->IsHeapObject())
            {
                HeapObject* locationHeapObject = static_cast<HeapObject*>(locationObj);
                ObjectTypeSymbol* locationType = locationHeapObject->GetType();
                int32_t x_index = locationType->GetFieldIndex("x");
                if (x_index != -1)
                {
                    Object* xObject = locationHeapObject->GetField(x_index);
                    Object* xObj = xObject->GetObject();
                    if (xObj->IsValueObject())
                    {
                        Value* xValue = static_cast<Value*>(xObj);
                        x = xValue->ToReal();
                    }
                    else
                    {
                        ThrowError("error: Graphics.DrawArc: value object expected", Rt::Instance().FilePath(), Span());
                    }
                }
                else
                {
                    ThrowError("error: Graphics.DrawArc: 'x' not found", Rt::Instance().FilePath(), Span());
                }
                int32_t y_index = locationType->GetFieldIndex("y");
                if (y_index != -1)
                {
                    Object* yObject = locationHeapObject->GetField(y_index);
                    Object* yObj = yObject->GetObject();
                    if (yObj->IsValueObject())
                    {
                        Value* yValue = static_cast<Value*>(yObj);
                        y = yValue->ToReal();
                    }
                    else
                    {
                        ThrowError("error: Graphics.DrawArc: value object expected", Rt::Instance().FilePath(), Span());
                    }
                }
                else
                {
                    ThrowError("error: Graphics.DrawArc: 'y' not found", Rt::Instance().FilePath(), Span());
                }
            }
        }
        int32_t size_index = rectObjectType->GetFieldIndex("size");
        if (size_index != -1)
        {
            Object* sizeObject = rectHeapObject->GetField(size_index);
            Object* sizeObj = sizeObject->GetObject();
            if (sizeObj->IsHeapObject())
            {
                HeapObject* sizeHeapObject = static_cast<HeapObject*>(sizeObj);
                ObjectTypeSymbol* sizeType = sizeHeapObject->GetType();
                int32_t w_index = sizeType->GetFieldIndex("w");
                if (w_index != -1)
                {
                    Object* wObject = sizeHeapObject->GetField(w_index);
                    Object* wObj = wObject->GetObject();
                    if (wObj->IsValueObject())
                    {
                        Value* wValue = static_cast<Value*>(wObj);
                        w = wValue->ToReal();
                    }
                    else
                    {
                        ThrowError("error: Graphics.DrawArc: value object expected", Rt::Instance().FilePath(), Span());
                    }
                }
                else
                {
                    ThrowError("error: Graphics.DrawArc: 'w' not found", Rt::Instance().FilePath(), Span());
                }
                int32_t h_index = sizeType->GetFieldIndex("h");
                if (h_index != -1)
                {
                    Object* hObject = sizeHeapObject->GetField(h_index);
                    Object* hObj = hObject->GetObject();
                    if (hObj->IsValueObject())
                    {
                        Value* hValue = static_cast<Value*>(hObj);
                        h = hValue->ToReal();
                    }
                    else
                    {
                        ThrowError("error: Graphics.DrawArc: value object expected", Rt::Instance().FilePath(), Span());
                    }
                }
                else
                {
                    ThrowError("error: Graphics.DrawArc: 'h' not found", Rt::Instance().FilePath(), Span());
                }
            }
        }
        else
        {
            ThrowError("error: Graphics.DrawArc: 'size' not found", Rt::Instance().FilePath(), Span());
        }
    }
    Gdiplus::RectF rect(x, y, w, h);
    Gdiplus::Pen* pen = nullptr;
    Object* penObj = penObject->GetObject();
    if (penObj->IsHeapObject())
    {
        HeapObject* penHeapObject = static_cast<HeapObject*>(penObj);
        ObjectTypeSymbol* penObjectType = penHeapObject->GetType();
        int32_t colorIndex = penObjectType->GetFieldIndex("color");
        if (colorIndex != -1)
        {
            Object* color = penHeapObject->GetField(colorIndex);
            Object* colorObj = color->GetObject();
            if (colorObj->IsHeapObject())
            {
                HeapObject* colorObject = static_cast<HeapObject*>(colorObj);
                ObjectTypeSymbol* colorObjectType = colorObject->GetType();
                int32_t a = -1;
                int32_t r = -1;
                int32_t g = -1;
                int32_t b = -1;
                int32_t a_field_index = colorObjectType->GetFieldIndex("a");
                if (a_field_index != -1)
                {
                    a = colorObject->GetField(a_field_index)->ToInteger();
                }
                else
                {
                    ThrowError("error: Graphics.DrawArc: 'a' not found", Rt::Instance().FilePath(), Span());
                }
                int32_t r_field_index = colorObjectType->GetFieldIndex("r");
                if (r_field_index != -1)
                {
                    r = colorObject->GetField(r_field_index)->ToInteger();
                }
                else
                {
                    ThrowError("error: Graphics.DrawArc: 'r' not found", Rt::Instance().FilePath(), Span());
                }
                int32_t g_field_index = colorObjectType->GetFieldIndex("g");
                if (g_field_index != -1)
                {
                    g = colorObject->GetField(g_field_index)->ToInteger();
                }
                else
                {
                    ThrowError("error: Graphics.DrawArc: 'g' not found", Rt::Instance().FilePath(), Span());
                }
                int32_t b_field_index = colorObjectType->GetFieldIndex("b");
                if (b_field_index != -1)
                {
                    b = colorObject->GetField(g_field_index)->ToInteger();
                }
                else
                {
                    ThrowError("error: Graphics.DrawArc: 'b' not found", Rt::Instance().FilePath(), Span());
                }
                Gdiplus::Color color(a, r, g, b);
                float width = 0;
                int32_t width_index = penObjectType->GetFieldIndex("width");
                if (width_index != -1)
                {
                    Object* widthObject = penHeapObject->GetField(width_index);
                    if (widthObject->IsValueObject())
                    {
                        Value* widthValue = static_cast<Value*>(widthObject);
                        if (widthValue->IsRealValue())
                        {
                            width = widthValue->ToReal();
                        }
                        else
                        {
                            ThrowError("error: Graphics.DrawArc: real value expected", Rt::Instance().FilePath(), Span());
                        }
                    }
                    else
                    {
                        ThrowError("error: Graphics.DrawArc: value object expected", Rt::Instance().FilePath(), Span());
                    }
                }
                else
                {
                    ThrowError("error: Graphics.DrawArc: 'width' not found", Rt::Instance().FilePath(), Span());
                }
                pen = Rt::Instance().GetPen(PenKey(color, width));
            }
        }
    }
    Object* graphicsObj = graphicsObject->GetObject();
    if (graphicsObj->IsHeapObject())
    {
        HeapObject* graphicsObject = static_cast<HeapObject*>(graphicsObj);
        ObjectTypeSymbol* graphicsType = graphicsObject->GetType();
        int32_t nativeIndex = graphicsType->GetFieldIndex("native");
        if (nativeIndex != -1)
        {
            Object* native = graphicsObject->GetField(nativeIndex);
            if (native->IsValueObject())
            {
                Value* nativeValue = static_cast<Value*>(native);
                if (nativeValue->IsGenericPointerValue())
                {
                    GenericPointerValue* nativePtr = static_cast<GenericPointerValue*>(nativeValue);
                    void* np = nativePtr->Pointer();
                    Gdiplus::Graphics* graphics = static_cast<Gdiplus::Graphics*>(np);
                    Gdiplus::Status status = graphics->DrawArc(pen, rect, startAngle, sweepAngle);
                    if (status != Gdiplus::Ok)
                    {
                        ThrowError("error: Graphics.DrawArc: could not draw arc", Rt::Instance().FilePath(), Span());
                    }
                }
                else
                {
                    ThrowError("error: Graphics.DrawArc: pointer object expected", Rt::Instance().FilePath(), Span());
                }
            }
            else
            {
                ThrowError("error: Graphics.DrawArc: value object expected", Rt::Instance().FilePath(), Span());
            }
        }
        else
        {
            ThrowError("error: Graphics.DrawArc: 'native' not found", Rt::Instance().FilePath(), Span());
        }
    }
}

void Rt::Init(const std::string& filePath_, const soul::ast::Span& span)
{
    filePath = filePath_;
    Gdiplus::Status status = Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
    if (status != Gdiplus::Status::Ok)
    {
        ThrowError("error: GDI+ initialization failed", filePath, span);
    }
    AddExternalSubroutine(new PanicProcedure(span));
    AddExternalSubroutine(new IntToStringFunction(span));
    AddExternalSubroutine(new RealToStringFunction(span));
    AddExternalSubroutine(new ParseIntFunction(span));
    AddExternalSubroutine(new ParseRealFunction(span));
    AddExternalSubroutine(new InitStringBuilderProcedure(span));
    AddExternalSubroutine(new StringBuilderClearMethod(span));
    AddExternalSubroutine(new StringBuilderAppendStrMethod(span));
    AddExternalSubroutine(new StringBuilderAppendCharMethod(span));
    AddExternalSubroutine(new StringBuilderGetStringMethod(span));
    AddExternalSubroutine(new StringBuilderDisposeMethod(span));
    AddExternalSubroutine(new BitmapGetGraphicsMethod(span));
    AddExternalSubroutine(new BitmapSaveMethod(span));
    AddExternalSubroutine(new GraphicsClearMethod(span));
    AddExternalSubroutine(new GraphicsDrawLineMethod(span));
    AddExternalSubroutine(new GraphicsDrawRectangleMethod(span));
    AddExternalSubroutine(new GraphicsMeasureStringMethod(span));
    AddExternalSubroutine(new GraphicsDrawStringMethod(span));
    AddExternalSubroutine(new GraphicsFillPolygonMethod(span));
    AddExternalSubroutine(new GraphicsDrawEllipseMethod(span));
    AddExternalSubroutine(new GraphicsDrawArcMethod(span));
}

void Rt::Done()
{
    pens.clear();
    fonts.clear();
    brushes.clear();
    Gdiplus::GdiplusShutdown(gdiplusToken);
}

ExternalSubroutineSymbol* Rt::GetExternalSubroutine(const std::string& externalSubroutineName, Node* node) const
{
    auto it = externalSubroutineMap.find(externalSubroutineName);
    if (it != externalSubroutineMap.end())
    {
        return it->second;
    }
    else
    {
        ThrowError("external subroutine '" + externalSubroutineName + "' not found", node->FilePath(), node->Span());
    }
    return nullptr;
}

ExternalSubroutineSymbol* Rt::GetExternalSubroutineNoThrow(const std::string& externalSubroutineName) const
{
    auto it = externalSubroutineMap.find(externalSubroutineName);
    if (it != externalSubroutineMap.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

ExternalSubroutineSymbol* Rt::GetExternalSubroutine(const util::uuid& id) const
{
    auto it = externalSubroutineIdMap.find(id);
    if (it != externalSubroutineIdMap.end())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("external subroutine '" + util::ToString(id) + "' not found");
    }
}

ExternalSubroutineSymbol* GetExternalSubroutine(const std::string& externalSubroutineName, Node* node) 
{
    return Rt::Instance().GetExternalSubroutine(externalSubroutineName, node);
}

ExternalSubroutineSymbol* GetExternalSubroutineNoThrow(const std::string& externalSubroutineName)
{
    return Rt::Instance().GetExternalSubroutineNoThrow(externalSubroutineName);
}

ExternalSubroutineSymbol* GetExternalSubroutine(const util::uuid& externalSubroutineId)
{
    return Rt::Instance().GetExternalSubroutine(externalSubroutineId);
}

void Init(const std::string& filePath, const soul::ast::Span& span)
{
    Rt::Instance().Init(filePath, span);
}

void Done()
{
    Rt::Instance().Done();
}

} // namespace p
