#include "Sorter.h"

namespace EFTDEM {
	void Sorter::sortPointCloud(PointCloud &pointCloud, SYCLState &syclState, const int debug) {
		std::cout << "Sorting point cloud into a " << pointCloud.width << "x" << pointCloud.height << " grid...\n";

		const auto [width, height, mins, maxs, ignore1, ignore2] = pointCloud;

		syclState.pointsBuffer = {pointCloud.points};
		syclState.pointsBuffer.value().set_write_back(false);
		syclState.gridCellIndicesBuffer = {sycl::range<1>{pointCloud.points.size()}};

		syclState.queue.submit([&](sycl::handler &handler) {
			const sycl::accessor points{syclState.pointsBuffer.value(), handler, sycl::read_only, sycl::no_init};
			const sycl::accessor indices{syclState.gridCellIndicesBuffer.value(), handler, sycl::write_only, sycl::no_init};

			handler.parallel_for(pointCloud.points.size(), [=](const sycl::id<1> id) {
				const sycl::vec<double, 2> normalizedCoordinates{
					(points[id].x - mins.x) / (maxs.x - mins.x),
					(points[id].y - mins.y) / (maxs.y - mins.y)
				};

				const sycl::vec<unsigned long, 2> cell{
					sycl::min(width - 1, static_cast<unsigned long>(normalizedCoordinates.x() * static_cast<double>(width))),
					sycl::min(height - 1, static_cast<unsigned long>(normalizedCoordinates.y() * static_cast<double>(height)))
				};

				indices[id] = cell.y() * width + cell.x();
			});
		});

		if (debug) printOutput(pointCloud, syclState, debug);
	}

	void Sorter::printOutput(const PointCloud &pointCloud, SYCLState &syclState, const int approximateNumLines) {
		const sycl::host_accessor indices{syclState.gridCellIndicesBuffer.value()};

		std::cout << "\nIndices:\n";
		for (std::size_t i = 0; i < pointCloud.points.size(); i += pointCloud.points.size() / approximateNumLines) std::cout << "\t" << i << ": " << indices[i] << "\n";
		std::cout << "\t[...]\n\n";
	}
} // EFTDEM
