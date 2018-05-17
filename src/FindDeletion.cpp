//////////////////////////////////////////////////////////////////
//
// DELETIONS
//                            ==
//     ==                           =====           
//     =====                    ==========      =====
//     =========        ===========             =========   Query
//     |       |        |   ==============      |       |
//  <->|       |<------>|                |<---->|       |<------>
//     |       |deletion|                |      |       |
//     |       |        |                |      |       |
//  =============================================================  Target
//
// A is j->first, B is j->second
//





string FindDeletion( )
{
  size_t count = 0 ;
  size_t LENG = 0 ;
  string tmp;

  map < long , vector < string > > LIST;  // list of deletions

  vector < LGF > :: iterator LGF_array_it;
  vector < LGF > :: iterator LGF_array_jt;




  for ( map < string , map < string , vector < LGF > > > :: iterator it = TQmap.begin() ; it != TQmap.end() ; it ++ ){
    for ( map < string , vector < LGF > > :: iterator jt = it->second.begin() ; jt != it->second.end() ; jt ++ ){

      vector < LGF > LGF_array = jt->second ;
      
      // flip positions, if the strand is "-"
      // These steps are needed to removing the influence of inversions.
      for ( size_t k = 0 ; k != LGF_array.size() ; k ++ ){
	if ( LGF_array[k].QueryStart() > LGF_array[k].QueryEnd() ){
	  swap ( LGF_array[k].zstart2, LGF_array[k].end2 ) ;
	}
	// if ( LGF_array[k].TargetStart() > LGF_array[k].TargetEnd() ){
	//   cout << "???" << endl;
	//   swap ( LGF_array[k].zstart1 , LGF_array[k].end1 ) ;
	// }
      }

      // sort by Query start positions.
      sort ( LGF_array.begin() , LGF_array.end() , CompTStart );

      
      // filtering small blocks
      vector < LGF> tmp_array;
      size_t flag;
      if ( LGF_array.size() > 1 ){
	flag = 0;
	for ( size_t i = 1 ; i != LGF_array.size() ; i ++ ){
	  if ( LGF_array[flag].TargetEnd() < LGF_array[i].TargetEnd() ){
	    tmp_array.push_back(LGF_array[flag]);
	    flag = i ;
	  }
	}
	if ( tmp_array.size() > 0 &&
	     tmp_array[tmp_array.size()-1].TargetEnd() < LGF_array[LGF_array.size()-1].TargetEnd() ){
	  tmp_array.push_back( LGF_array[LGF_array.size()-1]);
	}
	if ( tmp_array.size() > 1 )
	  LGF_array = tmp_array;
      }
      tmp_array.clear();

  
      size_t I = 1 ;
      size_t J = 0 ;
      size_t N = LGF_array.size() ;
      long size;
      long size_1;
      
      while ( I != N ) {
	size = LGF_array[I].TargetStart() - LGF_array[J].TargetEnd() ;
	size_1 = LGF_array[I].QueryStart() - LGF_array[J].QueryEnd() ;
	if ( size_1 < 0 ) size_1 *= -1;
	size_1 ++ ;
	if( size_1 < masking_size &&
	    size >= window_size ){ // length between nearest target blocks
	  if ( ( (double) size / (double) size_1 ) > filtering_fraction ) {
	    tmp
	      = LGF_array[J].TargetName() + "\t" 
	      + to_string ( LGF_array[J].TargetEnd()   ) + "\t"
	      + LGF_array[I].TargetName() + "\t" 
	      + to_string ( LGF_array[I].TargetStart() ) + "\t"
	      + LGF_array[J].QueryName() + "\t" 
	      + to_string ( LGF_array[J].QueryEnd()  ) + "\t"
	      + LGF_array[I].QueryName() + "\t" 
	      + to_string ( LGF_array[I].QueryStart()  ) ;
	    LIST [ size ] . push_back ( tmp ) ;
	    LENG += size ;
	  }
	}
	I ++ ;
	J ++ ;
      }
    }
      

    // size = LGF_array[J].TargetSize() - LGF_array[J].TargetEnd() ;

    // if ( size >= window_size && LGF_array.size() > 1 ){
    // 	tmp
    // 	  = LGF_array[J].TargetName() + "\t" 
    // 	  + to_string ( LGF_array[J].TargetEnd()   ) + "\t"
    // 	  + LGF_array[J].TargetName() + "\t"
    // 	  + to_string ( LGF_array[J].TargetSize() ) + "\t" 
    // 	  + LGF_array[J].QueryName() + "\t" 
    // 	  + to_string ( LGF_array[J].QueryStart()  ) + "\t"
    // 	  + "*" + "\t" 
    // 	  + "*" ;
    // 	LIST [ size ] . push_back ( tmp ) ;
    // 	LENG += size ;
    // }

      
  }

  //
  // Write deletions to a file, prefix + ".delet" 
  // and return a string, to_string(count) + "\t" + to_string(LENG)
    
  string outfile = prefix + ".delet" ;
  ofstream Delet(outfile.c_str());

  Delet << FIRST_LINE << endl ;
  
  for ( map < long , vector < string > > :: reverse_iterator list_it = LIST.rbegin(); list_it != LIST.rend() ; list_it ++ ){
    for ( vector < string > :: iterator list_jt = list_it->second.begin() ; list_jt != list_it->second.end() ; list_jt ++ ){
      Delet << ++ count << "\tDELET\t" << * list_jt << endl ;
    }
  }

  Delet.close();

  return to_string(count) + "\t" + to_string(LENG) ;
}
  

