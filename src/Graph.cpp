#include "Graph.h"

using namespace std;
using namespace GraphOperations;
using namespace Utilities;
  
#define DEBUGORDER
//#define DEBUGMLB
//#define DEBUGMINBB
//#define DEBUGSIM
//#define DEBUGCON

/* Default constructor */
GraphOperations::Graph::Graph()
{
  numVertices =1;
  
}
// Get number of edges in the graph
  int GraphOperations::Graph::getNumEdges()
  {
    VertexList *temp;
    temp=vertex[0]->next;
    int count=0;
    while(temp!=NULL)
      {
        count+=adjMatrix[temp->number][0]->number;
        temp=temp->next;
      }

    return count/2;
  }
/* Constructor to read the graph */
GraphOperations::Graph::Graph(vector<vector<int> > &matrix)
{
  // Temporary variables
  int i,j;
  int n;
  // Initialize adjacency matrix, numVertices and the vertexlist
  n = matrix.size();
  numVertices=1;
  adjMatrix = vector<vector<EdgeList* > > (n);
  vertex = vector<VertexList*> (n);
  for(i=0;i<n;i++){
    adjMatrix[i]=vector<EdgeList*>(n);
    vertex[i]=new VertexList(i);
    for(j=0;j<n;j++){
      adjMatrix[i][j]=new EdgeList(j);
    }
  }
  
#ifdef DEBUG
  cout<<"Graph parameters initialized\n";
#endif
  // Copy the matrix into the graph data structure
  for(i=0;i<n-1;i++)
    for(j=0;j<n-1;j++)
      if(matrix[i][j] == 1) {
#ifdef DEBUG
	cout<<"Adding edge"<<i<<" "<<j<<"\n";
#endif
	addEdge(i+1,j+1);
      }
}


// Add a Null vertex to the graph
int GraphOperations::Graph::addVertex(int i)
{
  int j;
#ifdef DEBUG
  cout<<"Here3\n";
#endif
  // Reset any edges that the current vertex may have
  for(j=0;j<adjMatrix[i].size();j++){
    deleteEdge(i,j);
  }
#ifdef DEBUG
  cout<<"Here2\n";
#endif
  // Activate the vertex if it is inactive and add it to the vertexlist
  if(vertex[i]->value== INACTIVE){
    VertexList* temp;
    vertex[i]->Reset(i);
    vertex[i]->value=ACTIVE; 
    temp = vertex[0]->next;
    vertex[0]->next=vertex[i];
    vertex[i]->prev = vertex[0];
    vertex[i]->next=temp;
    if(temp!=NULL) temp->prev=vertex[i];
    numVertices=numVertices+1;
  }
#ifdef DEBUG
  cout<<"Here 1 \n";
#endif
  return 1;
}

// Add an Edge to the graph
int GraphOperations::Graph::addEdge(int i, int j)
{
  int temp;
  if(vertex[i]->value==INACTIVE){
    addVertex(i);
  }
  if(vertex[j]->value==INACTIVE){
    addVertex(j);
  }
#ifdef DEBUG
  cout<<"Here\n";
#endif
  if(adjMatrix[i][j]->value!=1){
    adjMatrix[i][j]->Reset(j);
    adjMatrix[i][j]->value=1;
    EdgeList *temp;
    temp=adjMatrix[i][0]->next;
    adjMatrix[i][0]->next=adjMatrix[i][j];
    adjMatrix[i][j]->prev=adjMatrix[i][0];
    adjMatrix[i][j]->next=temp;
    if(temp!=NULL) temp->prev=adjMatrix[i][j];
    adjMatrix[i][0]->number=adjMatrix[i][0]->number+1;
  }
  
  if(adjMatrix[j][i]->value!=1){
    adjMatrix[j][i]->Reset(i);
    adjMatrix[j][i]->value=1;
    EdgeList *temp;
    temp=adjMatrix[j][0]->next;
    adjMatrix[j][0]->next=adjMatrix[j][i];
    adjMatrix[j][i]->prev=adjMatrix[j][0];
    adjMatrix[j][i]->next=temp;
    if(temp!=NULL) temp->prev=adjMatrix[j][i];
    adjMatrix[j][0]->number=adjMatrix[j][0]->number+1;
  }
}
int GraphOperations::Graph::addEdges(vector<Edge> &edges)
{
  vector<Edge>::iterator curr,last;
  curr= edges.begin();
  last=edges.end();
  while(curr!=last){
    Edge e = *curr;
    addEdge(e.v1,e.v2);
    curr++;
  }
}
int GraphOperations::Graph::addEdges(list<Edge> &edges)
{
  list<Edge>::iterator curr,last;
  curr= edges.begin();
  last=edges.end();
  while(curr!=last){
    Edge e = *curr;
    addEdge(e.v1,e.v2);
    curr++;
  }
}

int GraphOperations::Graph::deleteEdges(vector<Edge> &edges)
{
  vector<Edge>::iterator curr,last;
  curr= edges.begin();
  last=edges.end();
  while(curr!=last){
    Edge e = *curr;
    deleteEdge(e.v1,e.v2);
    curr++;
  }
}

