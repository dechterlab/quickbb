#ifndef UTIL_H
#define UTIL_H
#include <iostream>
#include <vector>
#include <list>
#include <ext/hash_set>
#include <algorithm>

namespace Utilities{
  using namespace std;
  class Util{
  public:
    static int find_set_connected_components(vector<int>&p, int x);
    static void union_set_connected_components(vector<int>&rank,
					vector<int>&p,
					int x, int y);
    static void link_set_connected_components(vector<int>&rank,
				       vector<int>&p,
				       int x, int y);
    static void printVector(vector<int> &vec, ostream &oc);
  
  };
}
    

#endif
