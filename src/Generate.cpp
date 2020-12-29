/***************************************************************************
   Generate.cpp  -  description
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
#include "Generate.h"
#include "Graph.h"
#include <vector>

namespace Generate
  {

  using namespace std;
  using namespace GraphOperations;
  vector<vector<int> > RandomGraph::generateGraph (char *filename, int n, int e)
  {

    // Temporary variables
    int temp,duplicate;
    int i,j;

    // Initialize the file to be written to
    ofstream outfile,outfile_kirill;

    outfile.open(filename);
    outfile_kirill.open("kirill_graph");

    // Write the #vertices and the #edges to the outfile
    outfile<<n<<"\t"<<e<<endl;
    outfile_kirill<<n<<endl;
    srand((unsigned)time(0));
    //cout<<"Writing stuff\n";


    /* A temporary variable for storing the graph */
    vector <vector <int> > graph;

    /* Initialize the graph to contain n vertices */
    graph = vector<vector <int> > (n+1);

    for(i=0;i<n+1;i++)
      graph[i]=vector<int>(n+1);


    /*
         Initialize the graph to contain no edges
         If there is an edge between two vertices i and j graph[i][j] =1
         else graph[i][j]=0
    */
    for(i=0;i<n;i++)
      for(j=0;j<n;j++)
        graph[i][j]=0;


    /*  Generate edges uniformly at random */
    for(i=0;i<e;i++)
      {
        do
          {
            duplicate = false;
            temp = rand()% (n*n);

            /* Discard if it results in an edge between the same vertices */
            if(temp/n == temp%n)
              {
                duplicate = true;
              }

            /* If the edge is already present make duplicate true */
            if(graph[temp/n][temp%n] == 1)
              duplicate=true;
            if(duplicate == false)
              graph[temp/n][temp%n]= graph[temp%n][temp/n]=1;
          }
        while(duplicate);
        outfile<<temp%n+1<<"\t"<<temp/n+1<<endl;
        outfile_kirill<<temp%n<<"\t"<<temp/n<<endl;
      }

    outfile.close();
    outfile_kirill.close();

    return graph;
  }

  vector<vector<int> > RandomGraph::readGraph(char *filename)
  {

    /* Temporary vertices to hold each edge */
    int v1,v2;
    int i,j,edges,n;

    /* Open the input File */
    ifstream inFile;
    ofstream outfile_kirill("kirill_graph");

    inFile.open(filename);
    if (inFile.fail())
      {
        cerr << "Unable to open file input file "<<filename<<" for reading"<< endl;
        exit(1);
      }


    inFile>>n;
    inFile>>edges;

    outfile_kirill<<n<<endl;
    vector<vector<int> > graph;
    /* Initialize the graph to contain n vertices */
    graph = vector<vector <int> > (n+1);

    for(i=0;i<n+1;i++)
      graph[i]=vector<int>(n+1);


    /*
         Initialize the graph to contain no edges
         If there is an edge between two vertices i and j graph[i][j] =1
         else graph[i][j]=0
    */
    for(i=0;i<n;i++)
      for(j=0;j<n;j++)
        graph[i][j]=0;

    /* Read the file into the graph */
    for(i=0;i<edges;i++)
      {
        inFile>>v1;
        inFile>>v2;
        graph[v1-1][v2-1]=1;
        graph[v2-1][v1-1]=1;
        outfile_kirill<<v1-1<<"\t"<<v2-1<<endl;
      }
    return graph;
  }
  vector<vector<int> > RandomGraph::readCnf(char *filename)
  {
    int lastc;
    int nextc;
    ifstream inFile;
    string s;
    int n,e;
    int i,j,k;

    inFile.open(filename);
    if (inFile.fail())
      {
        cerr << "Unable to open file input file "<<filename<<" for reading"<< endl;
        exit(1);
      }

    while ((lastc = inFile.get()) == 'c')
      {
        while ((nextc = inFile.get()) != EOF && nextc != '\n')
        { }
      }
    inFile.unget();
    inFile>>s;
    if(s != "p")
      {
        fprintf(stderr,"Bad input file: wrong p line\n");
        exit(-1);
      }
    inFile>>s;
    if(s != "cnf")
      {
        fprintf(stderr,"Bad input file: wrong p line\n");
        exit(-1);
      }
    inFile>>n>>e;
    vector<vector<int> > matrix(n+1);
    for(i=0;i<n+1;i++)
      matrix[i]=vector<int>(n+1);
    for(int i=0;i<e;i++)
      {
        vector<int> clique;
        int temp=1;
        while(temp!=0)
          {
            inFile>>temp;
            if(temp <0)
              temp=-temp;
            if(temp!=0)
              clique.push_back(temp);
          }
        for(int j=0;j<clique.size();j++)
          for(int k=j+1;k<clique.size();k++)
            {
              matrix[clique[j]-1][clique[k]-1]=1;
              matrix[clique[k]-1][clique[j]-1]=1;
            }
      }

    return matrix;
  }



  vector<vector<int> > RandomGraph::generateKTree (char *filename, int n, int size, int percent)
  {
    // n is the number of vertices that the k-tree has
    // size is the size of the clique
    // e is the number of edges to be removed from the k-tree

    // Temporary variables
    int temp;
    int i,j;

    // Initialize the file to be written to
    ofstream outfile,outfile_kirill;

    outfile.open(filename);
    outfile_kirill.open("kirill_graph");

    srand((unsigned)time(0));
    //cout<<"Writing stuff\n";


    /* A temporary variable for storing the graph */
    vector <vector <int> > graph;

    /* Initialize the graph to contain n vertices */
    graph = vector<vector <int> > (n);

    for(i=0;i<n;i++)
      graph[i]=vector<int>(n);


    /*
         Initialize the graph to contain no edges
         If there is an edge between two vertices i and j graph[i][j] =1
         else graph[i][j]=0
    */
    for(i=0;i<n;i++)
      for(j=0;j<n;j++)
        graph[i][j]=0;

    vector<vector<int> > cliques;
    // The way we generate k-tree is by first forming a clique of size k+1
    // Then taking a new vertex and making it adjacent to a clique of size k.


    // form the first clique
    vector<int> tempVector;
    for(int i=0;i<size;i++)
      tempVector.push_back(i);
    cliques.push_back(tempVector);


    for(int i=size;i<n;i++)
      {
        size_t cliqueSize = cliques.size();
        temp=rand()%cliqueSize;
        for(int j=0;j<size;j++)
          {
            tempVector = vector<int>();
            for(int k=0;k<size;k++)
              {
                if(k==j)
                  {
                    tempVector.push_back(i);
                  }
                else
                  tempVector.push_back(cliques[temp][k]);
              }
            cliques.push_back(tempVector);
            //printVector(tempVector);
          }
      }
    for(int i=0;i<cliques.size();i++)
      {
        vector<int>::iterator curr,curr1,last;
        curr=cliques[i].begin();
        last=cliques[i].end();
        while(curr!=last)
          {
            curr1=curr;
            curr1++;
            while(curr1!=last)
              {
                graph[*curr][*curr1]=1;
                graph[*curr1][*curr]=1;
                curr1++;
              }
            curr++;
          }
      }
    cerr<<" Graph writte\n";
    int e=0;
    vector<Edge> edges;
    for(int i=0;i<n;i++)
      for(int j=i+1;j<n;j++)
        if(graph[i][j]==1)
          {
            e++;
            Edge edge;
            edge.v1=i;
            edge.v2=j;
            edges.push_back(edge);
          }
    int count=e*(percent)/100;
    e=e-count;

    int temp5=0;
    for(int i=0;i<count;i++)
      {
        bool dup;
        do
          {
            dup=false;
            temp=rand()%edges.size();
            if(edges[temp].v1<0)
              dup=true;
            else
              {
                graph[edges[temp].v1][edges[temp].v2]=0;
                graph[edges[temp].v2][edges[temp].v1]=0;
                edges[temp].v1=-1;
                edges[temp].v2=-1;
              }
          }
        while(dup);
      }
    cerr<<"Done\n";
    outfile<<n<<"\t"<<e<<endl;
    outfile_kirill<<n<<endl;
    for(int i=0;i<n;i++)
      {
        for(int j=i+1;j<n;j++)
          {
            if(graph[i][j]==1)
              {
                outfile<<i+1<<"\t"<<j+1<<endl;
                outfile_kirill<<i<<"\t"<<j<<endl;
              }
          }
      }

    return graph;
  }


}







