#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <vector>
#include <list>
#include <ext/hash_set>
#include <algorithm>
#include "Util.h"


namespace GraphOperations{
  using namespace std;
  using namespace __gnu_cxx;
  using namespace Utilities;
  

  const int ACTIVE=1;
  const int INACTIVE=-1;
  
  class EdgeList {
  public:
    int value;
    int number;
    EdgeList* next;
    EdgeList* prev;
    EdgeList(){
      value=0;
      number=0;
      next=NULL;
      prev=NULL;
    }
    EdgeList(int i) {
      value=0;
      next=NULL;
      prev=NULL;
      number=i;
    }
    void Reset(int i){
      value=0;
      next=NULL;
      prev=NULL;
      number=i;
    }
  };
 
  class VertexList{
  public:
    int value;
    int number;
    VertexList *next;
    VertexList *prev;
    VertexList(int i){
      value = INACTIVE;
      next = NULL;
      prev = NULL;
      number=i;
    }
    void Reset(int i){
      value=INACTIVE;
      next=NULL;
      prev=NULL;
      number=i;
    }
  };
  struct Edge {int v1; int v2;};
  struct ChangingVertex {
    public:
    int v;
    int degree;
  };
  class Graph{
  private:
    // The number of vertices that the graph has
    int numVertices;
    // The adjacency matrix for the graph
    // The 0th number stores the size of the EdgeList
    vector<vector<EdgeList*> > adjMatrix;
    vector<VertexList *> vertex;
    list<int> simplicialVertices;
    list<int> almostSimplicialVertices;
    list<int> stronglyMutexVertices;
    
  public:
   
    // Default constructor
    Graph();
    // Constructor to read into the graph
    Graph(vector<vector<int> > &matrix);
    // get Simplicial Vertices
    list<int> getSimplicialVertices(){return simplicialVertices;}
    list<int> getAlmostSimplicialVertices(){return almostSimplicialVertices;}
    list<int> getStronglyMutexVertices(){return stronglyMutexVertices;}
    int getAdjMatrixSize(){return adjMatrix.size();}
    int getNumEdges();
    // Add an Edge to the graph
    int addEdge(int i, int j);
    // Delete an Edge from the graph
    int deleteEdge(int i, int j);
    // Add a Null Vertex to the graph
    int addVertex(int i);
    // Delete a Vertex from the graph
    int deleteVertex(int i);
    // Add a set of edges to the graph
    int addEdges(vector<Edge> &edges);
    int addEdges(list<Edge> &edges);
    int getDegree(int v){ return adjMatrix[v][0]->number;}
    // Delete a set of edge from the graph
    int deleteEdges(vector<Edge> &edges);
    int deleteEdges(list<Edge> &edges);
 
    int getMinDegree();
    int getMaxDegree();
    int getNumVertices(){return numVertices;}
    // Add a vertex i along with some set of predefined edges
    int addVertex(int i, vector<int> &edges);
    // PrintGraph
    void printGraph();
    // Return the maximum-degree vertex having an edge with v
    int getMaxConnectedVertex(int v);
    // Return the minimum-degree vertex having an edge with v
    int getMinConnectedVertex(int v);
    // Return a list of edges connected to vertex v
    list<Edge> getListOfEdges(int v);
    list<int> getIntListOfEdges(int v);
    int computeInducedWidth(vector<int> &ordering);
    vector<int> randomOrdering();
    int computeM1LB();
    int computeM2LB();
    vector<int> minFillBBOrdering();
    vector<int> minDegreeBBOrdering();
    int getNumEdgesAdded(int v);
    void processSimplicialAndAlmostSimplicialVertices(int bound);
    void processSimplicialAndAlmostSimplicialVertices(int bound, vector<bool>&potentialSimplicialVertices);
    bool getIfSimplicial(int v);
    bool getIfAlmostSimplicial(int v);
    bool getIfSimplicial(int v,vector<bool>&potentialSimplcialVertices);
    bool getIfAlmostSimplicial(int v,vector<bool>&potentialSimplcialVertices);
    void printNeighborhoodGraph(int v);
    list<Graph> getConnectedComponents(int v);
    list<Edge> triangulate(int v);
    bool ifSameEdges(int v, list<int> &edges);
    void getPotentialSimplicialVertices(int, vector<bool>&);
  };
};   

 

#endif    
    
