all:
	mkdir -p build
	g++ -std=c++14 -I/usr/local/include/JSBSim /usr/local/lib/libJSBSim.a source/JFP.cpp -o build/JFP
