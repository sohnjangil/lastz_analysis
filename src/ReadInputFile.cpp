void ReadInputFile ()
{
  ifstream fin(infile.c_str());

  // get the first line
  // #score name1 strand1 size1 zstart1 end1 name2 strand2 size2 zstart2 end2 identity idPct coverage covPct

  //
  // array of string to get data
  //
  string field[15];

  getline ( fin , LGF_first_line );
  
  while(fin >> field[0]){
    if ( field[0][0] == '#'){
      getline ( fin , field[0] );
    }
    else{
      for(int i = 1 ; i < 15 ; i ++ ) {
	fin >> field[i] ;
      }
      sub_seq.push_back(field);
      size_map[field[1]] = strtol(field[3].c_str(),NULL,0);
    }
  }
  
  // close infile
  fin.close();

  
}


