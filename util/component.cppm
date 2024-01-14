// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.component;

import std.core;

export namespace util {

class Container;

class Component
{
public:
    Component();
    virtual ~Component();
    Container* GetContainer() const { return container; }
    void SetContainer(Container* container_) { container = container_; }
    Component* NextSibling() const { return nextSibling; }
    void SetNextSibling(Component* nextSibling_) { nextSibling = nextSibling_; }
    Component* PrevSibling() const { return prevSibling; }
    void SetPrevSibling(Component* prevSibling_) { prevSibling = prevSibling_; }
    void LinkBefore(Component* component)
    {
        if (prevSibling)
        {
            prevSibling->nextSibling = component;
        }
        component->prevSibling = prevSibling;
        component->nextSibling = this;
        prevSibling = component;
    }
    void LinkAfter(Component* component)
    {
        if (nextSibling)
        {
            nextSibling->prevSibling = component;
        }
        component->prevSibling = this;
        component->nextSibling = nextSibling;
        nextSibling = component;
    }
    void Unlink()
    {
        if (prevSibling)
        {
            prevSibling->nextSibling = nextSibling;
        }
        if (nextSibling)
        {
            nextSibling->prevSibling = prevSibling;
        }
    }
private:
    Container* container;
    Component* nextSibling;
    Component* prevSibling;
};

class Container
{
public:
    Container(Component* parent_);
    virtual ~Container();
    bool IsEmpty() const { return firstChild == nullptr; }
    Component* Parent() const { return parent; }
    Component* FirstChild() const { return firstChild; }
    Component* LastChild() const { return lastChild; }
    void AddChild(Component* child);
    std::unique_ptr<Component> RemoveChild(Component* child);
    void InsertBefore(Component* child, Component* before);
    void InsertAfter(Component* child, Component* after);
private:
    Component* parent;
    Component* firstChild;
    Component* lastChild;
};


} // namespace util
