#include "Sorter.h"

namespace EFTDEM {
	void Sorter::sortPointCloud(PointCloud &pointCloud, SYCLState &syclState) {
		std::cout << "Sorting point cloud into a " << pointCloud.width << "x" << pointCloud.height << " grid...\n";

		const auto [width, height, mins, maxs, ignore1, ignore2] = pointCloud;

		auto sorting = syclState.queue.submit([&](sycl::handler &handler) {
			const sycl::accessor points{syclState.pointsBuffer, handler, sycl::read_only};
			const sycl::accessor indices{syclState.gridCellIndicesBuffer, handler, sycl::write_only};

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

#if(DEBUG)
		// TODO: use host_task when available with AdaptiveCpp, temporary solution: call destructor and trigger copy back
		syclState.gridCellIndicesBuffer = {pointCloud.gridCellIndices};

		std::cout << "\nIndices:\n";
		constexpr std::size_t maximumLines = 25;
		for (std::size_t i = 0; i < pointCloud.points.size(); i += pointCloud.points.size() / maximumLines) std::cout << "\t" << i << ": " << pointCloud.gridCellIndices[i] << "\n";
#endif
	}
} // EFTDEM
