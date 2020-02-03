#ifndef ALGO_H
#define ALGO_H

#include <vector>
#include <string>
#include <set>
#include <unordered_map>

class Algo
{
public:
  Algo(); 
  void set_graph(std::vector<int> v, std::set <std::pair<int,int> > paths);
  std::set <std::pair<int,int> > BFS();
  std::set <std::pair<int,int> > DFS();
  int nocc();

protected:
  std::vector< std::vector<int> > graph;
  std::vector<int> conv_v;
  std::unordered_map <int,int> conv_m;
  int source;
};

#endif
