#ifndef pressureprocessor
#define pressureprocessor
#include <vector>
#include<array>
#include <fstream>
#include <iostream>
#include<string>

	class PressureProcessor {
	private:
		int previousFid;
		std::vector<std::vector<std::array<double,5>>> sections;
		std::vector<std::array<double,5>> section;
		std::vector<std::vector<std::array<double,5>>> sPhases;
		std::vector<std::vector<std::array<double,5>>> uPhases;
		std::vector<std::vector<std::array<double,5>>> ePhases;
		std::ofstream os[12];
		std::array<std::vector<std::vector<double>>,12> goalDistributionss;
		std::vector<std::vector<double>> goalDistributions;
		std::vector<double> bins;
		std::array<std::vector<double>,12> binss;
		std::vector<std::array<double,2>> distrSorter;
		std::array<std::vector<std::array<double,2>>,12> distrSorters;
		double frameWeighting;
		std::string filePath;
		
		std::vector<int> sizes;
		std::vector<int> sizesb;
		//TESTING VARIABLES
		int count{0};
		int timeToPrint;
		std::vector<int> tempPossession;
	public:
		PressureProcessor(double weighting,std::string);
		void addPressure(std::array<double,5>);
		void addFinalPressure();
		std::array<int,3> lengthThreshold(double timeLength, double size, bool increasing, bool savePhases, int lookieLookie, int interestingTeam, bool test);
		void addRates();
		void calcPressure(bool write,bool s,bool us, bool es, bool defVel, int fromEnd, int fromStart);
		void openStreams();
		void closeStreams();
		void clearPhases();
		void getInTheBin(std::array<double,2> rubbish);
		void getInTheBin(std::array<double,2> rubbish,int number_of_defenders);
		void setBins();
		void setBins(bool);
		std::vector<std::array<double,6>> getStats(std::ofstream&);
		std::vector<std::array<double,6>> getStats(int def, std::ofstream&);
		void printBinSize();
		void printBinSize(int def);
		void autoBins(int n);
		bool autoBins(int n, int def);
		void printBins();
	};

	
#endif
