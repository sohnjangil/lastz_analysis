void NoiseFiltering()
{
  //
  // noise will be written in a file with suffix ".noise"
  //

  vector < LGF > subSeq;
  
  string outfile = prefix + ".noise";
  ofstream noise(outfile.c_str());


  LGF tmp_LGF;
  // LGF tmp_LGF_1;
  // LGF tmp_LGF_2;


  /////////////////////////////////////////////////////////////////////////////////////////
  //
  // filtering noise by identity (default 0.985) and length (default 4000) thresholds
  //
  //     subSeq [ i ] = LGF
  //
  
  noise << LGF_first_line << endl ;
  for ( unsigned int i = 0 ; i < sub_seq.size() ; i ++ ) {
    if ( sub_seq[i].Identity() >= idth && sub_seq[i].QueryLength() >= lth ){
      tmp_LGF = sub_seq[i];
      subSeq.push_back(tmp_LGF);

      query_size[tmp_LGF.QueryName()] = tmp_LGF.QuerySize();
      target_size[tmp_LGF.TargetName()] = tmp_LGF.TargetSize();

      QT_weight[sub_seq[i].QueryName()][sub_seq[i].TargetName()] ++ ;
    }else{
      noise << sub_seq[i].ToString() << endl ;
    }
  }

  // cleare sub_seq of raw data for saving memory
  //sub_seq.clear();
  noise.close();
  sub_seq = subSeq;
  subSeq.clear();
  
  // writing the filtered sub-sequences to "prefix.filter"
  ofstream fout;
  outfile = prefix + ".filter";
  fout.open(outfile.c_str());
  fout << LGF_first_line << endl ;
  
  for ( vector < LGF > :: iterator k = sub_seq.begin()  ; k != sub_seq.end() ; k ++){
    //if (k->ToString().size() != 0)
      fout << k->ToString() << endl ;
  }
  fout.close();
  
}


