// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>
#include <gdiplus.h>
#undef min
#undef max
#undef GetObject

module p.rt;

import p.execute;
import p.type;
import p.value;
import util;

namespace p {

/*
    Gdiplus::Bitmap bm(100, 100, PixelFormat32bppARGB);
    Gdiplus::Graphics* graphics = Gdiplus::Graphics::FromImage(&bm);
    float dpiX = graphics->GetDpiX();
    float dpiY = graphics->GetDpiY();
    int x = 0;
*/

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

ExternalSubroutine::ExternalSubroutine(const std::string& name_) : Subroutine(SubroutineKind::external, nullptr), name(name_), id(-1)
{
}

class BitmapGetGraphicsMethod : public ExternalSubroutine
{
public:
    BitmapGetGraphicsMethod();
    void Execute(ExecutionContext* context) override;
};

BitmapGetGraphicsMethod::BitmapGetGraphicsMethod() : ExternalSubroutine("Bitmap.GetGraphics")
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
    ObjectType* objectType = nullptr;
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
                throw std::runtime_error("value object expected");
            }
        }
        else
        {
            throw std::runtime_error("x not found");
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
                throw std::runtime_error("value object expected");
            }
        }
        else
        {
            throw std::runtime_error("y not found");
        }
    }
    else
    {
        throw std::runtime_error("heap object expected");
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
        throw std::runtime_error("native not found");
    }
    Block* block = context->GetBlock();
    GlobalTypeMap* typeMap = block->GetGlobalTypeMap();
    Type* graphicsType = typeMap->GetType("Graphics");
    if (graphicsType->IsObjectType())
    {
        ObjectType* graphicsObjectType = static_cast<ObjectType*>(graphicsType);
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
                throw std::runtime_error("dpiX not found");
            }
            int32_t dpiy_index = graphicsObjectType->GetFieldIndex("dpiY");
            if (dpiy_index != -1)
            {
                RealValue dpiY(native_dpiY);
                graphics->SetField(dpiy_index, &dpiY, context);
            }
            else
            {
                throw std::runtime_error("dpiY not found");
            }
            stack->Push(graphics->Clone());
        }
        else
        {
            throw std::runtime_error("native not found");
        }
    }
    else
    {
        throw std::runtime_error("object type expected");
    }
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

bool operator==(const FontKey & left, const FontKey & right)
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

class GraphicsClearMethod : public ExternalSubroutine
{
public:
    GraphicsClearMethod();
    void Execute(ExecutionContext* context) override;
};

class GraphicsDrawLineMethod : public ExternalSubroutine
{
public:
    GraphicsDrawLineMethod();
    void Execute(ExecutionContext* context) override;
};

class GraphicsDrawRectangleMethod : public ExternalSubroutine
{
public:
    GraphicsDrawRectangleMethod();
    void Execute(ExecutionContext* context) override;
};

class GraphicsMeasureStringMethod : public ExternalSubroutine
{
public:
    GraphicsMeasureStringMethod();
    void Execute(ExecutionContext* context) override;
};

class GraphicsDrawStringMethod : public ExternalSubroutine
{
public:
    GraphicsDrawStringMethod();
    void Execute(ExecutionContext* context) override;
};

class GraphicsFillPolygonMethod : public ExternalSubroutine
{
public:
    GraphicsFillPolygonMethod();
    void Execute(ExecutionContext* context) override;
};

class GraphicsDrawEllipseMethod : public ExternalSubroutine
{
public:
    GraphicsDrawEllipseMethod();
    void Execute(ExecutionContext* context) override;
};

class GraphicsDrawArcMethod : public ExternalSubroutine
{
public:
    GraphicsDrawArcMethod();
    void Execute(ExecutionContext* context) override;
};

class BitmapSaveMethod : public ExternalSubroutine
{
public:
    BitmapSaveMethod();
    void Execute(ExecutionContext* context) override;
};

