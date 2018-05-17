/////////////////////////////////////////////////////
//
//  Duplication
//

//
// scanning all target
//
//   i->first : target name
//   i->second : j
//     j->first : start position on the target
//     j->second : LGF
//
//    -----------------------------------------
//         =======       =======  Query blocks
//         \      \     /      /
//          \      \   /      /
//           \      \ /      /
//            \      \      /
//             \    / \    /
//              \  /   \  /
//               \/     \/
//                =======  Target blocks
//    -------------------------------------
//
  
pair < long , pair < string , string > > ReturnDuplication ( map < string , map < long , vector < LGF > > > :: iterator & i , map < long , vector < LGF > > :: iterator & j){

  j -- ; vector < LGF > A = j->second ; 
  j ++ ; vector < LGF > B = j->second ;
  
  LGF a,b;
  a=A[0]; for ( unsigned int n = 1 ; n != A.size() ; n++ ) if ( A[n].TargetEnd() > a.TargetEnd() ) a = A[n];
  b=B[0]; for ( unsigned int n = 1 ; n != B.size() ; n++ ) if ( B[n].TargetEnd() > b.TargetEnd() ) b = B[n];

  long x_start;
  long x_end;
  long y_start[2];
  long y_end[2];

  long coor[4];
  
  // 1
  x_start = b.TargetStart() ;
  y_start[0] = b.QueryStart() ;

  // 2
  if ( a.QueryStrand() == "+" )
    y_start[1] = a.QueryStart() - ( a.TargetStart() - x_start ) ;
  else
    y_start[1] = a.QueryStart() + ( a.TargetStart() - x_start ) ;

  coor[0]= y_start[0] ;
  coor[1]= y_start[1] ;

  
  if ( a.TargetEnd() < b.TargetEnd() ){
    // 3
    x_end = a.TargetEnd();
    y_end[0] = a.QueryEnd();

    // 4
    if ( b.QueryStrand() == "+" )
      y_end[1] = b.QueryEnd() - ( b.TargetEnd() - x_end ) ;
    else
      y_end[1] = b.QueryEnd() + ( b.TargetEnd() - x_end ) ;

    coor[2] = y_end[0] ;
    coor[3] = y_end[1] ;
  }
  else{
    // 3
    x_end = b.TargetEnd () ;
    y_end[0] = b.QueryEnd() ;

    // 4
    if ( b.QueryStrand() == "+" )
      y_end[1] = a.QueryEnd() - ( a.TargetEnd() - b.TargetEnd() ) ;
    else
      y_end[1] = a.QueryEnd() + ( a.TargetEnd() - b.TargetEnd() ) ;

    coor[2] = y_end[1] ;
    coor[3] = y_end[0] ;
  }


  
  long diff = x_end - x_start ;
  
  pair < string , string > duplication;
  if ( diff > 0 ){
    
    duplication.first
      = i->first + '\t'
      + to_string(x_start) + '\t'
      + i->first + '\t'
      + to_string(x_end) + '\t'
      //+ to_string(diff) + '\t'
      + a.QueryName() + '\t'
      + to_string(coor[0]) + '\t'
      + a.QueryName() + '\t'
      + to_string(coor[3]) ;
  //+ to_string(diff) ;

    duplication.second
      = i->first + '\t'
      + to_string(x_start) + '\t'
      + i->first + '\t'
      + to_string(x_end) + '\t'
      //+ to_string(diff) + '\t'
      + a.QueryName() + '\t'
      + to_string(coor[1]) + '\t'
      + a.QueryName() + '\t'
      + to_string(coor[2]) ;
  //+ to_string(diff) ;

  }

  pair < long , pair < string , string > > output;
  output.first = diff;
  output.second = duplication;
  
  return output ;
}




string FindDuplication()
{
  string outfile=prefix+".dupli";
  ofstream dupli(outfile.c_str());
    
  dupli << FIRST_LINE << endl;

  pair < long , pair < string , string > > pack;
  pair < string , string > duplication;
    
  map < long , vector < string > > LIST1;
  map < long , vector < string > > LIST2;
  long leng;
  long LENG = 0 ;
    
    
  for ( map < string , map < long , vector < LGF > > > :: iterator i = TQ.begin() ; i != TQ.end() ; i ++ ){
    
    // scanning by start position on the target
    map < long , vector < LGF > > :: iterator j = i->second.begin();
    j++;
    for ( ; j != i->second.end() ; j ++ ){
      pack=ReturnDuplication(i,j);
      leng = pack.first ;
      duplication = pack.second ;
      if(leng>=window_size){
	LIST1[leng].push_back(duplication.first) ;
	LIST2[leng].push_back(duplication.second) ;
	LENG += leng;
      }

      if ( j->second.size() > 1 ){
	pack=ReturnDuplication(i,j);
	leng = pack.first ;
	duplication = pack.second ;
	if(leng>window_size){
	  LIST1[leng].push_back(duplication.first) ;
	  LIST2[leng].push_back(duplication.second) ;
	  LENG += leng;
	}
      }
    }
  }


  size_t count = 0;

  for ( map < long , vector < string > > :: reverse_iterator I =  LIST1.rbegin() ; I != LIST1.rend () ; I ++ ){
    for (unsigned int k = 0 ; k != I->second.size() ; k ++ ){
      dupli << ++ count;
      dupli << '\t';
      dupli << "DUPLI\t";
      dupli << I->second[k] << endl;
      dupli << count;
      dupli << '\t';
      dupli << "DUPLI\t";
      dupli << LIST2[I->first][k] << endl;
    }
  }

  return to_string(count) + "\t" + to_string(LENG) ;

}










