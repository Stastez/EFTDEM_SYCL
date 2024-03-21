#include "Sorter.h"

namespace EFTDEM {
	void Sorter::sortPointCloud(PointCloud &pointCloud, sycl::queue &queue) {
		std::cout << "Sorting point cloud into a " << pointCloud.width << "x" << pointCloud.height << " grid...\n";

		pointCloud.gridCellIndices = std::vector<std::size_t>(pointCloud.points.size(), 0); {
			sycl::buffer pointsBuffer{pointCloud.points};
			sycl::buffer indicesBuffer{pointCloud.gridCellIndices};

			const auto [width, height, mins, maxs, ignore1, ignore2] = pointCloud;

			queue.submit([&](sycl::handler &handler) {
				const sycl::accessor points{pointsBuffer, handler, sycl::read_only};
				const sycl::accessor indices{indicesBuffer, handler, sycl::write_only};

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
			}).wait_and_throw();
		}

		std::cout << "\nIndices:\n";
		constexpr std::size_t maximumLines = 25;
		for (std::size_t i = 0; i < pointCloud.points.size(); i += pointCloud.points.size() / maximumLines) std::cout << "\t" << i << ": " << pointCloud.gridCellIndices[i] << "\n";
	}
} // EFTDEM
