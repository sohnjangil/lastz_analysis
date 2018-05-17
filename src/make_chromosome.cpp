//#include "LGF_tools.hpp"
//#include <set>

using namespace LGF_tools;


map < string , string > find_major_chromosome (){

  string tmp;
  string contig;
  string chromo;
  size_t map_length;

  map < string , map < string , size_t > > contig_chromo_leng;

  map < string , string > out_map;
  
  string infile=prefix+".map2target";

  
  ifstream fin ( infile.c_str() );
  getline ( fin , tmp );
  while ( fin >> tmp >> contig >> chromo >> tmp >> map_length >> tmp >> tmp ){
    contig_chromo_leng[contig][chromo]=map_length;
  }
  fin.close();

  for ( auto & i : contig_chromo_leng ){
    size_t max = 0 ;
    for ( auto & j : i.second ){
      if ( j.second > max ){
	chromo = j.first;
	max = j.second;
      }
    }
    out_map[i.first]=chromo;
  }

  return out_map;
};




map < string , string > find_major_strand (){

  string tmp;
  string contig;
  string chromo;
  size_t map_length;
  string strand;

  map < string , map < string , size_t > > contig_chromo_leng;

  map < string , string > out_map;
  
  string infile=prefix+".map2target";

  
  ifstream fin ( infile.c_str() );
  getline ( fin , tmp );
  while ( fin >> tmp >> contig >> chromo >> tmp >> map_length >> tmp >> strand ){
    contig_chromo_leng[contig][chromo]=map_length;
  }
  fin.close();

  for ( auto & i : contig_chromo_leng ){
    size_t max = 0 ;
    for ( auto & j : i.second ){
      if ( j.second > max ){
	chromo = j.first;
	max = j.second;
      }
    }
    out_map[i.first]=chromo;
  }

  fin .open ( infile.c_str() );
  map < string , string > out_map_strand;
  getline ( fin , tmp );
  while ( fin >> tmp >> contig >> chromo >> tmp >> map_length >> tmp >> strand ){
    if (contig_chromo_leng[contig].find(chromo) != contig_chromo_leng[contig].end())
      out_map_strand[contig]=strand;
  }
  fin.close();


  return out_map_strand;
};



void flip_seq(string & seq){
  reverse(seq.begin(),seq.end());
  for ( string::iterator a = seq.begin() ; a != seq.end() ; a ++ ){
    switch ( *a ){
    case 'A' : *a = 'T' ; break ;
    case 'T' : *a = 'A' ; break ;
    case 'U' : *a = 'A' ; break ;

    case 'C' : *a = 'G' ; break ;
    case 'G' : *a = 'C' ; break ;

    case 'R' : *a = 'Y' ; break ;  //A, G -->  C, T(U)
    case 'Y' : *a = 'R' ; break ;  //C, T(U) ---> G, A

    case 'K' : *a = 'M' ; break ;  //G, T(U) ---> C, A
    case 'M' : *a = 'K' ; break ;  //C, A ---> G, T(U)

    case 'S' : *a = 'W' ; break ;  //G, C ---> A, T(U)
    case 'W' : *a = 'W' ; break ;  //A, T(U) ---> G, C

    case 'B' : *a = 'V' ; break ;  //not A --> not T(U)
    case 'V' : *a = 'B' ; break ;  //not T(U) --> not A

    case 'D' : *a = 'H' ; break ;  //not C --> not G
    case 'H' : *a = 'D' ; break ;  //not G --> not C

    case 'N' : *a = 'N' ; break ;

    default :  *a = 'N' ; break ;
    }
  }
}