int GraphOperations::Graph::deleteEdges(list<Edge> &edges)
{
  list<Edge>::iterator curr,last;
  curr= edges.begin();
  last=edges.end();
  while(curr!=last){
    Edge e = *curr;
    deleteEdge(e.v1,e.v2);
    curr++;
  }
}


int GraphOperations::Graph::deleteEdge(int i, int j)
{
  if(adjMatrix[i][j]->value == 1){
    EdgeList *p,*q;
    p=adjMatrix[i][j];
    q=adjMatrix[i][j]->prev;
    q->next=p->next;
    if(p->next!=NULL) p->next->prev=q;
    //delete(p);
    adjMatrix[i][j]->Reset(j);
    adjMatrix[i][0]->number=adjMatrix[i][0]->number-1;
  }
  if(adjMatrix[j][i]->value ==1){
    EdgeList *p,*q;
    p=adjMatrix[j][i];
    q=adjMatrix[j][i]->prev;
    q->next=p->next;
    if(p->next!=NULL) p->next->prev=q;
    //delete(p);
    adjMatrix[j][i]->Reset(i);
    adjMatrix[j][0]->number=adjMatrix[j][0]->number-1;
  }
  return 1;
}

int GraphOperations::Graph::deleteVertex(int i)
{
  int j;
  if(vertex[i]->value==ACTIVE){
    
    EdgeList *p;
    p=adjMatrix[i][0];
    p=p->next;
    while(p!=NULL){
      j=p->number;
      p=p->next;
      deleteEdge(i,j);
    }
    VertexList *a,*b;
    a=vertex[i]->next;
    b=vertex[i]->prev;
    if(a!=NULL) a->prev=b;
    if(b!=NULL) b->next=a;
    vertex[i]->Reset(i);
    numVertices=numVertices-1;
  }
}


void GraphOperations::Graph::printGraph()
{
  int i,j;
  cout<<"Printing AdjMatrix\n";
  for(i=1;i<numVertices;i++){
    for(j=1;j<numVertices;j++)
      cout<<adjMatrix[i][j]->value<<" ";
    cout<<endl;
  }
  cout<<"Printing EdgeLists\n";
  VertexList *temp;
  temp=vertex[0]->next;
  
  while(temp!=NULL){
    EdgeList *p;
    p=adjMatrix[temp->number][0];
    p=p->next;
    cout<<"Vertex "<<temp->number<<": ";
    while(p!=NULL){
      cout<<p->number<<" ";
      p=p->next;
    }
    cout<<", Degree = "<<adjMatrix[temp->number][0]->number<<endl;
    temp=temp->next;
  }
}

int GraphOperations::Graph::computeInducedWidth(vector<int> &ordering)
{
  
  int i;
  vector<Edge> edgesAdded;
  vector<bool> processed(adjMatrix.size());
  for(i=0;i<adjMatrix.size();i++)
    processed[i]=false;
  int w_star=0;
  bool done=false;
  for(i=0;i<ordering.size() && !done;i++){
    int v = ordering[i];
    EdgeList *p,*q;
    p=adjMatrix[v][0]->next;
    int count=0;
    

    while(p!=NULL){
      if(processed[p->number]) {
	p = p->next;
	count++;
	continue;
      }
      q=p->next;
      while(q!=NULL){
	if(processed[q->number]){
	  q = q->next;
	  continue;
	}
	if(adjMatrix[p->number][q->number]->value ==0){
	  addEdge(p->number,q->number);
	  Edge e;
	  e.v1=p->number;
	  e.v2=q->number;
	  edgesAdded.push_back(e);
	}
	q = q->next;
      }
      p=p->next;
    }
    processed[v]=true;
    int w=adjMatrix[v][0]->number-count;
    if(w > w_star) w_star=w;
    if((w_star+1) >= (ordering.size()-i)) done = true;
  }

  for(i=0;i<edgesAdded.size();i++)
    deleteEdge(edgesAdded[i].v1,edgesAdded[i].v2);

  return w_star;

}
/*	
std::vector<int> GraphOperations::Graph::randomOrdering()
{
  int i;
  vector<int> ordering(numVertices-1);
  for(i=0;i<numVertices;i++)
    ordering[i]=0;
  int processed;
  VertexList *vList;
  vList=vertex[0]->next;
  int count=0;
  while(vList!=NULL){
    
#ifdef DEBUGORDER
    cout<<vList->number<<" put in ";
    if(count>=numVertices) cout<<"Something wrong, count = "<<count<<endl;
#endif
    bool done;
    int temp;
    do{
      done=false;
      temp = rand()%(numVertices-1);
      if(ordering[temp]!=0){
	done=true;
      }
    }while(done);
#ifdef DEBUGORDER
    cout<<temp<<endl;
#endif
    ordering[temp]=vList->number;
    count++;
    vList=vList->next;
  }
#ifdef DEBUGORDER
  if(numVertices-1 !=count) cout<<"Something wrong\n";
#endif
  return ordering;
}

*/
std::vector<int> GraphOperations::Graph::randomOrdering()
{
  int i;
  srand ( time(NULL) );
  vector<int> ordering(numVertices-1);
  vector<int> temp_list;
  VertexList *vList;
  vList=vertex[0]->next;
  int count=0;

  // Traverse the vlist and put all vertices in a temporary list
  while(vList!=NULL){
    temp_list.push_back(vList->number);
    vList=vList->next;
    count++;
  }

  for(i=0;i<numVertices-1;i++){
    int temp=rand()%count;
    ordering[i]=temp_list[temp];
    count--;
    vector<int>::iterator pos=temp_list.begin();
    pos+=temp;
    temp_list.erase(pos);
  }
  return ordering;
}

