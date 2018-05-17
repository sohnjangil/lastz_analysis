#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>

using namespace std;

class lastz_general{
private:
  long score;
  string name1;
  string strand1;
  long size1;
  long zstart1;
  long end1;
  string name2;
  string strand2;
  long size2;
  long zstart2;
  long end2;
  string identity;
  string idPct;
  string coverage;
  string covPct;
public:
  lastz_general(){};
  lastz_general(  long _score,
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
		  string _covPct){
    score=_score;
    name1=_name1;
    strand1=_strand1;
    size1=_size1;
    zstart1=_zstart1;
    end1=_end1;
    name2=_name2;
    strand2=_strand2;
    size2=_size2;
    zstart2=_zstart2;
    end2=_end2;
    identity=_identity;
    idPct=_idPct;
    coverage=_coverage;
    covPct=_covPct;

  };

  lastz_general( string * _input ){
    score=strtol(_input[0].c_str(),NULL,0);
    name1=_input[1];
    strand1=_input[2];
    size1=strtol(_input[3].c_str(),NULL,0);
    zstart1=strtol(_input[4].c_str(),NULL,0);
    end1=strtol(_input[5].c_str(),NULL,0);
    name2=_input[6];
    strand2=_input[7];
    size2=strtol(_input[8].c_str(),NULL,0);
    zstart2=strtol(_input[9].c_str(),NULL,0);
    end2=strtol(_input[10].c_str(),NULL,0);
    identity=_input[11];
    idPct=_input[12];
    coverage=_input[13];
    covPct=_input[14];

  };
  ~lastz_general(){};

  void get( string * _input ){
    score=strtol(_input[0].c_str(),NULL,0);
    name1=_input[1];
    strand1=_input[2];
    size1=strtol(_input[3].c_str(),NULL,0);
    zstart1=strtol(_input[4].c_str(),NULL,0);
    end1=strtol(_input[5].c_str(),NULL,0);
    name2=_input[6];
    strand2=_input[7];
    size2=strtol(_input[8].c_str(),NULL,0);
    zstart2=strtol(_input[9].c_str(),NULL,0);
    end2=strtol(_input[10].c_str(),NULL,0);
    identity=_input[11];
    idPct=_input[12];
    coverage=_input[13];
    covPct=_input[14];
  };

  long return_zstart1(){
    return zstart1;
  };
  long return_end1(){
    return end1;
  };
  long return_zstart2(){
    return zstart2;
  };
  long return_end2(){
    return end2;
  };

  string return_name1(){
    return name1;
  };

  string return_name2(){
    return name2;
  }

  string return_strand2(){
    return strand2;
  }
  long return_size2(){
    return size2;
  }
};


void hello(){
  cout << "lastz2gnuplot <infile> [chromosome [scaffold]]" << endl;
}


int main(int argc, char ** argv){

  if ( argc<2 ) {
    hello();
    return 0 ;
  }
  
  string infile=argv[1];

  string chromosome=argv[2];
  string scaffold;
  if(argc==4) scaffold=argv[3];

  string outfile=infile+".gnuplot";

  ifstream fin(infile.c_str());

  vector < lastz_general > mapping;
  lastz_general map_tmp;

  string tmp;

  getline(fin,tmp);

  string field[15];
  //for ( int i = 0 ; i < 15 ; i ++ ) field.push_back();
  size_t m=0;
  while(fin >> field[0]){
    for(int i = 1 ; i < 15 ; i ++ ) {
      fin >> field[i] ;
    }
    map_tmp.get(field);
    if(map_tmp.return_name1()==chromosome){
      tmp = map_tmp.return_name2();

      if( argc == 4 ){
	//cout << tmp.c_str() << "\t" << scaffold.c_str() << "\t" << argc << endl;	  
	if( !strcmp( tmp.c_str() ,scaffold.c_str() ) ){
	  
	  mapping.push_back(map_tmp);
	  m++;
	}
      }
      else{
	mapping.push_back(map_tmp);
	m++;
      }
    }
  }

  
  for ( size_t i = 0 ; i < mapping.size() ; i ++ ){
    if ( mapping[i].return_strand2() == "+" ){
      cout << "#" <<mapping[i].return_name1() << "\t" << mapping[i].return_name2() << "\t" << mapping[i].return_strand2() << endl;
      cout << mapping[i].return_zstart1() << "\t" << mapping[i].return_zstart2() << endl ;
      cout << mapping[i].return_end1() << "\t" << mapping[i].return_end2() << endl << endl ;
    }else{ 
      cout << "#" <<mapping[i].return_name1() << "\t" << mapping[i].return_name2() << "\t" << mapping[i].return_strand2() << endl;
      //cout << mapping[i].return_end1() << "\t" <<  mapping[i].return_size2() - mapping[i].return_zstart2() << endl << endl ;
      //cout << mapping[i].return_zstart1() << "\t" << mapping[i].return_size2() - mapping[i].return_end2() << endl ;

      //cout << mapping[i].return_zstart1() << "\t" << mapping[i].return_size2() - mapping[i].return_end2() << endl ;
      //cout << mapping[i].return_end1() << "\t" << mapping[i].return_size2() - mapping[i].return_zstart2() << endl << endl ;

      cout << mapping[i].return_end1() << "\t" << mapping[i].return_size2() - mapping[i].return_end2() << endl ;
      cout << mapping[i].return_zstart1() << "\t" << mapping[i].return_size2() - mapping[i].return_zstart2() << endl << endl ;
    }
  }
  return 0;
}