class Rt
{
public:
    static Rt& Instance();
    void Init();
    void Done();
    void AddExternalSubroutine(ExternalSubroutine* externalSubroutine);
    ExternalSubroutine* GetExternalSubroutine(const std::string& externalSubroutineName) const;
    ExternalSubroutine* GetExternalSubroutine(int32_t id) const;
    Gdiplus::Pen* GetPen(const PenKey& penKey);
    Gdiplus::Font* GetFont(const FontKey& fontKey);
    Gdiplus::Brush* GetBrush(const Gdiplus::Color color);
private:
    Rt();
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    std::map<std::string, ExternalSubroutine*> externalSubroutineMap;
    std::vector<std::unique_ptr<ExternalSubroutine>> externalSubroutines;
    std::map<PenKey, Gdiplus::Pen*> penMap;
    std::vector<std::unique_ptr<Gdiplus::Pen>> pens;
    std::map<FontKey, Gdiplus::Font*> fontMap;
    std::vector<std::unique_ptr<Gdiplus::Font>> fonts;
    std::map<BrushKey, Gdiplus::Brush*> brushMap;
    std::vector<std::unique_ptr<Gdiplus::Brush>> brushes;
};

Rt::Rt()
{
    Gdiplus::Status status = Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
    if (status != Gdiplus::Status::Ok)
    {
        throw std::runtime_error("GDI+ initialization failed");
    }
    AddExternalSubroutine(new BitmapGetGraphicsMethod());
    AddExternalSubroutine(new GraphicsClearMethod());
    AddExternalSubroutine(new GraphicsDrawLineMethod());
    AddExternalSubroutine(new GraphicsDrawRectangleMethod());
    AddExternalSubroutine(new GraphicsMeasureStringMethod());
    AddExternalSubroutine(new GraphicsDrawStringMethod());
    AddExternalSubroutine(new GraphicsFillPolygonMethod());
    AddExternalSubroutine(new GraphicsDrawEllipseMethod());
    AddExternalSubroutine(new GraphicsDrawArcMethod());
    AddExternalSubroutine(new BitmapSaveMethod());
}

Rt& Rt::Instance()
{
    static Rt instance;
    return instance;
}

void Rt::Init()
{
}

void Rt::Done()
{
    pens.clear();
    fonts.clear();
    brushes.clear();
    Gdiplus::GdiplusShutdown(gdiplusToken);
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

void Rt::AddExternalSubroutine(ExternalSubroutine* externalSubroutine)
{
    int32_t id = externalSubroutines.size();
    externalSubroutine->SetId(id);
    externalSubroutines.push_back(std::unique_ptr<ExternalSubroutine>(externalSubroutine));
    externalSubroutineMap[externalSubroutine->Name()] = externalSubroutine;
}

ExternalSubroutine* Rt::GetExternalSubroutine(const std::string& externalSubroutineName) const
{
    auto it = externalSubroutineMap.find(externalSubroutineName);
    if (it != externalSubroutineMap.end())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("external subroutine '" + externalSubroutineName + "' not found");
    }
}

ExternalSubroutine* Rt::GetExternalSubroutine(int32_t id) const
{
    if (id >= 0 && id < externalSubroutines.size())
    {
        return externalSubroutines[id].get();
    }
    else
    {
        throw std::runtime_error("invalid external subroutine index " + std::to_string(id));
    }
}

ExternalSubroutine* GetExternalSubroutine(const std::string& externalSubroutineName)
{
    return Rt::Instance().GetExternalSubroutine(externalSubroutineName);
}

ExternalSubroutine* GetExternalSubroutine(int32_t id)
{
    return Rt::Instance().GetExternalSubroutine(id);
}

