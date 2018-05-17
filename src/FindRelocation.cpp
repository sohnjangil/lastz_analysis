///////////////////////////////////////////////////////////////////////////////////////////
//
// INVERSION
//

string FindRelocation ()
{
  string outfile=prefix+".reloc";
  ofstream reloc(outfile.c_str());

  reloc << FIRST_LINE << endl ;

  map < long , vector < string > > LIST;
    
  //string strand;
  
  long count = 0 ;



  for ( map < string , map < string , vector < LGF > > > :: iterator it = TQmap.begin() ; it != TQmap.end() ; it ++ ){
      
    for ( map < string , vector < LGF > > :: iterator jt = it ->second.begin() ; jt != it->second.end() ; jt ++ ){

      if ( MAJOR_MAPPING[jt->first] == it->first ){

	//cout << "FILTERING" << endl;
	
	vector < LGF > LGF_Q_order = jt->second ;
	vector < LGF > LGF_T_order;
	

	// flip positions, if the strand is "-"
	// These steps are needed to removing the influence of inversions
	// while filtering small blocks
	for ( size_t k = 0 ; k != LGF_Q_order.size() ; k ++ ){
	  if ( LGF_Q_order[k].QueryStrand() == "-" ){
	    //if ( LGF_Q_order[k].QueryStart() > LGF_Q_order[k].QueryEnd() ){
	    swap ( LGF_Q_order[k].zstart2, LGF_Q_order[k].end2 ) ;
	  }
	}

	
	// filtering small blocks
	size_t flag;


	// sort by Target start positions.
	sort ( LGF_Q_order.begin() , LGF_Q_order.end() , CompTStart );
	
	if ( LGF_Q_order.size() > 1 ){
	  flag = 0;
	  for ( size_t i = 1 ; i != LGF_Q_order.size() ; i ++ ){
	    if ( LGF_Q_order[flag].TargetEnd() < LGF_Q_order[i].TargetEnd() ){
	      LGF_T_order.push_back(LGF_Q_order[flag]);
	      flag = i ;
	    }
	  }
	  if ( LGF_T_order.size() > 0 &&
	       LGF_T_order[LGF_T_order.size()-1].TargetEnd() < LGF_Q_order[LGF_Q_order.size()-1].TargetEnd() ){
	    LGF_T_order.push_back( LGF_Q_order[LGF_Q_order.size()-1]);
	  }
	  if ( LGF_T_order.size() > 1 )
	    LGF_Q_order = LGF_T_order;
	}
	LGF_T_order.clear();


	// sort by Query start positions.
	sort ( LGF_Q_order.begin() , LGF_Q_order.end() , CompQStart );
	
	if ( LGF_Q_order.size() > 1 ){
	  flag = 0;
	  for ( size_t i = 1 ; i != LGF_Q_order.size() ; i ++ ){
	    if ( LGF_Q_order[flag].QueryEnd() < LGF_Q_order[i].QueryEnd() ){
	      LGF_T_order.push_back(LGF_Q_order[flag]);
	      flag = i ;
	    }
	  }
	  if ( LGF_T_order.size() > 0 &&
	       LGF_T_order[LGF_T_order.size()-1].QueryEnd() < LGF_Q_order[LGF_Q_order.size()-1].QueryEnd() ){
	    LGF_T_order.push_back( LGF_Q_order[LGF_Q_order.size()-1]);
	  }
	  if ( LGF_T_order.size() > 1 )
	    LGF_Q_order = LGF_T_order;
	}
	LGF_T_order.clear();


	//sort ( LGF_Q_order.begin() , LGF_Q_order.end() , CompTStart );

	

	// Restoring positions
	for ( size_t k = 0 ; k != LGF_Q_order.size() ; k++ ){
	  if ( LGF_Q_order[k].QueryStrand() == "-" ){
	    swap ( LGF_Q_order[k].zstart2, LGF_Q_order[k].end2 ) ;
	  }
	}

	sort ( LGF_Q_order.begin() , LGF_Q_order.end() , CompTStart );

	LGF_T_order = LGF_Q_order ;
	
	sort ( LGF_Q_order.begin() , LGF_Q_order.end() , CompQStart );

	
	//////////////////////////////////////////////
	//
	// Searching inversion from here
	//


	//cout << "ORDERING" << endl;
	
	// key_to_Qorder [key] = order_in_query_start ;
	// key_to_Torder [key] = order_in_targt_start ;
	//
	// Here, the key is Qstart position.
	//
	map < long , long > key_to_Qorder;
	map < long , long > key_to_Torder;
	
	for ( size_t k = 0 ; k != LGF_Q_order.size() ; k++ ){
	  key_to_Qorder [LGF_Q_order[k].QueryStart()] = k ;
	}

	for ( size_t k = 0 ; k != LGF_T_order.size() ; k ++ ) {
	  key_to_Torder [LGF_T_order[k].QueryStart()] = k ;
	}
	
	
	string STRAND = STRAND_MAP[jt->first][it->first];
	long strand = 1;
	if ( STRAND == "-" ) strand = -1 ;


	// FINDING RELOCATION
	{
	  long key[2];

	  long Qorder[2];
	  long Torder[2];
	  for ( size_t k = 1 ; k < LGF_Q_order.size() ; k ++ ){

	    key[0] = LGF_Q_order[k-1].QueryStart() ;
	    key[1] = LGF_Q_order[k  ].QueryStart() ;

	    Qorder[0] = key_to_Qorder[key[0]];
	    Qorder[1] = key_to_Qorder[key[1]];
	    
	    Torder[0] = key_to_Torder[key[0]];
	    Torder[1] = key_to_Torder[key[1]];

	    if ( Torder[0] + strand != Torder[1] && Qorder[0] + 1 == Qorder[1] ) {
	      LGF a = LGF_Q_order [k-1];
	      LGF b = LGF_Q_order [k  ];

	      reloc
		<< ++ count << '\t'
		<< "RELOC" << '\t'
		<< a.TargetName() << '\t'
		<< to_string(a.TargetEnd() ) << '\t'
		<< a.TargetName() << '\t' 
		<< to_string(b.TargetStart()) << '\t'
		<< a.QueryName() << '\t'
		<< to_string(a.QueryEnd()) << '\t'
		<< a.QueryName() << '\t'
		<< to_string(b.QueryStart()) << '\n';
	    }
	  }
	}
      }
    }
  }

  reloc.close();

  return to_string(count) + "\t-" ;

}


