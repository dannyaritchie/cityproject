#ifndef PARSERH
#define PARSERH

#include <iostream>
#include<string>
#include <vector>
#include <array>
#include <fstream>
class JsonParser
{
private:
	std::string dataDestination;

public:
	JsonParser(std::string dataDestination); 
	void writeCR(std::ofstream& of, std::vector<std::string> columnTitles, std::vector<std::string> rowTitles);
	void write(std::string filename, std::vector<std::array<double, 3>> dataA);
};
#endif