GraphicsClearMethod::GraphicsClearMethod() : ExternalSubroutine("Graphics.Clear")
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
        ObjectType* colorObjectType = static_cast<ObjectType*>(colorObject->GetType());
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
            throw std::runtime_error("a not found");
        }
        int32_t r_field_index = colorObjectType->GetFieldIndex("r");
        if (r_field_index != -1)
        {
            r = colorObject->GetField(r_field_index)->ToInteger();
        }
        else
        {
            throw std::runtime_error("r not found");
        }
        int32_t g_field_index = colorObjectType->GetFieldIndex("g");
        if (g_field_index != -1)
        {
            g = colorObject->GetField(g_field_index)->ToInteger();
        }
        else
        {
            throw std::runtime_error("r not found");
        }
        int32_t b_field_index = colorObjectType->GetFieldIndex("b");
        if (b_field_index != -1)
        {
            b = colorObject->GetField(g_field_index)->ToInteger();
        }
        else
        {
            throw std::runtime_error("r not found");
        }
        color = Gdiplus::Color(a, r, g, b);
    }
    std::unique_ptr<Object> graphics = stack->Pop();
    Object* graphicsObj = graphics->GetObject();
    if (graphicsObj->IsHeapObject())
    {
        HeapObject* graphicsObject = static_cast<HeapObject*>(graphicsObj);
        ObjectType* graphicsType = graphicsObject->GetType();
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
                        throw std::runtime_error("could not clear");
                    }
                }
            }
        }
        else
        {
            throw std::runtime_error("native not found");
        }
    }
    else
    {
        throw std::runtime_error("heap object expected");
    }
}

GraphicsDrawLineMethod::GraphicsDrawLineMethod() : ExternalSubroutine("Graphics.DrawLine")
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
        ObjectType* penObjectType = penHeapObject->GetType();
        int32_t colorIndex = penObjectType->GetFieldIndex("color");
        if (colorIndex != -1)
        {
            Object* color = penHeapObject->GetField(colorIndex);
            Object* colorObj = color->GetObject();
            if (colorObj->IsHeapObject())
            {
                HeapObject* colorObject = static_cast<HeapObject*>(colorObj);
                ObjectType* colorObjectType = static_cast<ObjectType*>(colorObject->GetType());
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
                    throw std::runtime_error("a not found");
                }
                int32_t r_field_index = colorObjectType->GetFieldIndex("r");
                if (r_field_index != -1)
                {
                    r = colorObject->GetField(r_field_index)->ToInteger();
                }
                else
                {
                    throw std::runtime_error("r not found");
                }
                int32_t g_field_index = colorObjectType->GetFieldIndex("g");
                if (g_field_index != -1)
                {
                    g = colorObject->GetField(g_field_index)->ToInteger();
                }
                else
                {
                    throw std::runtime_error("r not found");
                }
                int32_t b_field_index = colorObjectType->GetFieldIndex("b");
                if (b_field_index != -1)
                {
                    b = colorObject->GetField(g_field_index)->ToInteger();
                }
                else
                {
                    throw std::runtime_error("r not found");
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
                    throw std::runtime_error("width not found");
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
                    ObjectType* point1ObjectType = point1Object->GetType();
                    x_index = point1ObjectType->GetFieldIndex("x");
                    if (x_index != -1)
                    {
                        x = point1Object->GetField(x_index)->ToReal();
                    }
                    else
                    {
                        throw std::runtime_error("x not found");
                    }
                    y_index = point1ObjectType->GetFieldIndex("y");
                    if (y_index != -1)
                    {
                        y = point1Object->GetField(y_index)->ToReal();
                    }
                    else
                    {
                        throw std::runtime_error("y not found");
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
                    ObjectType* graphicsType = graphicsObject->GetType();
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
                                    throw std::runtime_error("could not draw line");
                                }
                            }
                        }
                    }
                    else
                    {
                        throw std::runtime_error("native not found");
                    }
                }
                else
                {
                    throw std::runtime_error("heap object expected");
                }
            }
            else
            {
                throw std::runtime_error("heap object expected");
            }
        }
        else
        {
            throw std::runtime_error("color not found");
        }
    }
    else
    {
        throw std::runtime_error("heap object expected");
    }
}

