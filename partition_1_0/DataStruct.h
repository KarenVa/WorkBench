//----------------------------------------------------*- C++ -*-------------------//
// file: Datastruct.h
// author: xb1zhao, lffan
// purpose: define the Data Model of Netlist for partitioning
//--------------------------------------------------------------------------------//

#ifndef DATA_STRUCT_H_
#define DATA_STRUCT_H_

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <string.h>
#include <stdlib.h>

using namespace std;
#define K 2

class Net;
class Pin;
class Cluster;
class Driver;
//===----------------------------------------------------------------------===//
//                               Cell class
//===----------------------------------------------------------------------===//
class Cell{
public:
  Cell(string name);
  ~Cell(){}
public:
  vector <Pin*> pin_list_;

  void changePartition(int target){};
  void updateGain(int gain){};

public:
  string name_;
  int gain_;
  int resource_[10];   //use Sparse representation to replace it
  bool locked_;
  bool dirty_;

  int owner_part_;    //partition which the cell belongs to
  int best_part_;     //best partition belongs to

  multimap<int,Cell*>::iterator gain_itr_;
};

//===----------------------------------------------------------------------===//
//                               Cluster class
//===----------------------------------------------------------------------===//
class Cluster : public Cell{
public:
  Cluster(vector<Cell*> members);
  ~Cluster();

  friend class Net;
public:
  vector <Cell*> members_;
};

class Net;
class Cell;
class Driver;
//===----------------------------------------------------------------------===//
//                               Pin class
//===----------------------------------------------------------------------===//
class Pin{
public:
  Pin(string name, bool driver)
  : name_(name),
    driver_(driver){}
  ~Pin(){}

  friend class Net;
  friend class Cell;
  friend class Driver;
public:
  string name_;
  bool driver_;     //sink/source : false/true
  //int width_;       //two or more edge can combine to a wide edge
  Net* connector_;  //connector is the Net connected with current pin
};

class Cell;
class Pin;
//===----------------------------------------------------------------------===//
//                               Net class
//===----------------------------------------------------------------------===//
class Net{
public:
  Net(string name, int weight)
  : name_(name),
    weight_(weight),
    cut_state_(false),
    dirty_(false){
      for(int i=0;i<K;i++) lock_partition_[i]=false;
      for(int i=0;i<K;i++) lock_partition_[i]=false;
    }
  ~Net(){}
  
public:
  string name_;
  //int pin_num_;     //the num of pins connected with the Net
  int weight_;
  bool cut_state_;
  bool dirty_;
  //bool critical_;
  bool lock_partition_[K];
  int partition_cell_num_[K]; //the num of its cells in a partition area   

  vector <Pin*> pin_list_;    //the pin connected with the net
} ;

//===----------------------------------------------------------------------===//
//                               Hypergraph class
//===----------------------------------------------------------------------===//
//to store every level's Coarsening Graph struct
class Graph{
public:
  Graph(string name, int level)
  : name_(name),
    level_(level),
    cut_size_(0),
    mini_cut_size_(0){
      for(int i=0;i<2;i++){
        for(int j=0;j<10;j++){
          partition_area_[i][j]=0;
        }
      }
    }

public:
  int level_;//the coarsen level of this graph
  string name_;
  vector<Cell*> cells_;
  vector<Pin*> pins_;
  vector<Net*> nets_;
  int partition_area_[K][10];
  int cut_size_;
  int mini_cut_size_;
};

//===----------------------------------------------------------------------===//
//                               Driver class
//===----------------------------------------------------------------------===//
class Driver{
public:
  Driver(int pass_num, int initial_type, float ubf);
  ~Driver(){};

  //Initial Partition step
  Graph* readNet(string net_file_path, Graph* G0);
  Graph* readCell(string cell_file_path, Graph* G0);
  void doInitialPartition();
  void computeGain();
  void computeCutsize();

public:
  void RandomPartition();
  void RegionGrowPartition();
public:
  int pass_num_;
  int initial_type_;
  float ub_factor_;
  map <string, Cell*> cell_map_;
};

#endif