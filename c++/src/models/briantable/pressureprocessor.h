#ifndef pressureprocessor
#define pressureprocessor

class PressureProcessor {
	private:
		int previousFid;
		std::vector<std::vector<std::array<double,1>>> sections;
		std::vector<std::array<double,1> section;
		std::vector<std::vector<std::array<double,1>>> phases;
		std::vector<> rate;
	public:
		PressureProcessor();
		void addPressure(std::array<double,2>);
		int threshold(double timeLength, bool increasing)
		void addRates();
	
#endif
