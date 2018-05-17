int InputParameters( int argc , char ** argv )
{
  //default parameters
  int opt;
  while ( (opt = getopt ( argc, argv, "i:p:d:l:w:m:f:s:c:g:G:h:" ) ) != -1 ){;
    switch ( opt ) {
    case 'i':
      infile=optarg;
      break;
    case 'p':
      prefix=optarg;
      break ;
    case 'd':
      if ( optarg ) idth = stod ( optarg ) ;
      break;
    case 'l':
      if ( optarg ) lth = (long) atoi ( optarg ) ;
      break;
    case 'w':
      if ( optarg ) window_size = (long) atoi ( optarg ) ;
      break;
    case 'm':
      if ( optarg ) masking_size = (long) atoi ( optarg ) ;
      break;
    case 'f':
      if ( optarg ) filtering_fraction = stod ( optarg ) ;
      break;
    case 's':
      if ( optarg ) {
	scaffold_file=optarg;
      }
      break;
    case 'c':
      if ( optarg ) {
	chr_list=optarg;
      }
      break;
  
    case 'g':
      if ( optarg ) {
	min_gap= stod ( optarg ) ;
      }
      break;
    case 'G':
      if ( optarg ) {
	MAX_gap= stod ( optarg ) ;
      }
      break;
    default:
      cout << "Invalid option" << optarg << endl;
      return 0 ;
    }
  }


  
  if ( infile.size() == 0 ){
    cout << "Required option: -i in_file_name" << endl;
    hello();
    return 0 ;
  }

  if ( prefix.size() == 0 ){
    prefix = infile ;
  }

  if ( scaffold_file.size() != 0 ){
    out_scaffold_file = scaffold_file + "SScaffold.fasta" ;
  }

  if ( chr_list.size() == 0 && scaffold_file.size() !=0 ){
    cout << "Required option: -c (chr_list_list)" << endl;
    return 0;
  }

  
  return 1;
}
