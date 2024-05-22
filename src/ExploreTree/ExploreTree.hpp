#ifndef EXPLORE_TREE
#define EXPLORE_TREE

#include <queue>
#include <vector>
#include <memory>
#include <Move.hpp>
#include <observer_ptr.hpp>

enum class TypeMinMax
{
    MIN,
    MAX
};

class ExploreTree;

class NodeTree final
{
public:
    NodeTree() = default;
    NodeTree(NodeTree const &) = delete;
    NodeTree &operator=(NodeTree const &) = delete;

    NodeTree(NodeTree &&) = default;
    NodeTree &operator=(NodeTree &&) = default;

    ~NodeTree() = default;

    void add_node(std::unique_ptr<NodeTree>&& node);

    friend ExploreTree;

private:
    bool cmpNodeTree(NodeTree const &lhs, NodeTree const &rhs) const;

    std::priority_queue<
        std::unique_ptr<NodeTree>,
        std::vector<std::unique_ptr<NodeTree>>,
        decltype(NodeTree::cmpNodeTree)>
        m_sub_node;
    Move m_move;
    double m_eval;
    TypeMinMax m_min_max;
};

class ExploreTree final
{
public:
    ExploreTree();

    ExploreTree(ExploreTree const &) = delete;
    ExploreTree &operator=(ExploreTree const &) = delete;

    ExploreTree(ExploreTree &&) = default;
    ExploreTree &operator=(ExploreTree &&) = default;

    ~ExploreTree() = default;

    void keep_one_branch(std::unique_ptr<NodeTree>&& new_root);

private:
    std::unique_ptr<NodeTree> m_root;
};

#endif
