# lastz_analysis
This is a tool for analyzing lastz alignment result generated using "--format=general" option to find structural variations or to perform super-scaffolding guided by reference genome.

We recommand lastz_32 for large genomes (1>Gbp genome size).


# Requirement
g++ compiler with c++11 (or higher)

lastz (https://github.com/lastz/lastz.git)


# Installation
Just do

         make

After compiling, set PATH

         export PATH=/path-to-here/bin:$PATH

or add it into ~/.bashrc or ~/.bash_profile


# Usage

Examples)

1. Lastz alignment

         lastz reference.fa assembly.fa[multiple] --format=general --ambiguous=iupac --step=10000 > output.lastz

If needed, modify --step option, but do not change the option "--format=general".
Note that we do not recommand options "--gapped" or "--chain".

2. Using lastz_analysis

         lastz_analysis -i output.lastz [options]

3. Plot

         lastz2gnuplot output.lastz [ reference_scaffold_name [ assembly_scaffold_name ] ] > output

If you want noise-filtered one,

         lastz2gnuplot output.lastz.filterd chr1 > chr1.plot

If you want generate a plot file of contig_1 to chr1, do
         
         lastz2gnuplot output.lastz.filterd chr1 contig_1 > chr1.contig_1.plot
         
The output file(s) can be used in gnuplot or R or any other plot tools.

# Result files

Nise filtered result

         (prefix).filter

Default cut-offs are -d 0.985 -l 4000 for noise filtration.


Filtered-out result

         (prefix).noise

A summary file with basic statistics of structural variations and options used

         (prefix).summary


Structural variation files:

         (prefix).delet   (Deletion)

         (prefix).dupli   (Duplication)

         (prefix).reduc   (Duplication in reference compared with assembly)

         (prefix).insrt   (Insertion)

         (prefix).invrs   (Inversion)

         (prefix).trans   (Inter-chromosomal translocation)

         (prefix).reloc   (Intra-chromosomal translocation)

Mapping informations

         (prefix).map2target  (Statistics of mapping status)

         (prefix).strand      (Strand of assembly scaffold compared with refence)

Strands are calculated in mainly mapped reference scaffold (or chromosome)

# Contact to

Jang-il Sohn

BIG (Bioinformatics and Genomics) Lab.,

Hanyang university, Seoul, Republic of Korea.

sohnjangil@gmail.com
