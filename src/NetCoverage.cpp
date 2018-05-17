
/////////////////////////////////////////////////////
//
// Net-Coverage
//



// long NetCoverage( map < string , map < long , vector < LGF > > > & TQ ){
long NetCoverage()
{
  long Delet=0, Size=0;
  map < string , map < long , vector < LGF > > > :: iterator i = TQ.begin() ;
  map < long , vector < LGF > > :: iterator j = i->second.begin() ;
  vector < LGF > A;
  vector < LGF > B;

  LGF a;
  LGF b;
  
  for ( i = TQ.begin() ; i != TQ.end() ; i ++ ) {
    j = i->second.begin() ;

    A = j->second ;
    a=A[0];
    for ( unsigned int n=1;n!=A.size();n++)
      if(A[n].TargetEnd()>a.TargetEnd()) a=A[n];
    
    Size += a.TargetSize() ;
    Delet += a.TargetStart();
    for ( ; j != i->second.end() ; j ++ ) {
      B = j->second ;
      b=B[0];
      for ( unsigned int n=1;n!=B.size();n++)
	if(B[n].TargetEnd()>b.TargetEnd()) b=B[n];
      if ( b.TargetStart() - a.TargetEnd() > 0 ) Delet += b.TargetStart() - a.TargetEnd() ;
      A = j->second ;
      a=A[0];
      for ( unsigned int n=1;n!=A.size();n++)
	if(A[n].TargetEnd()>a.TargetEnd()) a=A[n];
    }
    Delet += b.TargetSize() - a.TargetEnd();
  }

  return Size - Delet ;
}



