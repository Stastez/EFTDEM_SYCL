
#include "ConfigReader.h"

#include <fstream>
#include <iostream>

namespace EFTDEM {
	Config ConfigReader::readConfig(const std::string &configPath) {
		std::ifstream configFile{configPath, std::ios::in};
		if (!configFile.is_open()) {
			std::cerr << "Config file at " << configPath << " could not be opened!\n";
			throw std::exception{};
		}

		Config config{
			"",
			"",
			0,
			0,
			0,
			false,
			0
		};
		std::string currentLine;
		bool informationIsInvalid = false;

		enum LINE_TYPE {
			IN,
			OUT,
			WIDTH,
			HEIGHT,
			GPU,
			DEBUG_LINES,
			CLOSING_ITERATIONS
		};
		constexpr unsigned valueStartIndex[] = {4, 5, 7, 8, 5, 12, 20};

		while (getline(configFile, currentLine)) {
			LINE_TYPE lineType;
			if (currentLine.starts_with("IN:")) lineType = IN;
			else if (currentLine.starts_with("OUT:")) lineType = OUT;
			else if (currentLine.starts_with("WIDTH:")) lineType = WIDTH;
			else if (currentLine.starts_with("HEIGHT:")) lineType = HEIGHT;
			else if (currentLine.starts_with("GPU:")) lineType = GPU;
			else if (currentLine.starts_with("DEBUG_LINES:")) lineType = DEBUG_LINES;
			else if (currentLine.starts_with("CLOSING_ITERATIONS:")) lineType = CLOSING_ITERATIONS;
			else continue;

			const auto lastValidPosition = currentLine.length() - 1;
			if (valueStartIndex[lineType] >= lastValidPosition) {
				std::cerr << currentLine << " is formatted incorrectly. Lines must conform to <NAME>: <VALUE>.\n";
				throw std::exception{};
			}

			switch (lineType) {
				case IN:
					config.inputPath = currentLine.substr(valueStartIndex[lineType]);
					break;
				case OUT:
					config.outputPath = currentLine.substr(valueStartIndex[lineType]);
					break;
				case WIDTH:
					config.width = std::stoul(currentLine.substr(valueStartIndex[lineType]));
					break;
				case HEIGHT:
					config.height = std::stoul(currentLine.substr(valueStartIndex[lineType]));
					break;
				case CLOSING_ITERATIONS:
					config.numClosingIterations = std::stoul(currentLine.substr(valueStartIndex[lineType]));
					break;
				case DEBUG_LINES:
					config.numDebugLines = std::stoi(currentLine.substr(valueStartIndex[lineType]));
					break;
				case GPU:
					// This only works if currentLine is not used anymore this iteration!
					currentLine = currentLine.substr(valueStartIndex[lineType]);
					if (currentLine == "YES" || currentLine == "yes" || currentLine == "TRUE" || currentLine == "true")
						config.useGPU = true;
					else if (currentLine == "NO" || currentLine == "no" || currentLine == "FALSE" || currentLine == "false")
						config.useGPU = false;
					else {
						std::cerr << "Invalid GPU usage flag, use (lower- or uppercase) YES, TRUE, NO or FALSE\n";
						informationIsInvalid = true;
					}
					break;
			}
		}

		if (config.inputPath.empty()) {
			std::cerr << "Input path (IN:) is missing or invalid!\n";
			informationIsInvalid = true;
		}
		if (config.outputPath.empty()) {
			std::cerr << "Output path (OUT:) is missing or invalid!\n";
			informationIsInvalid = true;
		}
		if (config.width == 0) {
			std::cerr << "Width (WIDTH:) is missing or invalid!\n";
			informationIsInvalid = true;
		}
		if (config.height == 0) {
			std::cerr << "Height (HEIGHT:) is missing or invalid!\n";
			informationIsInvalid = true;
		}
		if (config.numClosingIterations == 0) {
			std::cerr << "Number of closing iterations (CLOSING_ITERATIONS:) is missing or invalid!\n";
			informationIsInvalid = true;
		}

		if (informationIsInvalid) throw std::exception{};

		return config;
	}

} // EFTDEM