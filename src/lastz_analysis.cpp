#include "LGF_tools.hpp"


using namespace LGF_tools;


/*
template < class T >
void check( T a ) {
  cout << "check\t" << a << endl;
}

void check() {
  cout << "check" << endl;
}
*/






///////////////////////////////////////////////////////////////////
//
//  MAIN function
//
///////////////////////////////////////////////////////////////////

int main(int argc, char ** argv){
  
  if ( argc < 2 ) {
    hello();
    return 0;
  }
 
  // initial parameters
  window_size = 1000 ;
  idth = 0.985 ;
  lth = 4000 ;
  filtering_fraction= 3;
  masking_size = 1000 ;
  min_gap=1000;
  MAX_gap=100000;
  
  int input_success=InputParameters( argc , argv );

  if ( ! input_success ) return 0 ;
  
 
  //
  // dummy variables for filtering unknown target
  //
  //size_t m=0; // for lines having '_'
  //size_t n=0; // for target LGEs

  

  ReadInputFile();
  NoiseFiltering();
  //WriteFilteredResults();

  BuildMap();

  BuildFlippedMap();

  //SmallBlockFiltering();

  ScanningQT();

  // Finding Structural Variations
  string DELET = FindDeletion();     // OK
  string INSRT = FindInsertion();    // OK
  string DUPLI = FindDuplication();  // OK
  string REDUC = FindReduction();    // 
  string INVRS = FindInversion();    // OK
  string RELOC = FindRelocation();     // THIS MUST BE FIXED
  //string TRANS = FindTranslocatedBlocks(); // THIS HAS BEEN REMOVED
  string TRANS = FindTranslocationPoints(); //OK






  
  
  

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  //
  // Print map of query to target
  //
  /* 
  string Reloc;
  int a,b,c,d;
  FindRelocation ( QT , Reloc, a, b, c, d) ;
  string outfile=prefix+".reloc";
  ofstream RelocOut(outfile.c_str());
  RelocOut << Reloc ;
  RelocOut.close();
  RELOC += "#RELOC\t" + to_string(a) + "\t" + to_string(b) + "\t" + to_string(c) + "\t" + to_string(d);
  */

  
  /////////////////////////////////////////////////////////////////////////////////////////////////////
  //
  // Print Summary
  //
  
  /////////////////////////////////////////////////////////////////////////////////////////////
  //
  // summary file
  //   which is being written until the end of the program
  //

  string Outfile = prefix + ".summary";
  ofstream summary(Outfile.c_str());

  summary << endl
	  << "#Input file:\t" << infile << endl <<endl
    
	  << "#parameters for removing mapping noise" << endl
	  << "#identity_threshold:\t" << idth << endl
	  << "#length_threshold:\t" << lth << endl << endl 

	  << "#parameters for removing false positives" << endl
	  << "#minimum_length_of_SV:\t" << window_size << endl
	  << "#masking_size:\t" << masking_size << endl
	  << "#filtering_fraction:\t" << filtering_fraction << endl << endl
    
    	  << "#Structural variation with window_size:\t" << window_size << endl
	  << "\t" << prefix << ".insrt\t(INSeRTion)" << endl
	  << "\t" << prefix << ".delet\t(DELETion)" << endl
	  << "\t" << prefix << ".dupli\t(DUPLIcation)" << endl
	  << "\t" << prefix << ".invrs\t(INVeRSion)" << endl
	  << "\t" << prefix << ".trans\t((inter-chromosomal) TRANSlocation)" << endl
  	  << "\t" << prefix << ".reloc\t((intra-chromosomal) RELOClocation)" << endl
	  << endl
  	  << "#Additional output files:" << endl
	  << "\t" << prefix << ".summary\t(SUMMARY, this file)" << endl
     	  << "\t" << prefix << ".filter\t(FILTERed file without noise in general lastz output format)" << endl 
	  << "\t" << prefix << ".noise\t(NOISE in general lastz output format)" << endl
    
	  << "\t" << prefix << ".map2target\t(map of query to target)" << endl
	  << "\t" << prefix << ".strand\t(query strand)" << endl
	  << endl;




  summary << "#==============================#" << endl ;
  summary << "#                              #" << endl ;
  summary << "#   SUMMARY                    #" << endl ;
  summary << "#                              #" << endl ;
  summary << "#==============================#" << endl
	  << endl;
  summary << "#Target_size\t" << to_string(Target_Size()) << endl ;
  summary << "#Query_size\t" << to_string(Query_Size()) << endl ;


  
  long NetCover=NetCoverage();

  summary << "#Net-Coverage\t" << to_string(NetCover) << endl ;
  summary << "#Net-Cov. (%)\t" << to_string( (double)NetCover / (double)Target_Size() )  << endl << endl ;
  summary << "#Type\tCount\tTotal_Length" << endl ;
  summary << "DELET\t" << DELET << endl ;
  summary << "INSRT\t" << INSRT << endl ;
  summary << "DUPLI\t" << DUPLI << endl ;
  summary << "REDUC\t" << REDUC << endl ; 
  summary << "INVRS\t" << INVRS << endl ;
  summary << "TRANS\t" << TRANS << endl ;
  summary << "RELOC\t" << RELOC << endl ;
  summary << endl ;


  if (scaffold_file.size()!=0){
    make_chromosome();
    summary << "Super-scaffold: " << out_scaffold_file << endl;
  }

  summary.close();

  return 0;
}

