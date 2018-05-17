// Target to Query
// Query to Target
void BuildFlippedMap()
{
  for ( vector < LGF > :: iterator it = sub_seq.begin() ; it != sub_seq.end() ; it ++ ){
    subSeq_flip.push_back( it->FlipTargetQuery() );
  }
    
  for ( vector < LGF > :: iterator it = subSeq_flip.begin() ; it != subSeq_flip.end() ; it ++ ){
    TQ_flip[it->TargetName()][it->TargetStart()].push_back(*it) ;
  }
}