int GraphOperations::Graph::getMaxConnectedVertex(int v)
{
  VertexList *vList;
  vList=vertex[v];
  if(vList->value == INACTIVE) return 0;
  EdgeList *p;
  p=adjMatrix[v][0]->next;
  int maxVertex=0;
  int max=0;
  while(p!=NULL){
    if(adjMatrix[p->number][0]->number > max){
      max = adjMatrix[p->number][0]->number;
      maxVertex=p->number;
    }
    p=p->next;
  }
  return maxVertex;
}
int GraphOperations::Graph::getMinConnectedVertex(int v)
{
  VertexList *vList;
  vList=vertex[v];
  if(vList->value == INACTIVE) return 0;
  EdgeList *p;
  p=adjMatrix[v][0]->next;
  int minVertex=0;
  int min=numVertices-1;
  while(p!=NULL){
    if(adjMatrix[p->number][0]->number <min){
      min = adjMatrix[p->number][0]->number;
      minVertex=p->number;
    }
    p=p->next;
  }
  return minVertex;
}
list<int> GraphOperations::Graph::getIntListOfEdges(int v)
{
  list<int> returnList = list<int>();
  VertexList *vList;
  vList=vertex[v];
  if(vList->value == INACTIVE) return returnList;
  EdgeList *p;
  p=adjMatrix[v][0]->next;
  while(p!=NULL){
    returnList.push_back(p->number);
    p=p->next;
  }
  return returnList;
}
   

