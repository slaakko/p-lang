// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module p.ast.node_list;

import p.ast.node;
import p.ast.writer;
import p.ast.reader;
import soul.ast.span;
import soul.lexer;
import std.core;

export namespace p {

template<typename T>
class NodeList
{
public:
    int Count() const { return nodes.size(); }
    void Add(T* node) { nodes.push_back(std::unique_ptr<T>(node)); }
    T* Get(int index) const { return nodes[index].get(); }
    std::vector<std::unique_ptr<T>>::const_iterator begin() const { return nodes.begin(); }
    std::vector<std::unique_ptr<T>>::const_iterator end() const { return nodes.end(); }
    void CloneTo(NodeList<T>& that, Node* parent) const
    {
        for (const auto& node : nodes)
        {
            that.Add(static_cast<T*>(node->Clone()));
        }
        that.SetParent(parent);
    }
    void Write(AstWriter& writer)
    {
        writer.GetBinaryWriter().Write(static_cast<int32_t>(Count()));
        for (const auto& node : nodes)
        {
            writer.WriteNode(node.get());
        }
    }
    void Read(AstReader& reader)
    {
        int n = reader.GetBinaryReader().ReadInt();
        for (int i = 0; i < n; ++i)
        {
            T* node = static_cast<T*>(reader.ReadNode());
            nodes.push_back(std::unique_ptr<T>(node));
        }
    }
    void SetParent(Node* parent)
    {
        for (auto& node : nodes)
        {
            node->SetParent(parent);
        }
    }
private:
    std::vector<std::unique_ptr<T>> nodes;
};

} // namespace p
