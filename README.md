# lastz_analysis
A tool for analyzing lastz alignment result generated using "--format=general" option to find structural variations or to perform super-scaffolding guided by reference genome

We recommand lastz_32 for large genomes (1>Gbp genome size).


# Requirement
g++ compiler with c++11 (or higher)

lastz (https://github.com/lastz/lastz.git)


# Installation
Just do

$ make

After compiling, set PATH

$ export PATH=/path-to-here/bin:$PATH

or add it into ~/.bashrc or ~/.bash_profile


# Usage

1. Lastz alignment

$ lastz reference.fa assembly.fa[multiple] --format=general --ambiguous=iupac --step=10000 > assembly.fasta.lastz

If needed, modify step option.

2. Using lastz_analysis

$ lastz_analysis -i assembly.fasta.lastz [options]

3. Plot

$ lastz2gnuplot result.lastz [ reference_scaffold_name [ assembly_scaffold_name ] ] > output

The output file can be used for gnuplot or R or any other plot tools.

# Result files

filter:  Noise filtered result.

         Default cut-offs are

                 -d 0.985

                 -l 4000

         for noise filtration.

         The cut-offs can be modified in using lastz_analysis.

noise:   Filtered result by noise filtration.


summary: A summary file of lastz_analysis.

         This contains basic statistics of structural variations and options used.

Structural variation files:

delet:   Deletion.

dupli:   Duplication.

reduc:   Duplication in reference compared with assembly.

insrt:   Insertion.

invrs:   Inversion.

trans:   Inter-chromosomal translocation.

reloc:   Intra-chromosomal translocation.

Mapping informations:

map2target:     Statistics of mapping status.

strand:         Strand of assembly scaffold compared with refence.

                Strands are calculated in mainly mapped reference scaffold (or chromosome)

# Contact to

Jang-il Sohn

BIG (Bioinformatics and Genomics) Lab.,

Hanyang university, Seoul, Republic of Korea.

sohnjangil@gmail.com
