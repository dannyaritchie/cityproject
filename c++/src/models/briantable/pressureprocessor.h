#ifndef pressureprocessor
#define pressureprocessor
#include <vector>
#include<array>
#include <fstream>
#include <iostream>

	class PressureProcessor {
	private:
		int previousFid;
		std::vector<std::vector<std::array<double,4>>> sections;
		std::vector<std::array<double,4>> section;
		std::vector<std::vector<std::array<double,4>>> sPhases;
		std::vector<std::vector<std::array<double,4>>> uPhases;
		std::ofstream os[10];
		std::vector<std::vector<double>> goalDistributions;
		std::vector<double> bins;
		std::vector<std::array<double,2>> distrSorter;
	public:
		PressureProcessor();
		void addPressure(std::array<double,4>);
		void addFinalPressure();
		std::array<int,2> lengthThreshold(double timeLength, double size, bool increasing, bool savePhases, int lookieLookie);
		void addRates();
		void calcPressure(bool write,bool s,bool us);
		void openStreams();
		void closeStreams();
		void clearPhases();
		void getInTheBin(std::array<double,2> rubbish);
		void setBins(std::vector<double> bins);
		void setBins();
		std::vector<std::array<double,6>> getStats();
		void printBinSize();
		void autoBins(int n);
		void printBins();
	};

	
#endif
