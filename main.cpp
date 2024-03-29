#include <iostream>
#include <sycl/sycl.hpp>

#include "MobileMappingReader.h"
#include "PngWriter.h"
#include "RadialFiller.h"
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
	} else if (argc < 6) {
		std::cerr << "Provide whether to use the GPU!\n";
		return 1;
	}

	int debug;
	if (argc > 6) debug = std::max(std::stoi(argv[6]), EFTDEM_DEBUG);
	else debug = EFTDEM_DEBUG * 25;

	EFTDEM::PointCloud pointCloud{std::stoul(argv[3]), std::stoul(argv[4])};

	EFTDEM::MobileMappingReader::readPointsFromFile(pointCloud, argv[1], debug);

	pointCloud.heights = std::vector<float>(pointCloud.width * pointCloud.height, 0);

	const auto startTime = std::chrono::high_resolution_clock::now();

	// Trigger write back to host on buffer destruction
	{
		sycl::queue queue;
		if (std::stoi(argv[5])) queue = sycl::queue{sycl::gpu_selector_v, sycl::property::queue::in_order{}};
		else queue = sycl::queue{sycl::cpu_selector_v, sycl::property::queue::in_order{}};

		EFTDEM::SYCLState syclState{
			queue,
			{pointCloud.points},
			{sycl::range<1>{pointCloud.points.size()}},
			// This does not initialize heightsBuffer with the values in heights
			{pointCloud.heights.data(), sycl::range<2>{pointCloud.width, pointCloud.height}}
		};
		syclState.pointsBuffer.set_write_back(false);
		std::cout << "Selected " << syclState.queue.get_device().get_info<sycl::info::device::name>() << " as the SYCL device.\n";

		EFTDEM::Sorter::sortPointCloud(pointCloud, syclState, debug);

		EFTDEM::Rasterizer::rasterizePointCloud(pointCloud, syclState, debug);

		EFTDEM::RadialFiller::fill(pointCloud, syclState, 300, debug);
	}

	const auto endTime = std::chrono::high_resolution_clock::now();

	std::cout << "Elapsed time for SYCL operations: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << " ms\n";

	EFTDEM::PngWriter::exportPointCloud(pointCloud, argv[2]);

	return 0;
}
