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
class NodeTree;

bool operator<(NodeTree const &lhs, NodeTree const &rhs);

class NodeTree final
{
public:
  NodeTree(Move m, double eval, TypeMinMax min_max);
  NodeTree(NodeTree const &) = default;
  NodeTree &operator=(NodeTree const &) = default;

  NodeTree(NodeTree &&) = default;
  NodeTree &operator=(NodeTree &&) = default;

  ~NodeTree() = default;

  void add_node(NodeTree const& node);
  bool empty() const;
  void setCutAlphaBeta();
  bool checkCutAlphaBeta() const;

  friend ExploreTree;

private:
  friend bool operator<(
      NodeTree const &lhs,
      NodeTree const &rhs);

  std::priority_queue<NodeTree>
      m_sub_node;
  Move m_move;
  double m_eval;
  TypeMinMax m_min_max;
  bool m_cut_alpha_beta;
};

class ExploreTree final
{
public:
  ExploreTree(TypeMinMax root_min_max = TypeMinMax::MIN);

  ExploreTree(ExploreTree const &) = delete;
  ExploreTree &operator=(ExploreTree const &) = delete;

  ExploreTree(ExploreTree &&) = default;
  ExploreTree &operator=(ExploreTree &&) = default;

  NodeTree& getRoot();

  ~ExploreTree() = default;

  void keep_one_branch(NodeTree &new_root);

private:
  NodeTree m_root;
};

bool operator<(NodeTree const &lhs, NodeTree const &rhs)
{
  if (lhs.m_min_max != rhs.m_min_max)
  {
    throw std::runtime_error("Invalid compare between Max-Node and Min-Node");
  }
  if (lhs.m_min_max == TypeMinMax::MAX)
  {
    return lhs.m_eval < rhs.m_eval;
  }
  else
  {
    return lhs.m_eval > rhs.m_eval;
  }
}

#endif
