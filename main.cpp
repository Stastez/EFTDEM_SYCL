#include <iostream>
#include <sycl/sycl.hpp>

#include "MobileMappingReader.h"
#include "Sorter.h"
#include "SYCLState.h"

int main(const int argc, const char *argv[]) {
	if (argc < 2) {
		std::cerr << "Provide a path to a point cloud file!\n";
		return 1;
	} else if (argc < 4) {
		std::cerr << "Provide both a width and height resolution!\n";
		return 1;
	}

	EFTDEM::PointCloud pointCloud{std::stoul(argv[2]), std::stoul(argv[3])};

	EFTDEM::MobileMappingReader::readPointsFromFile(pointCloud, argv[1], true);

	pointCloud.gridCellIndices = std::vector<std::size_t>(pointCloud.points.size(), 0);

	EFTDEM::SYCLState syclState{
		sycl::queue{
			sycl::gpu_selector_v, [](const sycl::exception_list &exceptions) {
				for (auto &exception: exceptions) {
					try {
						std::rethrow_exception(exception);
					} catch (sycl::exception &e) {
						std::cerr << e.what();
					}
				}

				std::terminate();
			}
		},
		{pointCloud.points},
		{pointCloud.gridCellIndices}
	};
	std::cout << "Selected " << syclState.queue.get_device().get_info<sycl::info::device::name>() << " as the SYCL device.\n";

	EFTDEM::Sorter::sortPointCloud(pointCloud, syclState);

	return 0;
}
