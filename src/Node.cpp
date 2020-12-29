/***************************************************************************
   Node.cpp  -  description
      -------------------
begin                : Sat May 31 2003
copyright            : (C) 2003 by Vibhav Gogate
email                : vgogate@ics.uci.edu
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include "SearchAlgorithms.h"

using namespace std;
using namespace GraphOperations;
using namespace SearchAlgorithms;
extern Statistics stat;
extern ORDERING_HEURISTIC ordering_heuristic;

//#define DEBUGSEARCH
#define MAX(a,b) ((a)>(b)?(a):(b))
/*

  Function: expandNode
  Aim:
    1. To expand a node by generating its children
    2. Compute the gValue and hValue of the node and compare it with the bound.
  Arguments:
    1.
  Side Effects:
    1.

*/
void printVector(vector<bool> temp){
  vector<bool>::iterator curr,last;
  curr=temp.begin();
  last=temp.end();
  int count=0;
  cout<<"Printing vector\n";
  while(curr!=last){
    if(*curr==true) cout<<count<<" ";
    curr++;
    count++;
  }
  cout<<endl;
}
int Node::expandNode(int bound, Graph &graph,	
		     hash_set<int>&nodesProcessed,
		     vector<list<int> >&connectivityList)
{

  int temp;
  edgesAdded = list<Edge>();
  edgesDeleted = list<Edge>();
  potentialSimplicialVertices=vector<bool>(graph.getAdjMatrixSize()+1);
  
  /* Compute the gValue of the Node if not a start node */
  if(ifStart == false){
    gValue = MAX(gValue,graph.getDegree(id));
    if(gValue >= bound) {
      fValue=gValue;
      pruned = true;
      stat.nodesPrunedByGValue++;
      return 0;
    }
    
    /* Triangulate the current graph */
    edgesAdded=graph.triangulate(id);
    processedVertices=vector<int>();
    processedVertices.push_back(id);
    peo=vector<int>(getParent()->peo);
    potentialSimplicialVertices=vector<bool>(graph.getAdjMatrixSize()+1);
    //for(int i=0;i<potentialSimplicialVertices.size();i++)
    //potentialSimplicialVertices[i]=true;
    graph.getPotentialSimplicialVertices(id,potentialSimplicialVertices);
    
  }
  /* If start node then gValue is 0 */
  else {
      gValue = 0;
      fValue=0;
      peo=vector<int>();
      cerr<<"adjMat size ="<<graph.getAdjMatrixSize()<<endl;
      potentialSimplicialVertices=vector<bool>(graph.getAdjMatrixSize()+1);
      for(int i=0;i<potentialSimplicialVertices.size();i++)
	potentialSimplicialVertices[i]=true;
  }
#ifdef DEBUGSEARCH
  cout<<"Node:Deleting simplicial vertices\n";
#endif
  deleteSimplicialAndAlmostSimplicialVertices
    (graph,nodesProcessed,connectivityList);

  /* Prune if gValue is greater than the bound */
  if(gValue >= bound) {
      pruned = true;
      fValue = gValue;
      stat.nodesPrunedByGValue++;
      return 0;
  }

  
  if((graph.getNumVertices()-2) <=gValue){
    fValue = gValue;
    stat.nodesPrunedByGValue++;
    return 0;
  }
#ifdef DEBUGSEARCH
  cout<<"Node:Computing hValue\n";
#endif
  
  /* Compute the hValue of the current Node */
  hValue = graph.computeM2LB();
  
  /* Compute the fValue of the current Node */
  fValue = MAX(hValue,gValue);

  /* Test if the fValue exceeds the current bound */
  if(fValue >=bound) {
      pruned = true;
      stat.nodesPrunedByLB++;
      return 0;
  }
  /* Mark Distant neighbors of neighbors 
     markDistantNeighbors(bound);*/

  /* Generate children of the current Node */
#ifdef DEBUGSEARCH
  cout<<"Node: Generating children\n";
#endif

  generateChildren(bound,graph);
  
}
            
/*

 Function:     generateChildren()
 Aim:          To generate all children of the current node.
               a. Prune all children that are marked.
               b. Prune all children that have degree >= bound
               c. Children are visited according to the min-fill ordering.
 Arguments:
            1. int bound
 Side Effects:
            1.
 */


void Node::generateChildren(int bound, Graph &graph)
			    