list<Edge> GraphOperations::Graph::getListOfEdges(int v)
{
  list<Edge> returnList = list<Edge>();
  VertexList *vList;
  vList=vertex[v];
  if(vList->value == INACTIVE) return returnList;
  EdgeList *p;
  p=adjMatrix[v][0]->next;
  while(p!=NULL){
    Edge e;
    e.v1=v;
    e.v2=p->number;
    returnList.push_back(e);
    p=p->next;
  }
  return returnList;
}
    

    
int GraphOperations::Graph::computeM1LB()
{

#ifdef DEBUGMLB

#endif  
 
  int i,j;
  int minDegree = numVertices+1;
  int maxDegree = -1;
  int maxMinDegree;
  
  hash_set<int> degreeList[numVertices-1];
  vector<ChangingVertex> changedVertices(numVertices-1);
  for(i=0;i<numVertices-1;i++){
    degreeList[i]=hash_set<int>();
    changedVertices[i].v=0;
    changedVertices[i].degree=0;
  }
  
#ifdef DEBUGMLB
  cout<<"Hash set init\n";
#endif  
  list<Edge> addedEdges,deletedEdges;
  // Initialize Degree List. This is O(V) operation
  VertexList *vList;
  vList = vertex[0]->next;
  while(vList!=NULL){
    int v = vList->number;
    if(minDegree > adjMatrix[v][0]->number) 
      minDegree=adjMatrix[v][0]->number;
    if(maxDegree < adjMatrix[v][0]->number) 
      maxDegree = adjMatrix[v][0]->number;
    degreeList[adjMatrix[v][0]->number].insert(v);
    vList=vList->next;
  }
  maxMinDegree = minDegree;
#ifdef DEBUGMLB
  cout<<"Degree list inited\n";
#endif  

  while(minDegree != maxDegree){
    hash_set<int> minDegreeList = degreeList[minDegree];

#ifdef DEBUGMLB
    cout<<"Printing mindegree list\n";
    hash_set<int>::iterator currentptr,lastptr;
    currentptr = minDegreeList.begin();
    lastptr = minDegreeList.end();
    while(currentptr!=lastptr){
      cout<<*currentptr<<" ";
      currentptr++;
    }
    cout<<endl;
    hash_set<int> maxDegreeList = degreeList[maxDegree];
    cout<<"Printing maxdegree list\n";
    currentptr = maxDegreeList.begin();
    lastptr = maxDegreeList.end();
    while(currentptr!=lastptr){
      cout<<*currentptr<<" ";
      currentptr++;
    }
    cout<<endl;
    printGraph();
#endif    
    if(minDegreeList.empty()!=true){
      hash_set<int>::iterator curr;
      curr = minDegreeList.begin();
      int v1=*curr;
      int v2=getMaxConnectedVertex(v1);
      int degreeOfV2=0,degreeOfV1=minDegree;
      int changedCount=0;
#ifdef DEBUGMLB
      cout<<"Processing vertex v1= "<<v1<<" v2 = "<<v2<<"\n";
#endif  
      if(v2!=0){
	degreeOfV2=adjMatrix[v2][0]->number;
	list<Edge> temp=getListOfEdges(v1);

	deletedEdges.insert(deletedEdges.end(),temp.begin(),temp.end());
	list<Edge>::iterator tempCurr,tempLast;
	tempCurr = temp.begin();
	tempLast=temp.end();
	changedCount=0;
	while(tempCurr!=tempLast){
	  Edge e = *tempCurr;
#ifdef DEBUGMLB
      cout<<"Deleting edge = "<<e.v1<<" "<<e.v2<<"\n";
#endif  
	  if(e.v1 == v1) e.v1=v2;
	  if(e.v2== v1) e.v2 = v2;
	  if(e.v1 != e.v2){
	    if(adjMatrix[e.v1][e.v2]->value==1){
	      if(e.v1!=v2){
		ChangingVertex temp;
		temp.v=e.v1;
		temp.degree = adjMatrix[e.v1][0]->number;
		changedVertices[changedCount++]=temp;
	      }
	      else{
		ChangingVertex temp;
		temp.v=e.v2;
		temp.degree=adjMatrix[e.v2][0]->number;
		changedVertices[changedCount++]=temp;
	      }
	    }
	    else {
	      addEdge(e.v1,e.v2);
	      addedEdges.push_back(e);
	    }
	  }
	  tempCurr++;
	}
	//delete v1 from the graph and degreeList
	deleteVertex(v1);
	degreeList[degreeOfV1].erase(v1);
	
	// Add v2 to the changedVertices
	changedVertices[changedCount].v=v2;
	changedVertices[changedCount].degree = degreeOfV2;
	changedCount++;

	//update degree of V2
	degreeOfV2=adjMatrix[v2][0]->number;
	
	//Change the position in the degreeList of all changedVertices
	for(j=0;j<changedCount;j++){
	  ChangingVertex currVertex; 
	  currVertex= changedVertices[j];
	  degreeList[currVertex.degree].erase(currVertex.v);

	  //update degree of V2 and insert v2 in proper hash_set
	  degreeList[adjMatrix[currVertex.v][0]->number].insert(currVertex.v);
	
	//update maxDegree and minDegree for the change made
	  if(maxDegree < adjMatrix[currVertex.v][0]->number ) maxDegree = adjMatrix[currVertex.v][0]->number;
	  if(minDegree > adjMatrix[currVertex.v][0]->number) minDegree = adjMatrix[currVertex.v][0]->number; 
	}
      }
      else{
	//delete v1
	deleteVertex(v1);
#ifdef DEBUGMLB
	cout<<"Degree of v1= "<<degreeOfV1<<"\n";
#endif
	degreeList[degreeOfV1].erase(v1);
      }
    }
#ifdef DEBUGMLB
    cout<<"old minDegree = "<<minDegree<<", old maxDegree = "<<maxDegree<<endl;
#endif	
    //update maxDegree and minDegree
    while(degreeList[minDegree].empty() == true)
      minDegree++;
    while(degreeList[maxDegree].empty() == true)
      maxDegree--;
    if(maxMinDegree < minDegree) maxMinDegree = minDegree;
#ifdef DEBUGMLB
    cout<<"new minDegree = "<<minDegree<<", new maxDegree = "<<maxDegree<<endl;
#endif	
  if(minDegree > maxDegree) break;
  }
  // reset the graph by first adding the deleted edges and 
  // then deleting the added edges
  addEdges(deletedEdges);
  deleteEdges(addedEdges);
#ifdef DEBUGMLB
  cout<<"Lower Bound = "<<maxMinDegree<<endl;
#endif
  return maxMinDegree;
}


 
    
