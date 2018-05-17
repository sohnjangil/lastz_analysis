#ifndef LGF_CLASS
#define LGF_CLASS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <map>
#include <vector>
#include <list>
#include <utility> 
#include <algorithm>
#include <set>


using namespace std;

class LGF{
public:
  long score;     // 0
  string name1;   // 1 target
  string strand1; // 2
  long size1;     // 3
  long zstart1;   // 4
  long end1;      // 5
  string name2;   // 6 query
  string strand2; // 7
  long size2;     // 8
  long zstart2;   // 9
  long end2;      // 10 
  string identity;// 11
  string idPct;   // 12
  string coverage;// 13
  string covPct;  // 14

  LGF(){};
  LGF(  long _score,
	string _name1,
	string _strand1,
	long _size1,
	long _zstart1,
	long _end1,
	string _name2,
	string _strand2,
	long _size2,
	long _zstart2,
	long _end2,
	string _identity,
	string _idPct,
	string _coverage,
	string _covPct);

  LGF( string * _input );
  ~LGF(){};

  void get( string * _input );
  void calibrate_position();  
  long TargetStart();
  long TargetEnd();
  
  long TargetSize();
  
  long QueryStart();
  long QueryEnd();
  long QuerySize();

  string TargetName();

  string QueryStrand();

  string QueryName();

  double Identity();

  double Coverage();

  long TargetLength();
  long QueryLength();

  void Print();

  string ToString();
  
  bool operator == (const LGF & a);

  LGF FlipTargetQuery();

  bool empty();
  
};



#endif