{
  // A vector of ordering
  vector<int> ordering;

  // Temporary variables
  int i,j;
  
  // Compute MinFill Ordering of the current node
#ifdef DEBUGSEARCH
  cout<<"Node+generateChildren: getting minFill ordering\n";
  graph.printGraph();
#endif

  if(ordering_heuristic==MINFILL){
  ordering = graph.minFillBBOrdering();
  }else if(ordering_heuristic==RANDOM){
  ordering=graph.randomOrdering();
  }
  else if(ordering_heuristic==MINDEGREE){
    ordering=graph.minDegreeBBOrdering();
  }
  else {
    ordering=graph.randomOrdering();
  }

  children=vector<Node*>();
#ifdef DEBUGSEARCH
  cout<<"Node+generateChildren:Done processing here\n";
#endif
  // Generate a child node for each unmarked vertex 
  for(i=0;i<ordering.size();i++){
    hash_set<int>::iterator temp1;
    if((temp1=markedVertices.find(ordering[i]))!=markedVertices.end()) 
      continue;
    Node* temp;
    temp = new Node();
    temp->setId(ordering[i]);
    temp->setParent(this);
    temp->ifStart = false;
    temp->setGValue(fValue);
    #ifdef DEBUGGING
    temp->nodeNumber = count++;
    #endif
    children.push_back(temp);
 
  }
#ifdef DEBUGSEARCH
  cout<<"Number of children = "<<children.size()<<endl;
#endif
  // If no children are generated then the underlying graph is a clique
  if(children.size()==0)
   fValue=MAX(fValue,graph.getNumVertices()-2);
}
    

void Node::deleteSimplicialAndAlmostSimplicialVertices
(Graph &graph,
 hash_set<int>&nodesProcessed,
 vector<list<int> >&connectivityList)
{
  // Store all simplicial Vertices of the graph 
  list<int> simplicialVertices;
  list<int> almostSimplicialVertices;
  // Stores temporary list of edges that are deleted
  // Also stores the edges that will be later used to mark the vertices
  // that won't be considered when generating children
  list<Edge> temp;
  int lastVertex;
  list<Edge>::iterator curr,last;
  bool first=true;
  
  // Delete all simplicial and almost Simplicial vertices from the graph
  do {
    int lb= graph.computeM2LB();
    lb=MAX(gValue,lb);
    list<int>::iterator curr,last;
    hash_set<int>::iterator element;
    graph.processSimplicialAndAlmostSimplicialVertices(lb,potentialSimplicialVertices);
#ifdef DEBUGSEARCH
    cout<<"Node:Got simplicial and almost simplicial Vertices from graph\n";
#endif
    simplicialVertices=graph.getSimplicialVertices();
    almostSimplicialVertices=graph.getAlmostSimplicialVertices();
    curr = almostSimplicialVertices.begin();
    last = almostSimplicialVertices.end();
    while(curr!=last){
      temp=graph.getListOfEdges(*curr);
      element = nodesProcessed.find(*curr);
      if(element != nodesProcessed.end() 
	 && graph.ifSameEdges(*curr,connectivityList[*curr])){
	stat.nodesPrunedByStronglyMutex++;
	pruned =true;
      }
      peo.push_back(*curr);
      edgesDeleted.insert(edgesDeleted.end(),temp.begin(),temp.end());
      temp = graph.triangulate(*curr);
      edgesAdded.insert(edgesAdded.end(),temp.begin(),temp.end());
      gValue=MAX(gValue,graph.getDegree(*curr));
      graph.deleteVertex(*curr);
      lastVertex=*curr;
      graph.getPotentialSimplicialVertices(*curr,potentialSimplicialVertices);
      curr++;
    }
    curr=simplicialVertices.begin();
    last=simplicialVertices.end();
    while(curr!=last){
      temp=graph.getListOfEdges(*curr);
      element = nodesProcessed.find(*curr);
      if(element != nodesProcessed.end() 
	 && graph.ifSameEdges(*curr,connectivityList[*curr])){
	pruned =true;
	stat.nodesPrunedByStronglyMutex++;
      }
      peo.push_back(*curr);
      edgesDeleted.insert(edgesDeleted.end(),temp.begin(),temp.end());
      gValue=MAX(gValue,graph.getDegree(*curr));
      graph.deleteVertex(*curr);
      lastVertex=*curr;
      graph.getPotentialSimplicialVertices(*curr,potentialSimplicialVertices);
      curr++;
    }
#ifdef DEBUGSEARCH
    cout<<"Node: Done simplicial sizes= "<<simplicialVertices.size();
    cout<<", "<<almostSimplicialVertices.size();
    cout<<", "<<graph.getNumVertices()<<"\n";
#endif
    if(first){
      list<int> tempList = graph.getStronglyMutexVertices();
      curr=tempList.begin();
      last=tempList.end();
      while(curr!=last){
	if(*curr!=id)
	  stronglyMutexVertices.insert(*curr);
	curr++;
      }
      first=false;
    }
  }while((simplicialVertices.size() !=0 || almostSimplicialVertices.size()!=0)
	 && graph.getNumVertices()>1);
  curr=temp.begin();
  last=temp.end();
  markedVertices=hash_set<int>();
  while(curr!=last){
    Edge e;
    e=*curr;
    if(e.v1 == lastVertex) 
      markedVertices.insert(e.v2);
    else
      markedVertices.insert(e.v1);
    curr++;
  }
#ifdef DEBUGSEARCH
  cout<<"Node: Done marking vertices\n";
#endif  	
  
}  
        

   


    
