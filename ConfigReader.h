
#pragma once
#include <string>

namespace EFTDEM {

struct Config {
	std::string inputPath, outputPath;
	std::size_t width, height, numClosingIterations;
	bool useGPU;
	int numDebugLines;
};

class ConfigReader {
public:
	static Config readConfig(const std::string& configPath);
};

} // EFTDEM
