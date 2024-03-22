#include <iostream>
#include <sycl/sycl.hpp>

#include "MobileMappingReader.h"
#include "PngWriter.h"
#include "Rasterizer.h"
#include "Sorter.h"
#include "SYCLState.h"

int main(const int argc, const char *argv[]) {
	if (argc < 2) {
		std::cerr << "Provide a path to a point cloud file!\n";
		return 1;
	} else if (argc < 3) {
		std::cerr << "Provide a path to output to!\n";
		return 1;
	} else if (argc < 5) {
		std::cerr << "Provide both a width and height resolution!\n";
		return 1;
	}

	int debug;
	if (argc > 5) debug = std::max(std::stoi(argv[5]), EFTDEM_DEBUG);
	else debug = EFTDEM_DEBUG * 25;

	EFTDEM::PointCloud pointCloud{std::stoul(argv[3]), std::stoul(argv[4])};

	EFTDEM::MobileMappingReader::readPointsFromFile(pointCloud, argv[1], debug);

	pointCloud.gridCellIndices = std::vector<std::size_t>(pointCloud.points.size(), 0);
	pointCloud.heights = std::vector<float>(pointCloud.width * pointCloud.height, 0);

	// Trigger write back to host on buffer destruction
	{
		EFTDEM::SYCLState syclState{
			sycl::queue{sycl::gpu_selector_v},
			{pointCloud.points},
			{pointCloud.gridCellIndices},
			{pointCloud.heights}
		};
		std::cout << "Selected " << syclState.queue.get_device().get_info<sycl::info::device::name>() << " as the SYCL device.\n";

		EFTDEM::Sorter::sortPointCloud(pointCloud, syclState, debug);

		EFTDEM::Rasterizer::rasterizePointCloud(pointCloud, syclState, debug);
	}

	EFTDEM::PngWriter::exportPointCloud(pointCloud, argv[2]);

	return 0;
}
