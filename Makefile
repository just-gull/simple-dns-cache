all:
	g++ -std=c++17 -pthread dns-cache.cpp main.cpp -o dns-cache-test