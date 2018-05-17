all:
	cd src ; make -k
	if [ ! -d 'bin' ] ; then mkdir bin ; fi
	cp src/lastz_analysis src/lastz2gnuplot bin

clean:
	rm -rf bin
