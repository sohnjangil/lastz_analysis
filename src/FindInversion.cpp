///////////////////////////////////////////////////////////////////////////////////////////
//
// INVERSION
//

string FindInversion ()
{
  string outfile=prefix+".invrs";
  ofstream inver(outfile.c_str());

  inver << FIRST_LINE << endl ;

  long leng;
  long LENG = 0;

  map < long , vector < string > > LIST;
    
  string strand;
  // long str_int=0;

  string tmp;




  for ( map < string , map < string , vector < LGF > > > :: iterator it = TQmap.begin() ; it != TQmap.end() ; it ++ ){
      
    for ( map < string , vector < LGF > > :: iterator jt = it ->second.begin() ; jt != it->second.end() ; jt ++ ){

      if ( MAJOR_MAPPING[jt->first] == it->first ){

	//cout << "FILTERING" << endl;
	
	vector < LGF > LGF_array = jt->second ;
	
	
	// flip positions, if the strand is "-"
	// These steps are needed to removing the influence of inversions
	// while filtering small blocks
	for ( size_t k = 0 ; k != LGF_array.size() ; k ++ ){
	  if ( LGF_array[k].QueryStart() > LGF_array[k].QueryEnd() ){
	    swap ( LGF_array[k].zstart2, LGF_array[k].end2 ) ;
	  }
	}
	
	// sort by Target start positions.
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
	
	// Restoring positions
	for ( size_t k = 0 ; k != LGF_array.size() ; k++ ){
	  if ( LGF_array[k].QueryStrand() == "-" ){
	    swap ( LGF_array[k].zstart2, LGF_array[k].end2 ) ;
	  }
	}


	//////////////////////////////////////////////
	//
	// Searching inversion from here
	//


	//cout << "ORDERING" << endl;
	
	// LGF_map [Query_start] = order_in_target_start ;
	map < long , int > LGF_map;
	for ( size_t k = 0 ; k != LGF_array.size() ; k++ ){
	  LGF_map [LGF_array[k].QueryStart()] = k ;
	}

	map < int , int > t2q ;
	map < int , int > q2t ;
	int t_order;
	int q_order = 0 ;
	for ( map < long , int > :: iterator kt = LGF_map.begin() ; kt != LGF_map.end() ; kt ++ ){
	  t_order = kt->second;
	  t2q [t_order] = q_order ;
	  q2t [q_order] = t_order ;
	  q_order ++ ;
	}
	
	string STRAND = STRAND_MAP[jt->first][it->first];

	size_t start=0;
	size_t end=0;

	long strand = 1;
	if ( STRAND == "-" ){
	  strand = -1 ;
	}

	map < size_t , size_t > inverted_block;

	//cout << "FINDING" << endl;
	for ( size_t k = 0 ; k < LGF_array.size() ; k++ ){

	  if ( LGF_array[k].QueryStrand() != STRAND ){
	    start = k ;
	    end = k ;
	    // scanning inverted blocks
	    for ( ; k+1 < LGF_array.size() ; k ++ ){
	      end = k ;
	      if ( LGF_array[k+1].QueryStrand() == STRAND ||
		   t2q[k] - t2q[k+1] != strand ){
		break;
	      }
	    }

	    // if ( start > end ) {
	    //   cout << "WARNING!" << endl;
	    // }
	    inverted_block[start]=end;
	  }
	}


	//cout << "STORING" << endl;
	for ( map < size_t , size_t > :: iterator kt = inverted_block.begin() ; kt != inverted_block.end() ; kt ++ ) {
	  start = kt->first;
	  end = kt->second;
	  
	  LGF a = LGF_array[start];
	  LGF b = LGF_array[end];
	  
	  double dx = (double) ( b.TargetEnd() - a.TargetStart() ) ;
	  double dy = (double) ( b.QueryEnd() - a.QueryStart() ) ;
	  if ( strand == 1 ) dy *= -1 ;

	  if ( dx / dy < 2 && dy / dx < 2 ){
	    leng = b.QueryEnd() - a.QueryStart() ;
	    if ( leng < 0 ) leng *= -1;
	    tmp
	      = a.TargetName() + '\t'
	      + to_string(a.TargetStart() ) + '\t'
	      + a.TargetName() + '\t' 
	      + to_string(b.TargetEnd()) + '\t'
	      + a.QueryName() + '\t'
	      + to_string(a.QueryStart()) + '\t'
	      + a.QueryName() + '\t'
	      + to_string(b.QueryEnd());
	    LIST[leng].push_back(tmp);
	    LENG += leng;
	  }
	}
      }
    }
  }

  
  int count = 0 ;
  for ( map < long , vector < string > > :: reverse_iterator I=LIST.rbegin() ; I != LIST.rend () ; I ++ ){
    for ( unsigned int k = 0 ; k != I->second.size() ; k ++ ){
      inver << ++ count ;
      inver << '\t';
      inver << "INVRS\t" ;
      inver << I->second[k] << endl;
    }
  }
  inver.close();

  return to_string(count) + "\t" + to_string(LENG) ;

}


