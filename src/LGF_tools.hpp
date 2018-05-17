#ifndef LGF_TOOLS
#define LGF_TOOLS

#include "LGF.hpp"

#include <unistd.h>


namespace LGF_tools{

  // parameters
  long window_size;
  long lth;
  double idth;
  double filtering_fraction;
  long masking_size;
  
  string infile;
  string prefix;

  string scaffold_file;
  string chr_list;
  string out_scaffold_file;
  size_t min_gap;
  size_t MAX_gap;
  // raw container
  vector < LGF > sub_seq; 


  // QT_weight [ query_name ] [ target_name ] is counts mapped reads.
  // if QT_weight [ query_name ] [ target_name ] == 0 , then it is regarded as "NOISE"
  map < string , map < string , int > > QT_weight; // Query to Target

  // It is the main map
  // MAP [target_id] [query_id] = vector < LGF >
  map < string , map < string , vector < LGF > > > MAP;

  
  // // containers, that is sorted by starting points
  map < string , map < long , vector < LGF > > > TQ;
  map < string , map < long , vector < LGF > > > QT;

  // MAIN CONTAINERS
  map < string , map < string , vector < LGF > > > TQmap;
  map < string , map < string , vector < LGF > > > QTmap;
  

  // // small-block-filtered containers
  map < string , map < long , LGF > > tq;
  map < string , map < long , LGF > > qt;






  void hello(){
    cout << endl << "Usage:\n\tlastz_analysis [options] -i in_file_name [options]" << endl
      //-o out_file -d identity_filtering -l length_filtering" << endl
	 << endl
	 << "Options:" << endl
	 << "\t-p <string>\t" << "Prefix of output files" << endl
	 << "\t-d <float> \t" << "iDentity threshold for noise-filtering (0~1) [0.985]" << endl
	 << "\t-l <int>   \t" << "Length threshold for noise-filtering (>0) [4000]" << endl
	 << "\t-w <int>   \t" << "minimum Window size for detecting structural variants (>0) [1000]" << endl
	 << "\t-m <int>   \t" << "Masking size: Blocks shorter than this are masked [1000]" << endl
	 << "\t-f <float> \t" << "Fraction cut-off [3]" << endl
	 << "\t-s <string>\t" << "Scaffold file for super-scaffolding [NONE]" << endl
	 << "\t-c <string>\t" << "Chromosome list for super-scaffolding [NONE]" << endl
	 << "\t-g <int>   \t" << "minimun Gap-size in super-scaffolding [1000]" << endl
	 << "\t-G <int>   \t" << "MAXIMUM Gap-size in super-scaffolding [100000]" << endl
	 << endl
	 << "Note!!!" << endl
	 << "\t" << "input file must be a output file of Lastz alignment generated with option "<< endl
	 << "\t" << "\"--format=general\"." << endl
	 << endl
	 << "Output file:" << endl
	 << "\tprefix.summary   \t(SUMMARY)" << endl
	 << "\tprefix.filter    \t(noise-FILTERed file"<< endl
	 << "\tprefix.noise     \t(mapping NOISE)" << endl
	 << "\tprefix.map2target\t(map of query to target)" << endl
	 << "\tprefix.strand    \t(query STRAND to target)" << endl
	 << endl
	 << "Large structural variations (>=minimum_window_size):" << endl
	 << "\tprefix.insrt     \t(INSeRTion)" << endl
	 << "\tprefix.delet     \t(DELETion)" << endl
	 << "\tprefix.dupli     \t(DUPLIcation)" << endl
	 << "\tprefix.reduc     \t(REDUCtion or duplication in reference)" << endl
	 << "\tprefix.invrs     \t(INVeRSion)" << endl
	 << "\tprefix.trans     \t((inter-chromosomal) TRANSlocation)" << endl
	 << "\tprefix.reloc     \t((intra-chromosomal) RELOClocation)" << endl
	 << endl
	 << "bug-report to " << endl
	 << "\tJang-il Sohn (sohnjangil@gmail.com)" << endl
	 << endl
	 << "Last update at Apr 30, 2018 (ver1.5)"
	 << endl
	 << endl;
  };
  



  // flipped main containers
  // Target --> Query
  // Query --> Target
  vector < LGF > subSeq_flip;
  map < string , map < long , vector < LGF > > > TQ_flip;




  //
  // auxiliary containers
  //
  
  // STRAND_MAP [ query_id ] [ target_id ] = strand(+/-)
  map < string , map < string , string > > STRAND_MAP;

  // SIZE_MAP [ query_id ] = query_size 
  map < string , long > SIZE_MAP;

  // MAJOR_MAPPING [ query_id ] = target_id
  map < string , string > MAJOR_MAPPING;



  
  // maps of size
  map < string , long > size_map;
  map < string , long > query_size ;
  map < string , long > target_size ;

  
  // the first line of output files
  string FIRST_LINE="#ID_No.\tTYPE\tTarget\tstart\tTarget\tend\tQuery\tstart\tQuery\tend"  ;
  string FIRST_LINE_BLOCK="#ID_No.\tTYPE\tTarget\tstart\tend\tsize\tQuery\tstart\tend\tsize"  ;
  
  // string for the first line of LGF
  string LGF_first_line;

  
  bool CompTStart (LGF i,LGF j) { return ( i.TargetStart() < j.TargetStart() ); }
  bool CompQStart (LGF i,LGF j) { return ( i.QueryStart() < j.QueryStart() ); }

  
#include "BuildFlippedMap.cpp"
#include "FindDeletion.cpp"
#include "FindInsertion.cpp"
#include "FindRelocation.cpp"
#include "InputParamters.cpp"
#include "NoiseFiltering.cpp"
#include "ReadInputFile.cpp"
  //#include "SmallBlockFiltering.cpp"
#include "BuildMap.cpp"
#include "FindDuplication.cpp"
#include "FindInversion.cpp"
#include "FindTranslocation.cpp"
#include "NetCoverage.cpp"

#include "ScanningQT.cpp"

#include "Size.cpp"
#include "Calibrate.cpp"

#include "make_chromosome.cpp"
}

#endif
          
