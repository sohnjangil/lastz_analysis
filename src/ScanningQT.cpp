
//
// Scanning QT map
//
// update SIZE_MAP
//     STRAND_MAP
//     MAJOR_MAPPING
//
// and write files
//     strand
//     map2target
//

void ScanningQT()
{

  string outfile;

  // mapping_weight[QueryName][TargetName] = weight
  map < string , map < string , long > > mapping_weight ;
  map < string , map < string , long > > strand_weight;


  long max;
  string major;

  
  for ( vector < LGF > :: iterator it = sub_seq.begin() ; it != sub_seq.end() ; it ++ ){
    if ( SIZE_MAP.find(it->QueryName()) == SIZE_MAP.end() ){
      SIZE_MAP[it->QueryName()] = it->QuerySize();
    }

    
    mapping_weight[it->QueryName()][it->TargetName()] += it->QueryLength() ;

    if (it->QueryStrand() == "+" ){
      strand_weight[it->QueryName()][it->TargetName()] += it->QueryLength() ;
    }
    else {
      strand_weight[it->QueryName()][it->TargetName()] -= it->QueryLength() ;
    }
  }

  

  // find strand direction
  for ( map < string , map < string , long > > :: iterator i = strand_weight.begin() ; i != strand_weight.end() ; i ++ ) {
    for ( map < string , long > :: iterator j = i->second.begin() ; j != i->second.end() ; j ++ ) {
      if ( j->second >= 0  ) STRAND_MAP [i->first][j->first] = "+";
      else STRAND_MAP [i->first][j->first] = "-";
    }
  }


  // find major mapping target
  for ( map < string , map < string , long > > :: iterator it = mapping_weight.begin() ; it != mapping_weight.end() ; it ++ ){
    max = 0 ;
    for ( map < string , long > :: iterator jt = it->second.begin() ; jt != it->second.end() ; jt ++ ){
      if ( max < jt->second ) {
	max = jt->second;
	major = jt->first;
      }
    }
    MAJOR_MAPPING[it->first]=major;
  }


  //
  // Writing strand
  //
    
  outfile=prefix+".strand";
  ofstream StrandOut(outfile.c_str());
  StrandOut << "#Query\tstrand\n" ;
  for ( map < string , map < string , string > > :: iterator it = STRAND_MAP.begin() ; it != STRAND_MAP.end() ; it ++ ){
    StrandOut << it->first + "\t" + STRAND_MAP[it->first][MAJOR_MAPPING[it->first]] + "\n";
  }
  StrandOut.close();
  


  //
  // Writing map2target
  //

  outfile=prefix+".map2target";
  ofstream Map2target(outfile.c_str());

  string kind[2];
  kind[0]="UNIQUE";
  kind[1]="MULTI";
  int k = 0;

  Map2target << "#Type\tQuery\tTarget\tMappingLegnth\tQueryLength\tFraction\tStrandToTarget\n";

  
  for ( map < string , map < string , long > > :: iterator i = mapping_weight.begin() ; i != mapping_weight.end() ; i ++ ){
    if ( i->second.size() > 1 ) k=1;
    else k = 0 ;
    for ( map < string , long > :: iterator j = i->second.begin() ; j != i->second.end() ; j ++ ){
      Map2target
	<< kind[k] << '\t'
	<< i->first << '\t'
	<< j->first << '\t'
	<< to_string(SIZE_MAP[i->first]) << '\t' 
	<< to_string(j->second) << '\t'
	<< to_string ( (double) j->second / (double) SIZE_MAP [i->first] ) << '\t'
	<< STRAND_MAP[i->first][j->first] << '\n';
    }
  }

  Map2target.close();


  outfile=prefix+".mapEntropy";
  ofstream Mapping_entropy(outfile.c_str());
  double norm=5;
  for ( map < string , map < string , long > > :: iterator i = mapping_weight.begin() ; i != mapping_weight.end() ; i ++ ){
    if ( i->second.size() > 1 ) {
      double entropy;
      double fraction;
      double sum;
      long max = 0;
      string TARGET_NAME;
      for ( map < string , long > :: iterator j = i->second.begin() ; j != i->second.end(); j ++ ) {
	sum += (double) j->second;
	if ( max < j->second ){
	  max = j->second ;
	  TARGET_NAME = j->first ;
	}
      }
      for ( map < string , long > :: iterator j = i->second.begin() ; j != i->second.end(); j ++ ) {
	fraction = ( (double) j->second ) / ( (double) sum) ;
	entropy -= fraction * log ( fraction );
      }
      Mapping_entropy
	<< i->first << "\t"
	<< TARGET_NAME << "\t"
	//<< entropy << endl;
	<< exp(-entropy/norm) << endl;
    }
    else{
      map < string , long > :: iterator j = i->second.begin() ;
      Mapping_entropy
	<< i->first << "\t"
	<< j->first << "\t"
	<< 1 << endl;
    }
  }

  

  Mapping_entropy.close();
  
}