int GraphOperations::Graph::computeM2LB()
{

#ifdef DEBUGMLB

#endif  
 
  int i,j;
  int minDegree = numVertices+1;
  int maxDegree = -1;
  int maxMinDegree;
  
  hash_set<int> degreeList[numVertices-1];
  vector<ChangingVertex> changedVertices(numVertices-1);
  for(i=0;i<numVertices-1;i++){
    degreeList[i]=hash_set<int>();
    changedVertices[i].v=0;
    changedVertices[i].degree=0;
  }
  
#ifdef DEBUGMLB
  cout<<"Hash set init\n";
#endif  
  list<Edge> addedEdges,deletedEdges;
  // Initialize Degree List. This is O(V) operation
  VertexList *vList;
  vList = vertex[0]->next;
  while(vList!=NULL){
    int v = vList->number;
    if(minDegree > adjMatrix[v][0]->number) 
      minDegree=adjMatrix[v][0]->number;
    if(maxDegree < adjMatrix[v][0]->number) 
      maxDegree = adjMatrix[v][0]->number;
    degreeList[adjMatrix[v][0]->number].insert(v);
    vList=vList->next;
  }
  maxMinDegree = minDegree;
#ifdef DEBUGMLB
  cout<<"Degree list inited\n";
#endif  

  while(minDegree != maxDegree){
    hash_set<int> minDegreeList = degreeList[minDegree];

#ifdef DEBUGMLB
    cout<<"Printing mindegree list\n";
    hash_set<int>::iterator currentptr,lastptr;
    currentptr = minDegreeList.begin();
    lastptr = minDegreeList.end();
    while(currentptr!=lastptr){
      cout<<*currentptr<<" ";
      currentptr++;
    }
    cout<<endl;
    hash_set<int> maxDegreeList = degreeList[maxDegree];
    cout<<"Printing maxdegree list\n";
    currentptr = maxDegreeList.begin();
    lastptr = maxDegreeList.end();
    while(currentptr!=lastptr){
      cout<<*currentptr<<" ";
      currentptr++;
    }
    cout<<endl;
    printGraph();
#endif    
    if(minDegreeList.empty()!=true){
      hash_set<int>::iterator curr;
      curr = minDegreeList.begin();
      int v1=*curr;
      int v2=getMinConnectedVertex(v1);
      int degreeOfV2=0,degreeOfV1=minDegree;
      int changedCount=0;
#ifdef DEBUGMLB
      cout<<"Processing vertex v1= "<<v1<<" v2 = "<<v2<<"\n";
#endif  
      if(v2!=0){
	degreeOfV2=adjMatrix[v2][0]->number;
	list<Edge> temp=getListOfEdges(v1);

	deletedEdges.insert(deletedEdges.end(),temp.begin(),temp.end());
	list<Edge>::iterator tempCurr,tempLast;
	tempCurr = temp.begin();
	tempLast=temp.end();
	changedCount=0;
	while(tempCurr!=tempLast){
	  Edge e = *tempCurr;
#ifdef DEBUGMLB
      cout<<"Deleting edge = "<<e.v1<<" "<<e.v2<<"\n";
#endif  
	  if(e.v1 == v1) e.v1=v2;
	  if(e.v2== v1) e.v2 = v2;
	  if(e.v1 != e.v2){
	    if(adjMatrix[e.v1][e.v2]->value==1){
	      if(e.v1!=v2){
		ChangingVertex temp;
		temp.v=e.v1;
		temp.degree = adjMatrix[e.v1][0]->number;
		changedVertices[changedCount++]=temp;
	      }
	      else{
		ChangingVertex temp;
		temp.v=e.v2;
		temp.degree=adjMatrix[e.v2][0]->number;
		changedVertices[changedCount++]=temp;
	      }
	    }
	    else {
	      addEdge(e.v1,e.v2);
	      addedEdges.push_back(e);
	    }
	  }
	  tempCurr++;
	}
	//delete v1 from the graph and degreeList
	deleteVertex(v1);
	degreeList[degreeOfV1].erase(v1);
	
	// Add v2 to the changedVertices
	changedVertices[changedCount].v=v2;
	changedVertices[changedCount].degree = degreeOfV2;
	changedCount++;

	//update degree of V2
	degreeOfV2=adjMatrix[v2][0]->number;
	
	//Change the position in the degreeList of all changedVertices
	for(j=0;j<changedCount;j++){
	  ChangingVertex currVertex; 
	  currVertex= changedVertices[j];
	  degreeList[currVertex.degree].erase(currVertex.v);

	  //update degree of V2 and insert v2 in proper hash_set
	  degreeList[adjMatrix[currVertex.v][0]->number].insert(currVertex.v);
	
	//update maxDegree and minDegree for the change made
	  if(maxDegree < adjMatrix[currVertex.v][0]->number ) maxDegree = adjMatrix[currVertex.v][0]->number;
	  if(minDegree > adjMatrix[currVertex.v][0]->number) minDegree = adjMatrix[currVertex.v][0]->number; 
	}
      }
      else{
	//delete v1
	deleteVertex(v1);
#ifdef DEBUGMLB
	cout<<"Degree of v1= "<<degreeOfV1<<"\n";
#endif
	degreeList[degreeOfV1].erase(v1);
      }
    }
#ifdef DEBUGMLB
    cout<<"old minDegree = "<<minDegree<<", old maxDegree = "<<maxDegree<<endl;
#endif	
    //update maxDegree and minDegree
    while(degreeList[minDegree].empty() == true)
      minDegree++;
    while(degreeList[maxDegree].empty() == true)
      maxDegree--;
    if(maxMinDegree < minDegree) maxMinDegree = minDegree;
#ifdef DEBUGMLB
    cout<<"new minDegree = "<<minDegree<<", new maxDegree = "<<maxDegree<<endl;
#endif	
  if(minDegree > maxDegree) break;
  }
  // reset the graph by first adding the deleted edges and 
  // then deleting the added edges
  addEdges(deletedEdges);
  deleteEdges(addedEdges);
#ifdef DEBUGMLB
  cout<<"Lower Bound = "<<maxMinDegree<<endl;
#endif
 
  return maxMinDegree;
}

      
int GraphOperations::Graph::getMinDegree()
{
  int minDegree=numVertices;
  VertexList *vList;
  vList = vertex[0]->next;
  while(vList!=NULL){
    int v = vList->number;
    if(minDegree > adjMatrix[v][0]->number) 
      minDegree=adjMatrix[v][0]->number;
    vList=vList->next;
  }
  return minDegree;
}

int GraphOperations::Graph::getMaxDegree()
{
  int maxDegree=0;
  VertexList *vList;
  vList = vertex[0]->next;
  while(vList!=NULL){
    int v = vList->number;
    if(maxDegree < adjMatrix[v][0]->number) 
      maxDegree = adjMatrix[v][0]->number;
    vList=vList->next;
  }
  return maxDegree;
}

