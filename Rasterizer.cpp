#include "Rasterizer.h"

namespace EFTDEM {
	void Rasterizer::rasterizePointCloud(PointCloud &pointCloud, SYCLState &syclState, const int debug) {
		std::cout << "Rasterizing point cloud...\n";

		const auto [width, height, mins, maxs, ignore1, ignore2, ignore3] = pointCloud;

		sycl::buffer<std::size_t> pointCountsBuffer{sycl::range<1>{pointCloud.heights.size()}};

		syclState.queue.submit([&](sycl::handler &handler) {
			const sycl::accessor pointCounts{pointCountsBuffer, handler, sycl::read_write, sycl::no_init};

			handler.fill(pointCounts, 0ul);
		});

		syclState.queue.submit([&](sycl::handler &handler) {
			const sycl::accessor points{syclState.pointsBuffer, handler, sycl::read_only};
			const sycl::accessor indices{syclState.gridCellIndicesBuffer, handler, sycl::read_only};
			const sycl::accessor heights{syclState.heightsBuffer, handler, sycl::read_write};
			const sycl::accessor pointCounts{pointCountsBuffer, handler, sycl::read_write};

			handler.parallel_for(pointCloud.points.size(), [=](const sycl::id<1> id) {
				const auto normalizedHeight = static_cast<float>((points[id].z - mins.z) / (maxs.z - mins.z));

				sycl::atomic_ref<float, sycl::memory_order_relaxed, sycl::memory_scope_device>{heights[indices[id]]} += normalizedHeight;
				++sycl::atomic_ref<unsigned long, sycl::memory_order_relaxed, sycl::memory_scope_device>{pointCounts[indices[id]]};
			});
		});

		syclState.queue.submit([&](sycl::handler &handler) {
			const sycl::accessor heights{syclState.heightsBuffer, handler, sycl::read_write};
			const sycl::accessor pointCounts{pointCountsBuffer, handler, sycl::read_only};

			handler.parallel_for(pointCloud.heights.size(), [=](const sycl::id<1> id) {
				heights[id] /= static_cast<float>(pointCounts[id]);
			});
		});

		if (debug) printOutput(pointCloud, syclState, debug);
	}

	void Rasterizer::printOutput(const PointCloud &pointCloud, SYCLState &syclState, const int approximateNumLines) {
		const sycl::host_accessor heights{syclState.heightsBuffer};

		std::cout << "\nHeights:\n";
		for (std::size_t i = 0; i < pointCloud.heights.size(); i += pointCloud.heights.size() / approximateNumLines) std::cout << "\t" << i << ": " << heights[i] << "\n";
		std::cout << "\t[...]\n\n";
	}
} // EFTDEM
