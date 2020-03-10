#include "jsonparser.h"

JsonParser::JsonParser(std::string pdataDestination): dataDestination{pdataDestination} {}; 
void JsonParser::writeCR(std::ofstream& of, std::vector<std::string> columnTitles, std::vector<std::string> rowTitles){
	of << "{\"columns\":[";
	std::string seperator{", "};
	std::string sep{""};
	for(const auto& i : columnTitles) {
		of << sep << "\"" << i << "\"";
		sep = seperator;
	}
	of << "]," << std::endl; 
	of << "\"index\":[";
	sep = "";
	for(const auto& i : rowTitles) {
		of << sep << "\"" << i << "\"";
		sep = seperator;
	}
	of << "]," << std::endl;
	of << "\"data\":";
	return;
}
void JsonParser::write(std::string filename, std::vector<std::array<double, 3>> dataA){
//dataA[0][0] = Pressure, [0][1] = Group, [0][2] is phaseType, 0 is noposchange, 1 is poschange and 2 is framejump;
	std::ofstream of;
	of.open(dataDestination + filename);
	std::vector<std::string> columnTitles = {"Pressure","Group","Phase Type"};
	std::vector<std::string> rowTitles;
	for(int i = 0; i<dataA.size();i++){
		rowTitles.push_back(std::to_string(i));
	}
	writeCR(of, columnTitles, rowTitles); 
	of << "[";
	std::string seperator{", "};
	std::string sep{""};
	for(auto i : dataA){
		of << sep << "[" << "\"" <<  i[0] << "\"" << "," << "\"" << i[1] << "\"" << "," << "\"";
		std::string phaseType;
		if(i[2] == 0){
			phaseType = "No Possession Change";
		}
		if(i[2] == 1){
			phaseType = "Possession Change";
		}
		if(i[2] == 2){
			phaseType = "Frame Jump";
		}
		of << phaseType << "\"" << "]";
		sep = seperator;
	}
	of << "]}" << std::endl;
	of.close();
	return;
}

		
