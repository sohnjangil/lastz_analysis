all:
	g++ -Wall -g -std=c++11 LGF.cpp -c -o LGF.o	
	g++ -Wall -g -std=c++11 lastz_analysis.cpp LGF.o -o lastz_analysis
	g++ -Wall -g -std=c++11 lastz2gnuplot.cpp -o lastz2gnuplot


clean:
	rm *.o lastz_analysis lastz2gnuplot
