#ifndef BUILD_MAP
#define BUILD_MAP

void BuildMap()
{
  // calibrating positions of "-" strand
  for ( vector < LGF > :: iterator it = sub_seq.begin() ; it != sub_seq.end() ; it ++ ){
    it->calibrate_position();
  }

  for ( vector < LGF > :: iterator it = sub_seq.begin() ; it != sub_seq.end() ; it ++ ){
    TQmap [it->TargetName()][it->QueryName()].push_back(*it);
    QTmap [it->QueryName()][it->TargetName()].push_back(*it);
    // if ( it->strand2 == "+" ){
    TQ[it->TargetName()][it->TargetStart()].push_back(*it) ;
    QT[it->QueryName()] [it->QueryStart()] .push_back(*it) ;
    // }
    // else{
    //   TQ[it->TargetName()][it->TargetStart()].push_back(*it) ;
    //   QT[it->QueryName()] [it->QuerySize() - it->QueryStart()] .push_back(*it) ;
    // }
  }
}

#endif 