/////////////////////////////////////////////////////
//  Reduction:
//  Duplication in reference
//

//
// scanning all target
//
//   i->first : target name
//   i->second : j
//     j->first : start position on the target
//     j->second : LGF
//
//    -----------------------------------------
//         =======       =======  Target blocks
//         \      \     /      /
//          \      \   /      /
//           \      \ /      /
//            \      \      /
//             \    / \    /
//              \  /   \  /
//               \/     \/
//                =======  Query blocks
//    -------------------------------------
//
  
pair < long , pair < string , string > > ReturnReduction ( map < string , map < long , vector < LGF > > > :: iterator & i , map < long , vector < LGF > > :: iterator & j){

  j -- ; vector < LGF > A = j->second ; 
  j ++ ; vector < LGF > B = j->second ;
  
  LGF a,b;
  a=A[0]; for ( unsigned int n = 1 ; n != A.size() ; n++ ) if ( A[n].TargetEnd() > a.TargetEnd() ) a = A[n];
  b=B[0]; for ( unsigned int n = 1 ; n != B.size() ; n++ ) if ( B[n].TargetEnd() > b.TargetEnd() ) b = B[n];

  long x_start;
  long x_end;
  long y_start[2];
  long y_end[2];

  long coor[4];
  
  // 1
  x_start = b.TargetStart() ;
  y_start[0] = b.QueryStart() ;

  // 2
  if ( a.QueryStrand() == "+" )
    y_start[1] = a.QueryStart() - ( a.TargetStart() - x_start ) ;
  else
    y_start[1] = a.QueryStart() + ( a.TargetStart() - x_start ) ;

  coor[0]= y_start[0] ;
  coor[1]= y_start[1] ;

  
  if ( a.TargetEnd() < b.TargetEnd() ){
    // 3
    x_end = a.TargetEnd();
    y_end[0] = a.QueryEnd();

    // 4
    if ( b.QueryStrand() == "+" )
      y_end[1] = b.QueryEnd() - ( b.TargetEnd() - x_end ) ;
    else
      y_end[1] = b.QueryEnd() + ( b.TargetEnd() - x_end ) ;

    coor[2] = y_end[0] ;
    coor[3] = y_end[1] ;
  }
  else{
    // 3
    x_end = b.TargetEnd () ;
    y_end[0] = b.QueryEnd() ;

    // 4
    if ( b.QueryStrand() == "+" )
      y_end[1] = a.QueryEnd() - ( a.TargetEnd() - b.TargetEnd() ) ;
    else
      y_end[1] = a.QueryEnd() + ( a.TargetEnd() - b.TargetEnd() ) ;

    coor[2] = y_end[1] ;
    coor[3] = y_end[0] ;
  }


  
  long diff = x_end - x_start ;
  
  pair < string , string > reduction;
  if ( diff > 0 ){
    
    reduction.first
      = a.QueryName() + '\t'
      + to_string(coor[0]) + '\t'
      + a.QueryName() + '\t'
      + to_string(coor[3]) + '\t' 
      + i->first + '\t'
      + to_string(x_start) + '\t'
      + i->first + '\t'
      + to_string(x_end)  ;
    
    reduction.second
      = a.QueryName() + '\t'
      + to_string(coor[1]) + '\t'
      + a.QueryName() + '\t'
      + to_string(coor[2]) +'\t' 
      + i->first + '\t'
      + to_string(x_start) + '\t'
      + i->first + '\t'
      + to_string(x_end)  ;
   }

  pair < long , pair < string , string > > output;
  output.first = diff;
  output.second = reduction;
  
  return output ;
}




