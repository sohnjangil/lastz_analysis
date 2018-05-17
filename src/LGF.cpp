#include "LGF.hpp"


LGF::LGF(  long _score,
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

LGF::LGF( string * _input ){

  get ( _input );
  
  // score=strtol(_input[0].c_str(),NULL,0);
  // name1=_input[1];
  // strand1=_input[2];
  // size1=strtol(_input[3].c_str(),NULL,0);
  // zstart1=strtol(_input[4].c_str(),NULL,0);
  // end1=strtol(_input[5].c_str(),NULL,0);
  // name2=_input[6];
  // strand2=_input[7];
  // size2=strtol(_input[8].c_str(),NULL,0);
  // zstart2=strtol(_input[9].c_str(),NULL,0);
  // end2=strtol(_input[10].c_str(),NULL,0);
  // identity=_input[11];
  // idPct=_input[12];
  // coverage=_input[13];
  // covPct=_input[14];

};

void LGF::get( string * _input ){
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


void LGF::calibrate_position(){
    if ( strand2 == "-" ){
      long _zstart2 = size2 - end2 ;
      long _end2 = size2 - zstart2 ;
      zstart2 = _end2;
      end2 = _zstart2;
    }
  };

  
long LGF::TargetStart(){
    return zstart1;
  };
long LGF::TargetEnd(){
    return end1;
  };

long LGF::TargetSize(){
    return size1;
  };
  
long LGF::QueryStart(){
    return zstart2;
  };
long LGF::QueryEnd(){
    return end2;
  };
long LGF::QuerySize(){
    return size2;
  };

string LGF::TargetName(){
    return name1;
  };

string LGF::QueryStrand(){
    return strand2;
  }

string LGF::QueryName(){
    return name2;
  }

double LGF::Identity(){
    size_t pos = identity.find ( "/" );
    return stod(identity.substr(0, pos)) / stod( identity.substr(pos+1) ) ;
  }

double LGF::Coverage(){
    size_t pos = coverage.find ( "/" );
    return stod(coverage.substr(0, pos)) / stod( coverage.substr(pos+1) ) ;
  }

long LGF::TargetLength(){
    if ( end1 > zstart1 )
      return end1 - zstart1 ;
    else return zstart1 - end1 ;
  }
long LGF::QueryLength(){
    if ( end2 > zstart2 )
      return end2 - zstart2 ;
    else return zstart2 - end2 ;
  }

void LGF::Print(){
  cout << ToString() << endl;
}

string LGF::ToString(){
    return to_string(score) + '\t'
      + name1+ '\t'
      + strand1+ '\t'
      + to_string(size1)+ '\t'
      + to_string(zstart1)+ '\t'
      + to_string(end1)+ '\t'
      + name2+ '\t'
      + strand2+ '\t'
      + to_string(size2)+ '\t'
      + to_string(zstart2)+ '\t'
      + to_string(end2)+ '\t'
      + identity+ '\t'
      + idPct+ '\t'
      + coverage+ '\t'
      + covPct ;
  }
  
bool LGF::operator == (const LGF & a) {
    if( this->score == a.score &&
	this->name1 == a.name1 &&
	this->strand1 == a.strand1 &&
	this->size1 == a.size1 &&
	this->zstart1 == a.zstart1 &&
	this->end1 == a.end1 &&
	this->name2 == a.name2 &&
	this->strand2 == a.strand2 &&
	this->size2 == a.size2 &&
	this->zstart2 == a.zstart2 &&
	this->end2 == a.end2 &&
	this->identity == a.identity &&
	this->idPct == a.idPct &&
	this->coverage == a.coverage &&
	this->covPct == a.covPct ){
      return 1 ;
    }
    return 0;
  }

  
  // long _score,
  // 	  string _name1,
  // 	  string _strand1,
  // 	  long _size1,
  // 	  long _zstart1,
  // 	  long _end1,
  // 	  string _name2,
  // 	  string _strand2,
  // 	  long _size2,
  // 	  long _zstart2,
  // 	  long _end2,
  // 	  string _identity,
  // 	  string _idPct,
  // 	  string _coverage,
  // 	  string _covPct){

LGF LGF::FlipTargetQuery(){
  LGF tmp;
  tmp.name1 = this->name2;
  tmp.name2 = this->name1;
  tmp.strand1 = this->strand2;
  tmp.strand2 = this->strand1;
  tmp.size1 = this->size2;
  tmp.size2 = this->size1;
  tmp.zstart1 = this->zstart2;
  tmp.zstart2 = this->zstart1;
  tmp.end1 = this->end2;
  tmp.end2 = this->end1;
  tmp.identity = this->identity;
  tmp.idPct = this->idPct;
  tmp.coverage = this->coverage;
  tmp.covPct = this->covPct;
  return tmp;
}



bool LGF::empty(){
  if ( name2.size() == 0 )
    return 1;
  else
    return 0;
}
