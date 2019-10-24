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
	public:
		PressureProcessor();
		void addPressure(std::array<double,4>);
		void addFinalPressure();
		std::array<int,2> lengthThreshold(double timeLength, double size, bool increasing, bool savePhases, bool lookieLookie);
		void addRates();
		void calcPressure();
		void openStreams();
		void closeStreams();
		void clearPhases();
	};
	
#endif