size_t removing_internal_scaffold ( map < size_t , string > & scaffold_order_map,
				    map < string , pair < size_t , size_t > > & scaffold_start_end ){
  size_t count = 0 ;
  
  map < size_t , string > :: iterator it = scaffold_order_map.begin();
  map < size_t , string > :: iterator jt = scaffold_order_map.begin();
  jt ++ ;
    
  map < string , pair < size_t , size_t > > scaffold_start_end_tmp;
  scaffold_start_end_tmp[it->second] = scaffold_start_end[it->second];

  for ( ; jt != scaffold_order_map.end() ; jt ++ ){
      
    size_t b,d;
      
    b = scaffold_start_end[it->second].second;
    d = scaffold_start_end[jt->second].second;

    if ( b < d ) {
      scaffold_start_end_tmp[jt->second] = scaffold_start_end[jt->second];
    }
    else{
      count ++ ;
    }
    it = jt ;
  }
  
  scaffold_start_end=scaffold_start_end_tmp;

  scaffold_order_map.clear();
  for ( auto & i : scaffold_start_end ){
    scaffold_order_map[i.second.first] = i.first;
  }

  return count;
};





int make_chromosome (){

  string id ;
  string seq;
  string tmp;
  size_t found;
  map < string , string > Seq; // Seq[chromosome]=sequence
  set < string > Chr;
  string chr;

  map < string , size_t > scaffold_size; //scaffold_size[scaffold]=scaffold_size
  
  /****************************************
   * reading scaffold file
   */

  ifstream fin(scaffold_file.c_str());
  getline(fin , tmp );
  for ( size_t i = 1 ; i != tmp.size() ; i ++ ){
    if ( isspace(tmp[i]) ){
      found = i;
      break;
    }
  }
  id = tmp.substr(1,found-1);
  while(getline(fin,tmp)){
    if (tmp[0]=='>'){
      Seq[id]=seq;

      for ( size_t i = 1 ; i != tmp.size() ; i ++ ){
	if ( isspace(tmp[i]) ){
	  found = i;
	  break;
	}
      }
      id = tmp.substr(1,found-1);
      seq.clear();
    }else{
      seq += tmp;
    }
  }
  Seq[id]=seq;
  id.clear();
  seq.clear();
  fin.close();


  for ( auto & i : Seq ){
    scaffold_size[i.first]=i.second.size();
  }
  

  /****************************************
   * reading chromosome list
   */
  fin.open(chr_list.c_str());
  while ( fin >> chr ){
    Chr.insert(chr);
  }
  fin.close();

  /****************************************
   * searching major chromosome
   */


  map < string , string > scaf_chromosome = find_major_chromosome();
  map < string , string > scaf_strand = find_major_strand();


  /****************************************
   * caculating center position
   */
  for ( auto & i : Seq ){
    id = i.first;
    seq = i.second;
  }


  
  double block_size;
  double center;
  map < string , double > scaffold_cal; 
  map < string , double > scaffold_norm;
  map < string , size_t > scaffold; //scaffold[scaffold]=center_position



  //map < string , map < string , vector < LGF > > > TQmap
  
  for ( auto & i : TQmap ){
    chr = i.first;
    if ( Chr.find(chr) != Chr.end() ){
      for ( auto & j : i.second){
	id = j.first;
	if(scaf_chromosome[id]==chr){
	  for (auto & k : j.second){
	    block_size = (double) (k.TargetEnd() - k.TargetStart());
	    if ( block_size < 0 ) block_size *= -1;
	    center = (double) (k.TargetEnd() + k.TargetStart()) / 2;
	    scaffold_cal[id]+=block_size*center*k.Identity();
	    scaffold_norm[id]+=block_size*k.Identity();
	  }
	}
      }
    }
  }


  map < size_t , string > scaffold_order_map;
  
  size_t pos;  
  for ( auto & i : scaffold_cal){
    
    pos = (size_t)(i.second/scaffold_norm[i.first]);
    scaffold_order_map[pos]=i.first;
    
  }



  map < string , pair < size_t , size_t > > scaffold_start_end;
  
  for ( auto & i : scaffold_order_map ){
    size_t start;
    if ( i.first > ( scaffold_size[i.second] / 2 ) )
      start = i.first - ( scaffold_size[i.second] / 2 );
    else
      start = 0 ; 
    size_t end = start + scaffold_size[i.second] ;
    
    scaffold_start_end[i.second] = pair < size_t , size_t > ( start , end ) ;
  }
  

  /****************************************88
   * removing internal scaffolds
   */


  size_t check = removing_internal_scaffold(scaffold_order_map,scaffold_start_end);
  while ( check != 0 ){
    check = removing_internal_scaffold(scaffold_order_map,scaffold_start_end);
  }

  
  
  /*****************************************
   * repositioning scaffold
   */

  map < size_t , string > :: iterator it = scaffold_order_map.begin();
  map < size_t , string > :: iterator jt = scaffold_order_map.begin();
  jt ++ ;
    
  for ( ; jt != scaffold_order_map.end() ; jt ++ ){

    size_t b,c;
    size_t shift;

    b = scaffold_start_end[it->second].second;
    c = scaffold_start_end[jt->second].first;

    if (b + min_gap > c ) {
      shift = b + min_gap - c ;
      for ( map < size_t , string > :: iterator kt = jt ; kt != scaffold_order_map.end() ; kt ++ ){
	scaffold_start_end[kt->second].first += shift;
	scaffold_start_end[kt->second].second+= shift;
      }
    }
    
    if (b + MAX_gap < c ) {
      shift = c - b - MAX_gap ;
      for ( map < size_t , string > :: iterator kt = jt ; kt != scaffold_order_map.end() ; kt ++ ){
	scaffold_start_end[kt->second].first -= shift;
	scaffold_start_end[kt->second].second-= shift;
      }
    }
    
    it=jt;
    
  }


  map < string , size_t > chr_size;
  map < string , size_t > chr_start_cut;
  for ( auto & i : Chr ){
    chr_start_cut[i] = string :: npos;
  }

  for ( auto & i : scaffold_order_map){
    if (chr_size[scaf_chromosome[i.second]] < scaffold_start_end[i.second].second){
      chr_size[scaf_chromosome[i.second]] = scaffold_start_end[i.second].second ;
    }
    if (chr_start_cut[scaf_chromosome[i.second]] > scaffold_start_end[i.second].first){
      chr_start_cut[scaf_chromosome[i.second]] = scaffold_start_end[i.second].first ;
    }
  }


  for ( auto & i : scaffold_order_map){
    scaffold_start_end[i.second].first -= chr_start_cut[scaf_chromosome[i.second]] ;
    scaffold_start_end[i.second].second -= chr_start_cut[scaf_chromosome[i.second]] ;
  }

  for ( auto & i : chr_size ){
    i.second -= chr_start_cut[i.first] ;
  }


  /*****************************************************88
   * printing super-scaffolding info
   */
  string outfile = prefix + ".super_scaffolding.info";
  ofstream fout(outfile.c_str());

  
  for ( auto & i : scaffold_order_map){
    fout << scaf_chromosome[i.second] << "\t" << i.second << "\t" << scaffold_start_end[i.second].first << "\t" << scaffold_start_end[i.second].second << endl;
  }
  fout.close();


  /*****************************************************88
   * Construct super-scaffold
   */

  map < string , string > SScaffold;
  for ( auto & i : chr_size ){
    SScaffold[i.first].resize(i.second);
    for ( auto & j :  SScaffold[i.first] ){
      j = 'N';
    }
  }


  for ( auto & i : scaffold_start_end ){
    if(scaf_strand[i.first] == "-"){
      flip_seq(Seq[i.first]);
    }
  }

  
  for ( auto & i : scaffold_start_end ){
    chr = scaf_chromosome[i.first] ;
    size_t start = i.second.first;
    size_t end = i.second.second;
    //cout << end - start << "\t" << Seq[i.first].size() << endl;
    SScaffold[chr].replace(start,end-start,Seq[i.first]);
  }

  /*****************************************************
   * Printing SScaffold
   */

  outfile = prefix +".SScaffold.fasta";
  fout.open(outfile.c_str());
  for ( auto & i : SScaffold ){
    fout << ">" << i.first << endl << i.second << endl;
  }


  size_t number = 1 ;
  const string scaf_pref = "unplaced_";
  for ( auto & i : Seq ){
    if (scaffold_start_end.find(i.first) == scaffold_start_end.end()){
      fout << ">" << scaf_pref << number ++ << endl << i.second << endl;
    }
  }

  fout.close();
  
  return 0;
}

