#include "MobileMappingReader.h"

#include <array>
#include <fstream>
#include <iostream>

namespace EFTDEM {
	void MobileMappingReader::readPointsFromFile(PointCloud &pointCloud, const std::string &path, const bool printOutput) {
		std::cout << "Reading point cloud at " << path << "...\n";

		std::ifstream file{path, std::ios::in};
		if (!file.is_open()) {
			std::cerr << "Point cloud at " << path << " could not be opened!\n";
			throw std::exception{};
		}

		std::array<std::string, 5> dataColumns;
		std::string currentLine;

		getline(file, currentLine);
		while (getline(file, currentLine)) {
			std::size_t lastPosition = 0;
			for (auto &column: dataColumns) {
				auto delimiterPosition = currentLine.find(',', lastPosition);
				column = currentLine.substr(lastPosition,
				                            std::min(delimiterPosition - lastPosition,
				                                     currentLine.size() - lastPosition));
				lastPosition = delimiterPosition + 1;
			}

			if (dataColumns[3] != "1") continue;

			Point<double> p{std::stod(dataColumns[0]), std::stod(dataColumns[1]), std::stod(dataColumns[2])};
			pointCloud.points.emplace_back(p);

			for (auto axis = 0; axis < 3; ++axis) {
				if (p[axis] < pointCloud.mins[axis]) pointCloud.mins[axis] = p[axis];
				if (p[axis] > pointCloud.maxs[axis]) pointCloud.maxs[axis] = p[axis];
			}
		}

		if (printOutput) MobileMappingReader::printOutput(pointCloud);
	}

	void MobileMappingReader::printOutput(const PointCloud &pointCloud) {
		std::cout << "\nLimits:\n";
		std::cout << "\tx: " << "{" << pointCloud.mins.x << ", " << pointCloud.maxs.y << "}\n";
		std::cout << "\ty: " << "{" << pointCloud.mins.x << ", " << pointCloud.maxs.y << "}\n";
		std::cout << "\tz: " << "{" << pointCloud.mins.x << ", " << pointCloud.maxs.y << "}\n";

		std::cout << "Points:\n";
		constexpr std::size_t maximumLines = 25;
		for (std::size_t i = 0; i < pointCloud.points.size(); i += pointCloud.points.size() / maximumLines) {
			const auto& point = pointCloud.points[i];
			std::cout << "\t" << i << ": {" << point.x << ", " << point.y << ", " << point.z << "}\n";

			if (i == maximumLines - 1) std::cout << "\t[...]\n\n";
		}
	}

} // EFTDEM
