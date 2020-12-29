/***************************************************************************
                          Search.cpp  -  description
                             -------------------
    begin                : Mon Jun 2 2003
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
using namespace __gnu_cxx;
using namespace GraphOperations;
using namespace SearchAlgorithms;
extern Statistics stat;
void printVector(vector<int> vec) 
{
  int i;
  //std::sort(vec.begin(),vec.end());
  cout<<endl;
  for(i=0;i<vec.size();i++)
    cout<<vec[i]<<" ";
  cout<<endl;
}

//#define DEBUGSEARCH
int Search::branchAndBound(Graph &graph, int bound_, int timeBound_)
{
  int i;
  bound =bound_;
  timeBound = timeBound_;

  Node *start;

  startTime = time(NULL);
  
  start = new Node();
  start->setParent(NULL);
  start->setId(0);
  start->ifStart = true;
  
 
 
  opt=1;  
#ifdef DEBUGSEARCH
  cout<<"Search inited\n";
#endif
  vector<list<int> > temp1(graph.getAdjMatrixSize()+1);
  hash_set<int> temp2;
  vector<hash_set<int> > temp3(graph.getAdjMatrixSize()+1);
  bb(start,graph,temp2,temp1,temp3);
  return bound;

}

/* Traditional recursive implementation */

void Search::bb(Node *p, Graph &graph, 
		hash_set<int>nodesProcessed,vector<list<int> >
		connectivityList,vector<hash_set<int> > &simplicialList)
		
{
 
  Node *temp;
  stat.nodesExplored++;
  vector<hash_set<int> > selfSimplicialList(simplicialList.size());
 
  // If the time limit is execeded then exit
  currentTime = time(NULL);
  if((currentTime-startTime) >=timeBound) {
    opt=0; //solution is not optimal
    return;
  }

  // Expand node
  p->expandNode(bound,graph,nodesProcessed,connectivityList);
  
  // Check to see if the node can be pruned by strongly mutex property
  hash_set<int>::iterator currMutex,lastMutex,tempMutex;
  currMutex=p->stronglyMutexVertices.begin();
  lastMutex=p->stronglyMutexVertices.end();
  while(currMutex!=lastMutex){
    // Check to see if the same id is present in an already explored vertex
    // that made p->id simplicial
    tempMutex=simplicialList[*currMutex].find(p->getId());
    // If such a vertex is found
    if(tempMutex!=simplicialList[*currMutex].end()){
      simplicialList[p->getId()]=p->stronglyMutexVertices;
      
      // Revert Graph
      graph.addEdges(p->edgesDeleted);
      graph.deleteEdges(p->edgesAdded);
      stat.nodesPruned++;
      stat.nodesPrunedBySymmetry++;
      stat.nodesPrunedByStronglyMutex++;
      // bugfix - pad 6/27/6
      vector<Node*> children = p->getChildren();
      for (int i=0; i<children.size(); i++)
      	delete children[i];
      // end bugfix
      return;
    }
    currMutex++;
  }
  simplicialList[p->getId()]=p->stronglyMutexVertices;
  // If the node is pruned because of the lower bound then delete it
  if(p->pruned == true){
    // Revert Graph
    graph.addEdges(p->edgesDeleted);
    graph.deleteEdges(p->edgesAdded);
    stat.nodesPruned++;
    // bugfix - pad 6/27/6
    vector<Node*> children = p->getChildren();
    for (int i=0; i<children.size(); i++)
      delete children[i];
    // end bugfix
    return;
  }

  // Get the children of the current node
  vector<Node*> children = p->getChildren();

  // If the number of children is zero then this is the last node, so delete it
  if(children.size()==0 && p->getFValue() < bound) {
    bound = p->getFValue();
    bestOrdering=vector<int>(p->peo);
    cerr<<"printing peo "<<endl;
    printVector(p->peo);
    cerr<<"Bound changed to "<<bound<<"\n";
  }
  else{
    // expand children
    vector<Node*>::iterator curr,last;
    curr = children.begin();
    last = children.end();
    while(curr!=last){
      hash_set<int>::iterator element;
      // Check if the current node is already explored by a mutex node
      element=nodesProcessed.find((*curr)->getId());
      // If the node is not processed
      if(element==nodesProcessed.end()){
	bb(*curr,graph,nodesProcessed,connectivityList,selfSimplicialList);
	for(int i=0;i<(*curr)->processedVertices.size();i++){
	  int id=(*curr)->processedVertices[i];
	  nodesProcessed.insert(id);
	  connectivityList[id]=graph.
	    getIntListOfEdges(id);
	}
	delete(*curr);
      }
     
      else{
	 // If the node is in the processed list and its N(A) is same
	if(graph.ifSameEdges(*element,connectivityList[*element])){
	  stat.nodesPruned++;
	  stat.nodesPrunedBySymmetry++;
	  stat.nodesExplored++;
	  delete(*curr);
	  curr++;
	  continue;
	}
	// else update the N(A) of the node and explore it
	else{	
	  bb(*curr,graph,nodesProcessed,connectivityList,selfSimplicialList);
	  for(int i=0;i<(*curr)->processedVertices.size();i++){
	    int id=(*curr)->processedVertices[i];
	    nodesProcessed.insert(id);
	    connectivityList[id]=graph.
	      getIntListOfEdges(id);
	  }
	  delete(*curr);
	}
      }
      curr++;
    }
  }
  // Revert Graph
  graph.addEdges(p->edgesDeleted);
  graph.deleteEdges(p->edgesAdded);
}