GraphicsDrawRectangleMethod::GraphicsDrawRectangleMethod() : ExternalSubroutine("Graphics.DrawRectangle")
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
        ObjectType* penObjectType = penHeapObject->GetType();
        int32_t colorIndex = penObjectType->GetFieldIndex("color");
        if (colorIndex != -1)
        {
            Object* color = penHeapObject->GetField(colorIndex);
            Object* colorObj = color->GetObject();
            if (colorObj->IsHeapObject())
            {
                HeapObject* colorObject = static_cast<HeapObject*>(colorObj);
                ObjectType* colorObjectType = static_cast<ObjectType*>(colorObject->GetType());
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
                    throw std::runtime_error("a not found");
                }
                int32_t r_field_index = colorObjectType->GetFieldIndex("r");
                if (r_field_index != -1)
                {
                    r = colorObject->GetField(r_field_index)->ToInteger();
                }
                else
                {
                    throw std::runtime_error("r not found");
                }
                int32_t g_field_index = colorObjectType->GetFieldIndex("g");
                if (g_field_index != -1)
                {
                    g = colorObject->GetField(g_field_index)->ToInteger();
                }
                else
                {
                    throw std::runtime_error("r not found");
                }
                int32_t b_field_index = colorObjectType->GetFieldIndex("b");
                if (b_field_index != -1)
                {
                    b = colorObject->GetField(g_field_index)->ToInteger();
                }
                else
                {
                    throw std::runtime_error("b not found");
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
                    throw std::runtime_error("width not found");
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
        ObjectType* rectObjectType = rectHeapObject->GetType();
        int32_t location_index = rectObjectType->GetFieldIndex("location");
        if (location_index != -1)
        {
            Object* locationObject = rectHeapObject->GetField(location_index);
            Object* locationObj = locationObject->GetObject();
            if (locationObj->IsHeapObject())
            {
                HeapObject* locationHeapObject = static_cast<HeapObject*>(locationObj);
                ObjectType* locationType = locationHeapObject->GetType();
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
                        throw std::runtime_error("value object expected");
                    }
                }
                else
                {
                    throw std::runtime_error("x not found");
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
                        throw std::runtime_error("value object expected");
                    }
                }
                else
                {
                    throw std::runtime_error("y not found");
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
                ObjectType* sizeType = sizeHeapObject->GetType();
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
                        throw std::runtime_error("value object expected");
                    }
                }
                else
                {
                    throw std::runtime_error("w not found");
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
                        throw std::runtime_error("value object expected");
                    }
                }
                else
                {
                    throw std::runtime_error("h not found");
                }
            }
        }
        else
        {
            throw std::runtime_error("size not found");
        }
    }
    Gdiplus::RectF rect(x, y, w, h);
    Object* graphicsObj = graphics->GetObject();
    if (graphicsObj->IsHeapObject())
    {
        HeapObject* graphicsObject = static_cast<HeapObject*>(graphicsObj);
        ObjectType* graphicsType = graphicsObject->GetType();
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
                        throw std::runtime_error("could not draw rectangle");
                    }
                }
            }
        }
        else
        {
            throw std::runtime_error("native not found");
        }
    }
    else
    {
        throw std::runtime_error("heap object expected");
    }
}

