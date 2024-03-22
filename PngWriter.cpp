
#include <lodepng.h>
#include <iostream>

#include "PngWriter.h"

namespace EFTDEM {
	void PngWriter::exportPointCloud(const PointCloud &pointCloud, const std::string &path) {
		std::cout << "Outputting to " << path << "...\n";

		std::vector<unsigned char> imageData;
		imageData.reserve(pointCloud.heights.size());

		for (std::size_t yReverse = 1; yReverse <= pointCloud.height; ++yReverse) {
			for (std::size_t x = 0; x < pointCloud.width; ++x) {
				const auto index = pointCloud.width * (pointCloud.height - yReverse) + x;

				for (auto i = 0; i < 3; ++i) imageData.emplace_back(static_cast<char>(255 * pointCloud.heights[index]));
				imageData.emplace_back(255);
			}
		}

		const auto error = lodepng::encode(path, imageData, pointCloud.width, pointCloud.height);
		if (error) {
			std::cout << lodepng_error_text(error) << "\n";
			throw std::exception{};
		}
	}
} // EFTDEM
