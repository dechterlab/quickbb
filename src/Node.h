/***************************************************************************
                          Node.h  -  description
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
#ifndef NODE_H
#define NODE_H

//#define DEBUGGING
#include <vector>
#include <list>
#include <ext/hash_set>
#include "Graph.h"
#include "Util.h"


/*
Structure Node
Each node has:
1. A parent
2. The simplical vertices in the current graph
3. The number of unordered nodes in the current graph.
(The branching factor at each node is equal to the number of unordered nodes)
4. The children of the current node.
5. The f,g and h values of the current node.
6. The current reduced graph.
*/

namespace SearchAlgorithms{
  using namespace std;
  using namespace __gnu_cxx;
  using namespace Utilities;
  using namespace GraphOperations;
  struct Node {
    private:
    /* The parent of the current node */
    Node *parent;
    
    /* The set of children of the current node */
    vector<Node*> children;
    
    /* The id assigned dynamically during search */
    int id;
    
    /* The g value of the node (the induced width computed so far) */
    int gValue;

    /* The heuristic value of the current node based on the width. */
    int hValue;

    /* The f value of the current node (MAX (g,h)) */
    int fValue;

    /* Generate the children of the current node */
    void generateChildren(int bound);
    
    list<int> edgesAdded;
    list<int> edgesDeleted;
    hash_set<int> markedVertices;
    vector<bool> potentialSimplicialVertices;
    public:
    /* Default constructor */
    Node() {
      pruned = false;
    }
    
    /* Destructor */
    ~Node() {}

    /* Return the f-value of the current node */
    int getFValue() {
        return fValue;
    }

    /* Set Id */
    void setId(int param) {
        id =param;
    }

    int getId() {
        return id;
    }

    /* Return the parent of the current node */
    Node* getParent() {
        return parent;
    }

    /* Set Parent */
    void setParent(Node *p) {
        parent =p;
    }

    /* Return the children of the current node */
    vector<Node *> getChildren() {
        return children;
    }

    /* Expand a node based on the current bound */
    int expandNode(int bound, Graph &graph);

    /* Set G value */
    void setGValue(int val) {
        gValue = val;
    }

    /* Get hValue */
    int getHValue() {
        return hValue;
    }

    /* Get gValue */
    int getGValue() {
        return gValue;
    }
#ifdef DEBUGGING

    int nodeNumber;
#endif

 };
};

#endif
