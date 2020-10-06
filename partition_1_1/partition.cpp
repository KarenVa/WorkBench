//----------------------------------------------------*- C++ -*-------------------//
// file: partition.h
// author: xb1zhao, lffan
// purpose: describe functions of DoCluster, InitialPartition and Refinement
//--------------------------------------------------------------------------------//

#include "partition.h"

using namespace std;

//===----------------------------------------------------------------------===//
//                               Cell Functions
//===----------------------------------------------------------------------===//
Cell::Cell(string name)
  : name_(name),
    gain_(0),
    locked_(false),
    dirty_(false),
    owner_part_(0),
    best_part_(0)
  {
    // for(int i=0;i<10;i++)
    //   resource_[i]=0;
    memset(resource_,0,10); //faster than for loop
  }

//===----------------------------------------------------------------------===//
//                               Driver Functions
//===----------------------------------------------------------------------===//
Driver::Driver(int pass_num, int initial_type, float ubf)
  : pass_num_(pass_num),
    initial_type_(initial_type),
    ub_factor_(ubf)
  {}

Graph* Driver::readCell(string cell_file_path, Graph* G0){
  ifstream rc(cell_file_path);
  assert(rc.is_open()&& " Can't find Cell file");
  cout << "Reading Cell_file ......" <<endl;
  while(!rc.eof()){
    string cell_name;
    int resource[10];
    rc >> cell_name;
    if(cell_name == "") break;
    for(int i=0;i<10;i++) {rc>>resource[i];}    
    Cell* cell = new Cell(cell_name);
    for(int i=0;i<10;i++){
      G0->partition_area_[0][i]+=resource[i];
    }
    G0->cells_.push_back(cell);
    cell_map_[cell_name] = cell;
  }
  rc.close();
  cout << "Read Cells complete!" <<endl;
  return G0;
}

Graph* Driver::readNet(string net_file_path, Graph* G0){
  ifstream read_net(net_file_path);
  assert(read_net.is_open() && "Can't find Net file");
  cout << "Reading Net_file ......" <<endl;
  string cell_name;
  char driver_;
  int weight;
  read_net>>cell_name>>driver_;
  while(!read_net.eof())
  {
    read_net>>weight;
    Net* net = new Net(cell_name,weight);
    Pin* pin = new Pin(cell_name,driver_=='s');
    pin->connector_ = net;
    cell_map_[cell_name]->pin_list_.push_back(pin);   //push pin in cell_pins
    net->pin_list_.push_back(pin);                    //push pin in net_pins

    read_net >> cell_name >> driver_;
    while(driver_ == 'l'){
      Pin* pin = new Pin(cell_name,driver_=='s');
      pin->connector_ = net;
      cell_map_[cell_name]->pin_list_.push_back(pin);
      net->pin_list_.push_back(pin);
      
      if(!(read_net >> cell_name >> driver_)) break;
      
    }
    G0->nets_.push_back(net);
  }
  read_net.close();
  cout << "Read Net complete!" <<endl;
  
  return G0;
}

class doCluster{

};

class doInitialPartition{

};

class doRefinement{

};