vector<int> GraphOperations::Graph::minDegreeBBOrdering()
  {

    vector<int> ordering(numVertices-1);
    VertexList *vList;
    int i,j;
    vList = vertex[0]->next;
    int count=0;
    int min = numVertices*numVertices;
    int max = 0;
    vector<list<int> >sList(numVertices-1);
    while(vList!=NULL)
      {
        int v = vList->number;
        int key = getDegree(v);
        sList[key].push_back(v);
        vList=vList->next;
        if(key <min)
          min = key;
        if(key >max)
          max = key;
      }
    // Could have implemented sort by using stl but but for speed
    // we use the method shown above
    for(i=min;i<max+1;i++)
      {
        if(sList[i].empty()==true)
          continue;
        list<int>::iterator curr,last;
        curr = sList[i].begin();
        last = sList[i].end();
        int put_size=sList[i].size();
        vector<int> where_to_put(put_size);

        while(curr!=last)
          {
            bool done;
            int temp;
            do
              {
                done=false;
                temp = rand()%(sList[i].size());
                if(where_to_put[temp]!=0)
                  {
                    done=true;
                  }
              }
            while(done);
            where_to_put[temp]=*curr;
            curr++;
          }
        for(j=0;j<put_size;j++)
          ordering[count++]=where_to_put[j];

      }
  }
// Returns a list in which the vertices are sorted based on the number of edges
// added by each in triangulation
std::vector<int> GraphOperations::Graph::minFillBBOrdering()
{
#ifdef DEBUGMINBB
  cout<<"numVertices = "<<numVertices<<endl;
#endif 
  vector<int> ordering(numVertices-1);
  VertexList *vList;
  int i;
  vList = vertex[0]->next;
  int count=0;
  int min = numVertices*numVertices;
  int max = 0;
  list<int> sList[numVertices*(numVertices-1)/2];
  while(vList!=NULL){
    int v = vList->number;
    int key = getNumEdgesAdded(v);
#ifdef DEBUGMINBB
    cout<<"key = "<<key<<" for vertex "<<v<<" 2*n = "<<2*numVertices<<endl;
#endif
    sList[key].push_back(v);
    vList=vList->next;
    if(key <min) min = key;
    if(key >max) max = key;
  }
  if(min >= 2 *numVertices) min =0;
#ifdef DEBUGMINBB
  cout<<"sortedList inited\n";
#endif
  // Could have implemented sort by using stl but but for speed
  // we use the method shown above
  for(i=min;i<max+1;i++){
    if(sList[i].empty()==true) continue;
    list<int>::iterator curr,last;
    curr = sList[i].begin();
    last = sList[i].end();
    while(curr!=last){
#ifdef DEBUGMINBB
      cout<<"Puting vertex "<<*curr<<" having degree "<<i<<" in "<<count<<endl;
#endif
      ordering[count++]=*curr;
      curr++;
    }
  }
  return ordering;
}

int GraphOperations::Graph::getNumEdgesAdded(int v)
{
  EdgeList *p,*q;
  int edgesAdded=0;
  p = adjMatrix[v][0]->next;
  while(p!=NULL){
    q=p->next;
    while(q!=NULL){
      if(adjMatrix[p->number][q->number]->value ==0){
	edgesAdded++;
      }
      q=q->next;
    }
    p=p->next;
  }
  return edgesAdded;
}

bool GraphOperations::Graph::getIfSimplicial(int v)
{
  EdgeList *p,*q;
  p = adjMatrix[v][0]->next;
  while(p!=NULL){
    q=p->next;
    while(q!=NULL){
      if(adjMatrix[p->number][q->number]->value ==0){
	return false;
      }
      q=q->next;
    }
    p=p->next;
  }
  return true;
}
bool GraphOperations::Graph::getIfSimplicial(int v,vector<bool>&potentialSimplicialVertices)
{
  EdgeList *p,*q;
  if(potentialSimplicialVertices[v] ==false) return false;
  p = adjMatrix[v][0]->next;
  while(p!=NULL){
    q=p->next;
    while(q!=NULL){
      if(adjMatrix[p->number][q->number]->value ==0){
	return false;
      }
      q=q->next;
    }
    p=p->next;
  }
  return true;
}
bool GraphOperations::Graph::getIfAlmostSimplicial(int v)
{
  EdgeList *p,*q;
  int badVertex1=-1;
  int badVertex2=-1;
  p = adjMatrix[v][0]->next;
  while(p!=NULL){
    q=p->next;
    while(q!=NULL){
      if(adjMatrix[p->number][q->number]->value ==0){
	if(badVertex1==badVertex2){
	  if(badVertex1==-1){
	    badVertex1=p->number;
	    badVertex2=q->number;
	  }
	  else
	    if(p->number != badVertex1 && q->number !=badVertex1) return false;
	}
	else{
	  if(p->number ==badVertex1)
	    badVertex2=badVertex1;
	  else
	    if(p->number == badVertex2)
	      badVertex1=badVertex2;
	    else
	      if(q->number == badVertex1)
		 badVertex2=badVertex1;
	      else
		if(q->number == badVertex2)
		  badVertex1=badVertex2;
		else
		  return false;
	}   
      }
      q=q->next;
    }
    p=p->next;
  }
  return true;
}
bool GraphOperations::Graph::getIfAlmostSimplicial(int v,vector<bool>&potentialSimplicialVertices)
{
  
  if(potentialSimplicialVertices[v] ==false) return false;
  EdgeList *p,*q;
  int badVertex1=-1;
  int badVertex2=-1;
  p = adjMatrix[v][0]->next;
  while(p!=NULL){
    q=p->next;
    while(q!=NULL){
      if(adjMatrix[p->number][q->number]->value ==0){
	if(badVertex1==badVertex2){
	  if(badVertex1==-1){
	    badVertex1=p->number;
	    badVertex2=q->number;
	  }
	  else
	    if(p->number != badVertex1 && q->number !=badVertex1) return false;
	}
	else{
	  if(p->number ==badVertex1)
	    badVertex2=badVertex1;
	  else
	    if(p->number == badVertex2)
	      badVertex1=badVertex2;
	    else
	      if(q->number == badVertex1)
		badVertex2=badVertex1;
	      else
		if(q->number == badVertex2)
		  badVertex1=badVertex2;
		else
		  return false;
	}   
      }
      q=q->next;
    }
    p=p->next;
  }
  return true;
}