GraphicsMeasureStringMethod::GraphicsMeasureStringMethod() : ExternalSubroutine("Graphics.MeasureString")
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
        ObjectType* fontType = fontHeapObject->GetType();
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
                throw std::runtime_error("string object expected");
            }
        }
        else
        {
            throw std::runtime_error("family not found");
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
                throw std::runtime_error("value object expected");
            }
        }
        else
        {
            throw std::runtime_error("size not found");
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
                throw std::runtime_error("value object expected");
            }
        }
        else
        {
            throw std::runtime_error("style not found");
        }
        FontKey fontKey(familyName, size, style);
        font = Rt::Instance().GetFont(fontKey);
        Object* textObj = textObject->GetObject();
        std::u16string text = util::ToUtf16(textObj->ToStringObject(context)->Value());
        Object* graphicsObj = graphicsObject->GetObject();
        if (graphicsObj->IsHeapObject())
        {
            HeapObject* graphicsObject = static_cast<HeapObject*>(graphicsObj);
            ObjectType* graphicsType = graphicsObject->GetType();
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
                            Block* block = context->GetBlock();
                            GlobalTypeMap* typeMap = block->GetGlobalTypeMap();
                            Type* sizeType = typeMap->GetType("Size");
                            if (sizeType && sizeType->IsObjectType())
                            {
                                ObjectType* sizeObjectType = static_cast<ObjectType*>(sizeType);
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
                                    throw std::runtime_error("w not found");
                                }
                                int32_t h_index = sizeObjectType->GetFieldIndex("h");
                                if (h_index != -1)
                                {
                                    RealValue hValue(h);
                                    sizeObject->SetField(h_index, &hValue, context);
                                }
                                else
                                {
                                    throw std::runtime_error("h not found");
                                }
                                stack->Push(sizeObject->Clone());
                            }
                        }
                        else
                        {
                            throw std::runtime_error("could not measure string");
                        }
                    }
                }
            }
            else
            {
                throw std::runtime_error("native not found");
            }
        }
        else
        {
            throw std::runtime_error("heap object expected");
        }
    }
    else
    {
        throw std::runtime_error("heap object expected");
    }
}

GraphicsDrawStringMethod::GraphicsDrawStringMethod() : ExternalSubroutine("Graphics.DrawString")
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
        ObjectType* locationType = locationHeapObject->GetType();
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
                throw std::runtime_error("value object expected");
            }
        }
        else
        {
            throw std::runtime_error("x not found");
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
                throw std::runtime_error("value object expected");
            }
        }
        else
        {
            throw std::runtime_error("y not found");
        }
    }
    else
    {
        throw std::runtime_error("heap object expected");
    }
    Object* brushObj = brushObject->GetObject();
    if (brushObj->IsHeapObject())
    {
        HeapObject* heapBrushObject = static_cast<HeapObject*>(brushObj);
        ObjectType* brushType = heapBrushObject->GetType();
        int32_t color_index = brushType->GetFieldIndex("color");
        if (color_index != -1)
        {
            Object* color = heapBrushObject->GetField(color_index);
            Object* colorObj = color->GetObject();
            if (colorObj->IsHeapObject())
            {
                HeapObject* colorObject = static_cast<HeapObject*>(colorObj);
                ObjectType* colorObjectType = static_cast<ObjectType*>(colorObject->GetType());
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
                    throw std::runtime_error("a not found");
                }
                int32_t r_field_index = colorObjectType->GetFieldIndex("r");
                if (r_field_index != -1)
                {
                    r = colorObject->GetField(r_field_index)->ToInteger();
                }
                else
                {
                    throw std::runtime_error("r not found");
                }
                int32_t g_field_index = colorObjectType->GetFieldIndex("g");
                if (g_field_index != -1)
                {
                    g = colorObject->GetField(g_field_index)->ToInteger();
                }
                else
                {
                    throw std::runtime_error("r not found");
                }
                int32_t b_field_index = colorObjectType->GetFieldIndex("b");
                if (b_field_index != -1)
                {
                    b = colorObject->GetField(g_field_index)->ToInteger();
                }
                else
                {
                    throw std::runtime_error("r not found");
                }
                Gdiplus::Color color(a, r, g, b);
                brush = Rt::Instance().GetBrush(color);
            }
            else
            {
                throw std::runtime_error("heap object expected");
            }
        }
        else
        {
            throw std::runtime_error("color not found");
        }
    }
    else
    {
        throw std::runtime_error("heap object expected");
    }
    Object* fontObj = fontObject->GetObject();
    Gdiplus::Font* font = nullptr;
    if (fontObj->IsHeapObject())
    {
        HeapObject* fontHeapObject = static_cast<HeapObject*>(fontObj);
        std::string familyName;
        ObjectType* fontType = fontHeapObject->GetType();
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
                throw std::runtime_error("string object expected");
            }
        }
        else
        {
            throw std::runtime_error("family not found");
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
                throw std::runtime_error("value object expected");
            }
        }
        else
        {
            throw std::runtime_error("size not found");
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
                throw std::runtime_error("value object expected");
            }
        }
        else
        {
            throw std::runtime_error("style not found");
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
        ObjectType* graphicsType = graphicsObject->GetType();
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
                        throw std::runtime_error("could not draw string");
                    }
                }
            }
        }
    }
}

