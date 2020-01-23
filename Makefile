iptools: iptool.cpp
	g++ -o iptools iptool.cpp utility.cpp image.cpp

clean:
	rm -r *.exe *.out *.o