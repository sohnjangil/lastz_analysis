// calibrate positions of "-" strand
void Calibrate(){
  for ( vector < LGF > :: iterator it = sub_seq.begin() ; it != sub_seq.end() ; it ++ ){
    it->calibrate_position();
  }
}