GraphicsFillPolygonMethod::GraphicsFillPolygonMethod() : ExternalSubroutine("Graphics.FillPolygon")
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
                ObjectType* pointType = pointHeapObject->GetType();
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
                        throw std::runtime_error("value object expected");
                    }
                }
                else
                {
                    throw std::runtime_error("x not found");
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
                        throw std::runtime_error("value object expected");
                    }
                }
                else
                {
                    throw std::runtime_error("y not found");
                }
                points.push_back(Gdiplus::PointF(x, y));
            }
            else
            {
                throw std::runtime_error("heap object expected");
            }
        }
    }
    else
    {
        throw std::runtime_error("array object expected");
    }
    Object* brushObj = brushObject->GetObject();
    if (brushObj->IsHeapObject())
    {
        HeapObject* heapBrushObject = static_cast<HeapObject*>(brushObj);
        ObjectType* brushType = heapBrushObject->GetType();
        int32_t color_index = brushType->GetFieldIndex("color");
        if (color_index != -1)
        {
            Object* color = heapBrushObject->GetField(color_index);
            Object* colorObj = color->GetObject();
            if (colorObj->IsHeapObject())
            {
                HeapObject* colorObject = static_cast<HeapObject*>(colorObj);
                ObjectType* colorObjectType = static_cast<ObjectType*>(colorObject->GetType());
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
                    throw std::runtime_error("a not found");
                }
                int32_t r_field_index = colorObjectType->GetFieldIndex("r");
                if (r_field_index != -1)
                {
                    r = colorObject->GetField(r_field_index)->ToInteger();
                }
                else
                {
                    throw std::runtime_error("r not found");
                }
                int32_t g_field_index = colorObjectType->GetFieldIndex("g");
                if (g_field_index != -1)
                {
                    g = colorObject->GetField(g_field_index)->ToInteger();
                }
                else
                {
                    throw std::runtime_error("r not found");
                }
                int32_t b_field_index = colorObjectType->GetFieldIndex("b");
                if (b_field_index != -1)
                {
                    b = colorObject->GetField(g_field_index)->ToInteger();
                }
                else
                {
                    throw std::runtime_error("r not found");
                }
                Gdiplus::Color color(a, r, g, b);
                brush = Rt::Instance().GetBrush(color);
            }
            else
            {
                throw std::runtime_error("heap object expected");
            }
        }
        else
        {
            throw std::runtime_error("color not found");
        }
    }
    else
    {
        throw std::runtime_error("heap object expected");
    }
    Object* graphicsObj = graphicsObject->GetObject();
    if (graphicsObj->IsHeapObject())
    {
        HeapObject* graphicsObject = static_cast<HeapObject*>(graphicsObj);
        ObjectType* graphicsType = graphicsObject->GetType();
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
                        throw std::runtime_error("could not fill polygon");
                    }
                }
            }
        }
        else
        {
            throw std::runtime_error("native not found");
        }
    }
}

