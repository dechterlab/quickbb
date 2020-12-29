/***************************************************************************
 *   Copyright (C) 2004 by Vibhav Gogate                                   *
 *   vgogate@ics.uci.edu                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include <iostream>
#include <cstdlib>
#include <fstream>
#include "Graph.h"
#include "SearchAlgorithms.h"
#include "Generate.h"
#include <getopt.h>

using namespace std;
using namespace GraphOperations;
using namespace Generate;
using namespace SearchAlgorithms;
Statistics stat; 
ORDERING_HEURISTIC ordering_heuristic;
static struct option long_options[]=
    {
      {"min-degree-ordering",no_argument,NULL,'a'
      },
      {"min-fill-ordering",no_argument,NULL,'b'},
      {"random-ordering",no_argument,NULL,'c'},
      {"lb",no_argument,NULL,'d'},
      {"time",required_argument,NULL,'e'},
      {"cnffile",required_argument,NULL,'f'},
      {"cgfile",required_argument,NULL,'g'},
      {"help", no_argument,NULL,'h'},
      {"outfile",required_argument,NULL,'i'},
      {"statfile",required_argument,NULL,'j'},
      {"random-graph",no_argument,NULL,'k'}
    };

static const char OPTION_STRING[]="abcde:f:g:hi:j:k";

void print_help()
{
  cout<<" Help: There are 3 ways to use this program \n";
  cout<<" 1. given a cnf file: Use quickbb_1 [optional1] [optional2] --cnffile CNFFILE\n";
  cout<<" 2. given a cg file: Use quickbb_1 [optional1] [optional2] --cgfile CGFILE\n";
  cout<<" 3. For random-graphs: Use quickbb_1 [optional1] [optional2] --random-graph Filename #vertices #edges\n";
  cout<<"   [optional1] should be one of the following; if all are included the final one is the one used\n";
  cout<<"     a. --random-ordering: Branching performed in quickbb using random ordering\n";
  cout<<"     b. --min-fill-ordering: Branching performed in quickbb using min-fill ordering\n";
  //cout<<"     c. --min-degree-ordering: Branching performed in quickbb using min-fill ordering\n";
  cout<<"   [optional2] can be any of the following or combination thereof\n";
  cout<<"     a. --time int: program is run for int seconds\n";
  cout<<"     b. --lb: Computes lb of the graph\n";
  cout<<"     c. --outfile filename: Store the output like treewidth,lb and ordering in filename\n";
  cout<<"     d. --statfile filename: Store the output in a table form in which rows are separated by spaces\n";
  cout<<"                     Rows have the following format, row1:#vertices,row2:#edges\n";
  cout<<"                     row3: correct lb if --lb is used or 0, row4: treewidth until diffTime\n";
  cout<<"                     row5: Time elapsed, row6:Nodes Visited, row7:Nodes Pruned\n";
  cout<<"                     row8:1 if optimal 0 if not\n";

  exit(1);

}

int main(int argc, char *argv[])
{
  bool output_lb=false;
  long int time_bound=100000;
  ofstream statfile,outfile;
  char infilename[80];
  vector<vector<int> > matrix;
  Graph g;
  int n;
  int e;
  int bound;
  struct timeval startTime,endTime;
  struct timezone tz;
  Search s;
  if(argc==1)
    {
      print_help();
      return 1;
    }
  while(1)
    {

      int option_index =0;
      int opt = getopt_long(argc,argv,OPTION_STRING,long_options,&option_index);
      // Detect end of options
      if (opt == -1)
        break;
      switch(opt)
        {
        case 'a':
          ordering_heuristic=MINDEGREE;
          break;
        case 'b':
          ordering_heuristic=MINFILL;
          break;
        case 'c':
          ordering_heuristic=RANDOM;
          break;
        case 'd':
          output_lb=true;
          break;
        case 'e':
          sscanf(optarg,"%ld",&time_bound);
          break;
        case 'f':
          matrix = RandomGraph::readCnf(optarg);
          g = Graph(matrix);
          n = g.getNumVertices();
          strncpy(infilename,optarg,80);
          break;
        case 'g':
          matrix = RandomGraph::readGraph(optarg);
          g = Graph(matrix);
          n = g.getNumVertices();
          strncpy(infilename,optarg,80);
          break;
        case 'h':
          print_help();
          return 1;
        case 'i':
          outfile.open(optarg,fstream::app);
          break;
        case 'j':
          statfile.open(optarg,fstream::app);
          break;
        case 'k':
          strncpy(infilename,argv[optind],80);
          optind++;
          sscanf(argv[optind],"%d",&n);
          optind++;
          sscanf(argv[optind],"%d",&e);
          matrix=RandomGraph::generateGraph(infilename,n,e);
          g = Graph(matrix);

          break;
        }
    }

  e=g.getNumEdges();
  int lb=0;
  if(output_lb)
    lb=g.computeM2LB();
  bound = n;
  srand((unsigned)time(0));
  gettimeofday(&startTime,&tz);
  bound = s.branchAndBound(g,bound,time_bound);
  gettimeofday(&endTime,&tz);
  double diffTime=(endTime.tv_sec-startTime.tv_sec)+(double)(endTime.tv_usec - startTime.tv_usec)/(double)1000000;
  if(outfile)
    {
      outfile<<" The treewidth of the graph in the file "<<infilename<<" is "<<bound<<endl;
      if(output_lb)
        outfile<<" The lowerbound for the graph is "<<lb<<endl;

      outfile<<" Time required = "<< diffTime<<endl;
      outfile<<" The treewidth is ";
      if(s.getOpt())
        outfile<<"optimal\n";
      else
        outfile<<"not optimal\n";
      outfile<<" The optimal ordering is \n";
      Util::printVector(s.bestOrdering,outfile);
    }
  if(statfile)
    {
      statfile<<n<<" "<<e<<" "<<lb<<" "<<bound<<" "<<diffTime<<" "<<s.getNodesVisited()<<" "<<s.getNodesPruned()<<" "<<s.getOpt()<<endl;
    }
  cout<<"Treewidth= "<<bound<<endl;
  if(output_lb)
    outfile<<" The lowerbound for the graph is "<<lb<<endl;
}