string FindReduction()
{
  string outfile=prefix+".reduc";
  ofstream reduc(outfile.c_str());
    
  reduc << FIRST_LINE << endl;

  pair < long , pair < string , string > > pack;
  pair < string , string > reduction;
    
  map < long , vector < string > > LIST1;
  map < long , vector < string > > LIST2;
  long leng;
  long LENG = 0 ;
    
    
  for ( map < string , map < long , vector < LGF > > > :: iterator i = TQ_flip.begin() ; i != TQ_flip.end(); i ++ ){
    
    // scanning by start position on the target
    map < long , vector < LGF > > :: iterator j = i->second.begin();
    j++;
    for ( ; j != i->second.end() ; j ++ ){
      pack=ReturnReduction(i,j);
      leng = pack.first ;
      reduction = pack.second ;
      if(leng>=window_size){
	LIST1[leng].push_back(reduction.first) ;
	LIST2[leng].push_back(reduction.second) ;
	LENG += leng;
      }

      if ( j->second.size() > 1 ){
	pack=ReturnReduction(i,j);
	leng = pack.first ;
	reduction = pack.second ;
	if(leng>window_size){
	  LIST1[leng].push_back(reduction.first) ;
	  LIST2[leng].push_back(reduction.second) ;
	  LENG += leng;
	}
      }
    }
  }


  size_t count = 0;

  for ( map < long , vector < string > > :: reverse_iterator I =  LIST1.rbegin() ; I != LIST1.rend () ; I ++ ){
    for (unsigned int k = 0 ; k != I->second.size() ; k ++ ){
      reduc << ++ count;
      reduc << '\t';
      reduc << "REDUC\t";
      reduc << I->second[k] << endl;
      reduc << count;
      reduc << '\t';
      reduc << "REDUC\t";
      reduc << LIST2[I->first][k] << endl;
    }
  }

  return to_string(count) + "\t" + to_string(LENG) ;

}










/*

string FindReduction()
{
  string outfile=prefix+".reduc";
  ofstream reduc(outfile.c_str());
    
  reduc << FIRST_LINE << endl;
    
  string tmp1, tmp2;
    
  map < long , vector < string > > LIST1;
  map < long , vector < string > > LIST2;
  long leng;
  long LENG = 0 ;
    
    
  for ( map < string , map < long , vector < LGF > > > :: iterator i = TQ_flip.begin() ; i != TQ_flip.end() ; i ++ ){
    
    // scanning by start position on the target
    map < long , vector < LGF > > :: iterator j = i->second.begin();
    j++;
    for ( ; j != i->second.end() ; j ++ ){
      tmp1=ReturnDuplication1(i,j);
      tmp2=ReturnDuplication2(i,j);
      leng = DuplicationLength(i,j);
      if(leng>=window_size){
	LIST1[leng].push_back(tmp1) ;
	LIST2[leng].push_back(tmp2) ;
	LENG += leng;
      }

      if ( j->second.size() > 1 ){
	tmp1=ReturnDuplication1_RE(i,j);
	tmp2=ReturnDuplication2_RE(i,j);
	leng = DuplicationLength_RE(i,j); 
	if(leng>window_size){
	  LIST1[leng].push_back(tmp1) ;
	  LIST2[leng].push_back(tmp2) ;
	  LENG += leng;
	}
      }
    }
  }


  size_t count = 0;

  for ( map < long , vector < string > > :: reverse_iterator I =  LIST1.rbegin() ; I != LIST1.rend () ; I ++ ){
    for (unsigned int k = 0 ; k != I->second.size() ; k ++ ){
      reduc << ++ count;
      reduc << '\t';
      reduc << "REDUC\t";
      reduc << I->second[k] << endl;
      reduc << count;
      reduc << '\t';
      reduc << "REDUC\t";
      reduc << LIST2[I->first][k] << endl;
    }
  }

  reduc.close();

  ifstream fin ( outfile.c_str() );

  getline ( fin , tmp1 );

  vector < string > sin;
  sin.resize(10);
  vector < string > output;
  //#ID_No. TYPE    Target  start   Target  end     Query   start   Query   end
  //0       1       2       3       4       5       6       7       8       9
  //0       1       6       7       8       9       2       3       4       5
  while ( fin >> sin[0] ) {
    for ( size_t i = 1 ; i != 10 ; i ++ ) fin >> sin[i] ;
    output.push_back(
		     sin[0] + "\t"
		     + sin[1] + "\t"
		     + sin[6] + "\t"
		     + sin[7] + "\t"
		     + sin[8] + "\t"
		     + sin[9] + "\t"
		     + sin[2] + "\t"
		     + sin[3] + "\t"
		     + sin[4] + "\t"
		     + sin[5] );
		       
  }
  fin.close();
  reduc.open(outfile.c_str());
  reduc << FIRST_LINE << endl;
  for ( vector < string > :: iterator it = output.begin() ; it != output.end() ; it ++ ) {
    reduc << *it << endl;
  }
    
  return to_string(count) + "\t" + to_string(LENG) ;

}



*/