GraphicsDrawEllipseMethod::GraphicsDrawEllipseMethod() : ExternalSubroutine("Graphics.DrawEllipse")
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
        ObjectType* rectObjectType = rectHeapObject->GetType();
        int32_t location_index = rectObjectType->GetFieldIndex("location");
        if (location_index != -1)
        {
            Object* locationObject = rectHeapObject->GetField(location_index);
            Object* locationObj = locationObject->GetObject();
            if (locationObj->IsHeapObject())
            {
                HeapObject* locationHeapObject = static_cast<HeapObject*>(locationObj);
                ObjectType* locationType = locationHeapObject->GetType();
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
                        throw std::runtime_error("value object expected");
                    }
                }
                else
                {
                    throw std::runtime_error("x not found");
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
                        throw std::runtime_error("value object expected");
                    }
                }
                else
                {
                    throw std::runtime_error("y not found");
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
                ObjectType* sizeType = sizeHeapObject->GetType();
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
                        throw std::runtime_error("value object expected");
                    }
                }
                else
                {
                    throw std::runtime_error("w not found");
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
                        throw std::runtime_error("value object expected");
                    }
                }
                else
                {
                    throw std::runtime_error("h not found");
                }
            }
        }
        else
        {
            throw std::runtime_error("size not found");
        }
    }
    Gdiplus::RectF rect(x, y, w, h);
    Object* penObj = penObject->GetObject();
    if (penObj->IsHeapObject())
    {
        HeapObject* penHeapObject = static_cast<HeapObject*>(penObj);
        ObjectType* penObjectType = penHeapObject->GetType();
        int32_t colorIndex = penObjectType->GetFieldIndex("color");
        if (colorIndex != -1)
        {
            Object* color = penHeapObject->GetField(colorIndex);
            Object* colorObj = color->GetObject();
            if (colorObj->IsHeapObject())
            {
                HeapObject* colorObject = static_cast<HeapObject*>(colorObj);
                ObjectType* colorObjectType = static_cast<ObjectType*>(colorObject->GetType());
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
                    throw std::runtime_error("a not found");
                }
                int32_t r_field_index = colorObjectType->GetFieldIndex("r");
                if (r_field_index != -1)
                {
                    r = colorObject->GetField(r_field_index)->ToInteger();
                }
                else
                {
                    throw std::runtime_error("r not found");
                }
                int32_t g_field_index = colorObjectType->GetFieldIndex("g");
                if (g_field_index != -1)
                {
                    g = colorObject->GetField(g_field_index)->ToInteger();
                }
                else
                {
                    throw std::runtime_error("r not found");
                }
                int32_t b_field_index = colorObjectType->GetFieldIndex("b");
                if (b_field_index != -1)
                {
                    b = colorObject->GetField(g_field_index)->ToInteger();
                }
                else
                {
                    throw std::runtime_error("r not found");
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
                    throw std::runtime_error("width not found");
                }
                pen = Rt::Instance().GetPen(PenKey(color, width));
            }
        }
    }
    Object* graphicsObj = graphicsObject->GetObject();
    if (graphicsObj->IsHeapObject())
    {
        HeapObject* graphicsObject = static_cast<HeapObject*>(graphicsObj);
        ObjectType* graphicsType = graphicsObject->GetType();
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
                        throw std::runtime_error("could not draw ellipse");
                    }
                }
            }
        }
        else
        {
            throw std::runtime_error("native not found");
        }
    }
    else
    {
        throw std::runtime_error("heap object expected");
    }
}