void GraphOperations::Graph::processSimplicialAndAlmostSimplicialVertices(int bound)
{
  int i,j;
  simplicialVertices=list<int>();
  almostSimplicialVertices=list<int>();
  stronglyMutexVertices = list<int>();
  VertexList *vList;
  vList = vertex[0]->next;
  while(vList!=NULL){
    int v = vList->number;
    if(getIfSimplicial(v)){
      simplicialVertices.push_back(v);
      stronglyMutexVertices.push_back(v);
    }
    else 
      if(adjMatrix[v][0]->number <bound && getIfAlmostSimplicial(v)) {
	almostSimplicialVertices.push_back(v);
	stronglyMutexVertices.push_back(v);
      }
    vList=vList->next;
  }
  // Remove all almost simplicial vertices that are connected to each other
  list<int>::iterator curr,curr1,last,temp;
  curr = almostSimplicialVertices.begin();
  last = almostSimplicialVertices.end();
 
  while(curr!=last){
    curr1=curr;
    curr1++;
#ifdef DEBUGSIM
    cout<<"Vertices are "<<*curr<<endl;
    printNeighborhoodGraph(*curr);
#endif
    bool del=false;
    while(curr1!=last){
      if(adjMatrix[*curr][*curr1]->value ==1){
	del = true;
	break;
      }
      curr1++;
    }
    temp = curr;
    curr++;
#ifdef DEBUGSIM
    if(del) cout<<"Deleting "<<*temp<<endl;
#endif
    if(del) almostSimplicialVertices.erase(temp);
    
  }
}

void GraphOperations::Graph::processSimplicialAndAlmostSimplicialVertices(int bound,vector<bool>&potentialSimplicialVertices)
{
  int i,j;
  simplicialVertices=list<int>();
  almostSimplicialVertices=list<int>();
  stronglyMutexVertices = list<int>();
  VertexList *vList;
  vList = vertex[0]->next;
  while(vList!=NULL){
    int v = vList->number;
    if(getIfSimplicial(v,potentialSimplicialVertices)){
      simplicialVertices.push_back(v);
      stronglyMutexVertices.push_back(v);
    }
    else 
      if(adjMatrix[v][0]->number <bound && getIfAlmostSimplicial(v,potentialSimplicialVertices)){
	almostSimplicialVertices.push_back(v);
	stronglyMutexVertices.push_back(v);
      }
    vList=vList->next;
  }
  // Remove all almost simplicial vertices that are connected to each other
  list<int>::iterator curr,curr1,last,temp;
  curr = almostSimplicialVertices.begin();
  last = almostSimplicialVertices.end();
 
  while(curr!=last){
    curr1=curr;
    curr1++;
#ifdef DEBUGSIM
    cout<<"Vertices are "<<*curr<<endl;
    printNeighborhoodGraph(*curr);
#endif
    bool del=false;
    while(curr1!=last){
      if(adjMatrix[*curr][*curr1]->value ==1){
	del = true;
	break;
      }
      curr1++;
    }
    temp = curr;
    curr++;
#ifdef DEBUGSIM
    if(del) cout<<"Deleting "<<*temp<<endl;
#endif
    if(del) almostSimplicialVertices.erase(temp);
    
  }
}

  
void GraphOperations::Graph::printNeighborhoodGraph(int v)
{
  EdgeList *p,*q;
  p=adjMatrix[v][0]->next;
  while(p!=NULL){
    q=adjMatrix[v][0]->next;
    while(q!=NULL){
      cout<<adjMatrix[p->number][q->number]->value<<" ";
      q=q->next;
    }
    cout<<endl;
    p=p->next;
  }
}



 

