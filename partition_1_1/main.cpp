//----------------------------------------------------*- C++ -*-------------------//
// file: main.cpp
// author: xb1zhao, lffan
// purpose: the top file of hMetis Circuit Partitioning @FMSH
// usage: 
//--------------------------------------------------------------------------------//
#include "DataStruct.h"
#include <string.h>
#include <ctime>
#include <fstream>
#include <string>
using namespace std;

int main(){
  clock_t st,et;
  st=clock();
  Graph* gp1= new Graph("G0",1);
  Driver dr(5,1,.5);
  gp1=dr.readCell("./design.are",gp1);
  gp1=dr.readNet("./design.net",gp1);
  int k=gp1->nets_.size();
  cout <<"Num of cells : " <<k <<endl;
  for(int i=0; i<k;i++){
    for(int j=0;j<gp1->nets_[i]->pin_list_.size();j++){
      cout << gp1->nets_[i]->pin_list_[j]->name_ << " " << gp1->nets_[i]->pin_list_[j]->driver_ <<endl;
    }
  }
  cout <<endl;
  et=clock();

  cout << "Time use : " << (double)(et-st)/CLOCKS_PER_SEC << "s" <<endl;
  return 0;
}