GraphicsDrawArcMethod::GraphicsDrawArcMethod() : ExternalSubroutine("Graphics.DrawArc")
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
        ObjectType* rectObjectType = rectHeapObject->GetType();
        int32_t location_index = rectObjectType->GetFieldIndex("location");
        if (location_index != -1)
        {
            Object* locationObject = rectHeapObject->GetField(location_index);
            Object* locationObj = locationObject->GetObject();
            if (locationObj->IsHeapObject())
            {
                HeapObject* locationHeapObject = static_cast<HeapObject*>(locationObj);
                ObjectType* locationType = locationHeapObject->GetType();
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
                        throw std::runtime_error("value object expected");
                    }
                }
                else
                {
                    throw std::runtime_error("x not found");
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
                        throw std::runtime_error("value object expected");
                    }
                }
                else
                {
                    throw std::runtime_error("y not found");
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
                ObjectType* sizeType = sizeHeapObject->GetType();
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
                        throw std::runtime_error("value object expected");
                    }
                }
                else
                {
                    throw std::runtime_error("w not found");
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
                        throw std::runtime_error("value object expected");
                    }
                }
                else
                {
                    throw std::runtime_error("h not found");
                }
            }
        }
        else
        {
            throw std::runtime_error("size not found");
        }
    }
    Gdiplus::RectF rect(x, y, w, h);
    Gdiplus::Pen* pen = nullptr;
    Object* penObj = penObject->GetObject();
    if (penObj->IsHeapObject())
    {
        HeapObject* penHeapObject = static_cast<HeapObject*>(penObj);
        ObjectType* penObjectType = penHeapObject->GetType();
        int32_t colorIndex = penObjectType->GetFieldIndex("color");
        if (colorIndex != -1)
        {
            Object* color = penHeapObject->GetField(colorIndex);
            Object* colorObj = color->GetObject();
            if (colorObj->IsHeapObject())
            {
                HeapObject* colorObject = static_cast<HeapObject*>(colorObj);
                ObjectType* colorObjectType = static_cast<ObjectType*>(colorObject->GetType());
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
                    throw std::runtime_error("a not found");
                }
                int32_t r_field_index = colorObjectType->GetFieldIndex("r");
                if (r_field_index != -1)
                {
                    r = colorObject->GetField(r_field_index)->ToInteger();
                }
                else
                {
                    throw std::runtime_error("r not found");
                }
                int32_t g_field_index = colorObjectType->GetFieldIndex("g");
                if (g_field_index != -1)
                {
                    g = colorObject->GetField(g_field_index)->ToInteger();
                }
                else
                {
                    throw std::runtime_error("r not found");
                }
                int32_t b_field_index = colorObjectType->GetFieldIndex("b");
                if (b_field_index != -1)
                {
                    b = colorObject->GetField(g_field_index)->ToInteger();
                }
                else
                {
                    throw std::runtime_error("r not found");
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
                    throw std::runtime_error("width not found");
                }
                pen = Rt::Instance().GetPen(PenKey(color, width));
            }
        }
    }
    Object* graphicsObj = graphicsObject->GetObject();
    if (graphicsObj->IsHeapObject())
    {
        HeapObject* graphicsObject = static_cast<HeapObject*>(graphicsObj);
        ObjectType* graphicsType = graphicsObject->GetType();
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
                        throw std::runtime_error("could not draw arc");
                    }
                }
            }
        }
        else
        {
            throw std::runtime_error("native not found");
        }
    }
}

BitmapSaveMethod::BitmapSaveMethod() : ExternalSubroutine("Bitmap.Save")
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
        ObjectType* bitmapType = bitmapHeapObject->GetType();
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
                            throw std::runtime_error("could not save bitmap");
                        }
                    }
                    else
                    {
                        throw std::runtime_error("encoder CLSID not found for encoding '" + GetImageFormatStr(imageFormat) + "'");
                    }
                }
            }
        }
        else
        {
            throw std::runtime_error("native not found");
        }
    }
    else
    {
        throw std::runtime_error("heap object expected");
    }
}

void Init()
{
    Rt::Instance().Init();
}

void Done()
{
    Rt::Instance().Done();
}

} // namespace p