// Returns a list of connected components of the graph obtained by deleting
// the vertex v and its adjacent vertices
std::list< GraphOperations::Graph> GraphOperations::Graph::
getConnectedComponents(int v)
{
  // Delete the vertex v and its adjacent vertices from the graph
  list<Edge> edgesDeleted;
  EdgeList *p,*q;
  p=adjMatrix[v][0]->next;
  while(p!=NULL){
    Edge e;
    e.v1=p->number;
    e.v2=v;
    edgesDeleted.push_back(e);
    q=p->next;
    while(q!=NULL){
      e.v1=p->number;
      e.v2=q->number;
      edgesDeleted.push_back(e);
      q=q->next;
    }
#ifdef DEBUGCON
    //cout<<"Deleting vertex "<<p->number<<endl;
#endif
    p=p->next;
  }
#ifdef DEBUGCON
  //cout<<"Deleting vertex "<<v<<endl;
#endif
  deleteEdges(edgesDeleted);
  deleteVertex(v);
#ifdef DEBUGCON
  cout<<"Vertices Deleted,  new numVertices = "<<numVertices<<endl;
#endif
  // Implementing algorithm from CLR
  vector<int> parent(adjMatrix.size());
  vector<int> rank(adjMatrix.size());
  VertexList *vList1,*vList2;
  vList1=vertex[0]->next;
  while(vList1!=NULL){
    parent[vList1->number]=vList1->number;
    rank[vList1->number]=0;
    vList1=vList1->next;
  }
#ifdef DEBUGCON
  cout<<"Initialized parent set and rank  "<<endl;
#endif
  vList1=vertex[0]->next;
  while(vList1!=NULL){
    EdgeList *eList;
    eList = adjMatrix[vList1->number][0]->next;
    int u=vList1->number;
    while(eList!=NULL){
      int v = eList->number;
#ifdef DEBUGCON
      //cout<<"Processing edge between "<<u<<" and "<<v<<endl;
#endif
      if(Util::find_set_connected_components(parent,u)
	 !=Util::find_set_connected_components(parent,v))
	Util::union_set_connected_components(rank,parent,u,v);
#ifdef DEBUGCON
      //cout<<"Finished Processing edge between "<<u<<" and "<<v<<endl;
#endif
      eList=eList->next;
    }
    vList1=vList1->next;
  }
#ifdef DEBUGCON
  cout<<"Now finding components  "<<endl;
#endif
  vector<vector<int> > sameComponent;
  vector<bool> processed(adjMatrix.size());
  vList1=vertex[0]->next;
  while(vList1!=NULL){
    vector<int> temp;
    vList2=vList1->next;
    if(processed[vList1->number]) {
      vList1=vList1->next;
      continue;
    }
    processed[vList1->number]=true;
    temp.push_back(vList1->number);
    while(vList2!=NULL){
      if(Util::find_set_connected_components(parent,vList1->number) ==
	 Util::find_set_connected_components(parent,vList2->number)){
	temp.push_back(vList2->number);
	processed[vList2->number]=true;
      }
      vList2=vList2->next;
    }
    sameComponent.push_back(temp);
    vList1=vList1->next;
  }
#ifdef DEBUGCON
  cout<<"#components ="<<sameComponent.size()<<endl;
  cout<<"Printing components\n";
  for(int i=0;i<sameComponent.size();i++){
    cout<<"component "<<i<<" has vertices ";
    for(int j=0;j<sameComponent[i].size();j++)
      cout<<sameComponent[i][j]<<" ";
    cout<<endl;
  }
#endif
  addEdges(edgesDeleted);
#ifdef DEBUGCON
  cout<<"Graph restored"<<endl;
  //printGraph();
#endif
  list<Graph> tmp;
  return tmp;
}
   
list<Edge> GraphOperations::Graph::triangulate(int v)
{
  list<Edge> edgesAdded;
  EdgeList *p,*q;
  p=adjMatrix[v][0]->next;
  while(p!=NULL){
    q=p->next;
    while(q!=NULL){
      if(adjMatrix[p->number][q->number]->value==0){
	Edge e;
	e.v1=p->number;
	e.v2=q->number;
	edgesAdded.push_back(e);
	addEdge(e.v1,e.v2);
      }
      q=q->next;
    }
    p=p->next;
  }
  return edgesAdded;
}
      
bool GraphOperations::Graph::ifSameEdges(int v, list<int> &edges)
{
  list<int>::iterator curr,last;
  curr=edges.begin();
  last=edges.end();
  int count=0;
  while(curr!=last){
    if(adjMatrix[*curr][v]->value !=1) return false;
    count++;
    curr++;
  }
  return true;
}

void GraphOperations::Graph::getPotentialSimplicialVertices
(int v, 
 vector<bool> &potentialSimplicialVertices)
{
  
  potentialSimplicialVertices[v]=true;
  list<int>::iterator curr,last;
  list<int> vList,vList1;
  vList = getIntListOfEdges(v);
  curr=vList.begin();
  last=vList.end();
  while(curr!=last){
    vList1=getIntListOfEdges(*curr);
    potentialSimplicialVertices[*curr]=true;
    list<int>::iterator cur,lst;
    cur=vList1.begin();
    lst=vList1.end();
    while(cur!=lst){
      potentialSimplicialVertices[*cur]=true;
      cur++;
    }
    curr++;
  }
  
}
  
  
  
  

	
  
  
  

	
	
	
    
	
      
      
  
  
  
  
  
