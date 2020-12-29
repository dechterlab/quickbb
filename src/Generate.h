#include <iostream>
#include <vector>
#include <fstream>
#include <sys/times.h>
#include <sys/time.h>
#include <sys/timeb.h>

namespace Generate
  {
  using namespace std;
  class RandomGraph
    {
    public:
      static vector<vector<int> > generateGraph(char *filename, int n, int e);
      static vector<vector<int> > readGraph(char *filename);
      static vector<vector<int> > generateKTree (char *filename,int n, int size, int percent);
      static vector<vector<int> > readCnf(char *filename);
    };
}
