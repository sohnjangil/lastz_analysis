long Size( map < string , long > & size_map ){
  long out=0;
  for ( map < string , long > :: iterator i = size_map.begin() ; i != size_map.end() ; i ++ ) {
    out += i->second ;
  }
  return out ;
}


long Target_Size()
{
  return Size ( target_size ) ;
}


long Query_Size()
{
  return Size ( query_size ) ;
}
