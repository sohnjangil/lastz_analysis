///////////////////////////////////////////////////////////////////////
//
//  Target-Query map
//
  
///////////////////////////////////////////////////////////////////////
//
//  Inter-Chromosomal Translocation
//

/*
 *                             break points in Query
 *
 *                             C    D               c   d
 *                             |    |               |   |      
 * ================================================================== Query
 *                             |    |               |   |
 *    ----------------         |    -----------     |   |
 *               ---------------    |      ----------   ------------  Synteny blocks
 *                             |    |               |   |          
 *                             |    |               |   |          
 *                             |    |               |   |          
 * ===============================  ==================  ============= Targets
 *   Target_1                  |    | Target_2      |   | Taget_3
 *                             |    |               |   |
 *                             A    B               a   b
 *          
 *                             break points in Targets
 *
 * 1   TRANS   Target_2   B   a   (a-B)   Query   D   c   (c-D)
 */
/*
string FindTranslocatedBlocks()
{

  // input inter-chromosomal translocation
  size_t count = 0 ;
  long length = 0 ;
  long LENG = 0 ;

  map < long , vector < string > > LIST;
  string tmp;
    
  // iteration for all queries
  //for ( map < string , map < long , vector < LGF > > > :: iterator it = QT.begin() ; it != QT.end() ; it ++ ){
  for ( map < string , map < long , LGF > > :: iterator it = qt.begin() ; it != qt.end() ; it ++ ){

    // scanning query-start points
    for ( map < long , LGF > :: iterator jt = it->second.begin() ; jt != it->second.end() ; jt ++ ) {
      //for ( map < long , vector < LGF > > :: iterator jt = it->second.begin() ; jt != it->second.end() ; jt ++ ) {

      //for ( vector
      
      // if query is mapping to minor-target,
      // it is called (inter-chr.) translocation.
      if ( jt->second.TargetName() != MAJOR_MAPPING[it->first] ){
	  
	// << format >>
	// #ID_No.    TYPE    Target    start    end    length    Assembly    start    end    length
	
	tmp 
	  = "TRANS\t"
	  + jt->second.TargetName() + "\t"
	  + to_string(jt->second.TargetStart()) + "\t"
	  + to_string(jt->second.TargetEnd()) + "\t";
	
	// trans << to_string( ++ count) << "\t"
	//       << "TRANS" << "\t"
	//       << jt->second.TargetName() << "\t"
	//       << to_string(jt->second.TargetStart()) << "\t"
	//       << to_string(jt->second.TargetEnd()) << "\t";
	    
	length = jt->second.TargetEnd() - jt->second.TargetStart() ;
	if ( length < 0 ) length *= -1 ;
	    
	tmp
	  +=to_string(length) + "\t"
	  + jt->second.QueryName() + "\t"
	  + to_string(jt->second.QueryStart()) + "\t"
	  + to_string(jt->second.QueryEnd()) + "\t";
	
	// trans << to_string(length) << "\t"
	//       << jt->second.QueryName() << "\t"
	//       << to_string(jt->second.QueryStart()) << "\t"
	//       << to_string(jt->second.QueryEnd()) << "\t";
	  
	length = jt->second.QueryEnd() - jt->second.QueryStart() ;
	if ( length < 0 ) length *= -1;
	  
	tmp += to_string(length) ;
	// trans << to_string(length) << "\n";

	if ( length >= window_size ) {
	  LIST[length].push_back(tmp);
	  LENG += length;
	}
	// LENG += length;
      }
    }
  }


  string outfile=prefix+".trans";
  ofstream trans(outfile.c_str());

  trans << FIRST_LINE_BLOCK << endl;

  for ( map < long , vector < string > > :: reverse_iterator it = LIST.rbegin() ; it != LIST.rend() ; it ++ ){
    for ( vector < string > :: iterator jt = it->second.begin() ; jt != it->second.end() ; jt ++ ){
      trans << ++ count << "\t" << *jt << endl ;
    }
  }

  // print translocated blocks
  trans.close();


  return to_string(count) + "\t" + to_string (LENG) ;
}


*/


/*
 *                             break points in Query
 *
 *                             C    D               c   d
 *                             |    |               |   |      
 * ================================================================== Query
 *                             |    |               |   |
 *    ----------------         |    -----------     |   |
 *               ---------------    |      ----------   ------------  Synteny blocks
 *                             |    |               |   |          
 *                             |    |               |   |          
 *                             |    |               |   |          
 * ===============================  ==================  ============= Targets
 *   Target_1                  |    | Target_2      |   | Taget_3
 *                             |    |               |   |
 *                             A    B               a   b
 *          
 *                             break points in Targets
 *
 * 1   TRANS   Target_1   A   Target_2   B   Query   C   Query   D
 * 2   TRANS   Target_2   a   Target_3   b   Query   c   Query   d
 */



string FindTranslocationPoints()
{


  string outfile=prefix+".trans";
  ofstream trans(outfile.c_str());

  trans << FIRST_LINE << endl;

  // input inter-chromosomal translocation
  size_t count = 0 ;
  // long length = 0 ;
  // long LENG = 0 ;



  for ( map < string , map < string , vector < LGF > > > :: iterator it = QTmap.begin() ; it != QTmap.end() ; it ++ ){
    vector < LGF > LGF_array;
    for ( map < string , vector < LGF > > :: iterator jt = it->second.begin() ; jt != it->second.end() ; jt ++ ){
      LGF_array.insert(LGF_array.end() , jt->second.begin() , jt->second.end() );
    }
    sort(LGF_array.begin(),LGF_array.end(),CompQStart);




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
      

    

    for ( size_t i = 1 ; i < LGF_array.size() ; i ++ ){

      if ( LGF_array[i-1].TargetName() != LGF_array[i].TargetName() )
	// && LGF_array[i].QueryStart() - LGF_array[i-1].QueryEnd() > - 100000000 )
	{
	  trans << to_string ( ++ count ) << "\t"
		<< "TRANS" << "\t"
		<< LGF_array[i-1].TargetName() << "\t"
		<< LGF_array[i-1].TargetEnd() << "\t"
		<< LGF_array[i].TargetName() << "\t"
		<< LGF_array[i].TargetStart() << "\t"
		<< LGF_array[i-1].QueryName() << "\t"
		<< LGF_array[i-1].QueryEnd() << "\t"
		<< LGF_array[i].QueryName() << "\t"
		<< LGF_array[i].QueryStart() << "\n" ;
	}
    }
  }
  
  // print translocated blocks
  trans.close();


  return to_string(count) + "\t-" ;
}



