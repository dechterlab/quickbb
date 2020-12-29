#include "Util.h"
#include <vector>

int Utilities::Util::find_set_connected_components(vector<int>&p, int x)
{
  int temp;
  if(x!=p[x]){
    temp=find_set_connected_components(p,p[x]);
    p[x]=temp;
  }
  return p[x];
}

void Utilities::Util::union_set_connected_components(vector<int>&rank,
							    vector<int>&p,
							    int x, int y)
{
  link_set_connected_components(rank,p,find_set_connected_components(p,x),
				find_set_connected_components(p,y));
}
void Utilities::Util::link_set_connected_components(vector<int>&rank,
							   vector<int>&p,
							   int x, int y) 
{
  if(rank[x] > rank[y])
    p[y]=x;
  else{
    p[x]=y;
    if(rank[x]==rank[y])
      rank[y]++;
  }
}
void Utilities::Util::printVector(vector<int> &vec, ostream &oc)
  {
    int i;

    
    for(i=0;i<vec.size();i++)
      oc<<vec[i]<<" ";
    oc<<endl;
  }

  
    
