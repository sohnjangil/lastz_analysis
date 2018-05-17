/////////////////////////////////////////////////////
//
//  INSERTION
//
  
/////////////////////////////////////////////////////////////////////////////////////////////
//
// finding Insertion
//
//
//    ===========================================    Query
//                   |         |
//                   |         |
//                   |         |
//    ================         ==================    Target
//
//
// scanning all target
//
//   i->first : target name
//   i->second : j
//     j->first : start position on the target
//     j->second : LGF
//





string FindInsertion()
{
  size_t count = 0 ;
  size_t LENG = 0 ;
  string tmp;

  map < long , vector < string > > LIST;  // list of deletions

  vector < LGF > :: iterator LGF_array_it;
  vector < LGF > :: iterator LGF_array_jt;
    


  for ( map < string , map < string , vector < LGF > > > :: iterator it = QTmap.begin() ; it != QTmap.end() ; it ++ ){
    for ( map < string , vector < LGF > > :: iterator jt = it->second.begin() ; jt != it->second.end() ; jt ++ ){

      vector < LGF > LGF_array = jt->second ;
      // flip positions, if the strand is "-"
      // These steps are needed to filtering inversions.
      for ( size_t k = 0 ; k != LGF_array.size() ; k ++ ){
	if ( LGF_array[k].QueryStart() > LGF_array[k].QueryEnd() ){
	  swap ( LGF_array[k].zstart2, LGF_array[k].end2 ) ;
	}
	if ( LGF_array[k].TargetStart() > LGF_array[k].TargetEnd() ){
	  swap ( LGF_array[k].zstart1 , LGF_array[k].end1 ) ;
	}
      }
      
      // sort by Query start positions.
      sort ( LGF_array.begin() , LGF_array.end() , CompQStart );

      
      // filtering small blocks
      vector < LGF> tmp_array;
      size_t flag;
      if ( LGF_array.size() > 1 ){
	flag = 0;
	for ( size_t i = 1 ; i != LGF_array.size() ; i ++ ){
	  if ( LGF_array[flag].QueryEnd() < LGF_array[i].QueryEnd() ){
	    tmp_array.push_back(LGF_array[flag]);
	    flag = i ;
	  }
	}
	if ( tmp_array.size() > 0 &&
	     tmp_array[tmp_array.size()-1].QueryEnd() < LGF_array[LGF_array.size()-1].QueryEnd() ){
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
	size = LGF_array[I].QueryStart() - LGF_array[J].QueryEnd() ;
	size_1 = LGF_array[I].TargetStart() - LGF_array[J].TargetEnd() ;
	if ( size_1 < 0 ) size_1 *= -1;
	size_1 ++ ;
	if( size_1 < masking_size &&
	    size >= window_size ){ // length between nearest target blocks
	  if ( ( (double) size / (double) size_1 ) > filtering_fraction ) {
	    tmp
	      = LGF_array[J].TargetName() + "\t" 
	      + to_string ( LGF_array[J].TargetEnd()  ) + "\t"
	      + LGF_array[I].TargetName() + "\t" 
	      + to_string ( LGF_array[I].TargetStart()  ) + "\t"
	      + LGF_array[J].QueryName() + "\t" 
	      + to_string ( LGF_array[J].QueryEnd()   ) + "\t"
	      + LGF_array[I].QueryName() + "\t" 
	      + to_string ( LGF_array[I].QueryStart() ) ;
	    LIST [ size ] . push_back ( tmp ) ;
	    LENG += size ;
	  }
	}
	I ++ ;
	J ++ ;
      }
    }
    //cout << LENG << endl;
    
    // size = LGF_array[J].QuerySize() - LGF_array[J].QueryEnd() ;

    // if ( size >= window_size && LGF_array.size() > 1 ){
    // 	tmp
    // 	  = LGF_array[J].TargetName() + "\t" 
    // 	  + to_string ( LGF_array[J].TargetStart()  ) + "\t"
    // 	  + "*" + "\t" 
    // 	  + "*" + "\t"
    // 	  + LGF_array[J].QueryName() + "\t" 
    // 	  + to_string ( LGF_array[J].QueryEnd()   ) + "\t"
    // 	  + LGF_array[J].QueryName() + "\t"
    // 	  + to_string ( LGF_array[J].QuerySize() ) ;
    // 	LIST [ size ] . push_back ( tmp ) ;
    // 	LENG += size ;
    // }

      
  }
  
  string outfile = prefix + ".insrt" ;
  ofstream Insrt(outfile.c_str());

  Insrt << FIRST_LINE << endl ;
  
  for ( map < long , vector < string > > :: reverse_iterator list_it = LIST.rbegin(); list_it != LIST.rend() ; list_it ++ ){
    for ( vector < string > :: iterator list_jt = list_it->second.begin() ; list_jt != list_it->second.end() ; list_jt ++ ){
      Insrt << ++ count << "\tINSRT\t" << * list_jt << endl ;
    }
  }

  Insrt.close();

  return to_string(count) + "\t" + to_string(LENG) ;
}
  



