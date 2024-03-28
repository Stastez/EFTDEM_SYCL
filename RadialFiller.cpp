
#include "RadialFiller.h"

namespace EFTDEM {
	void RadialFiller::fill(const PointCloud &pointCloud, SYCLState &syclState, const unsigned numSteps, const int debug) {
		std::cout << "Filling point cloud " << numSteps << " times...\n";

		sycl::buffer<float, 2> heightsCopyBuffer{sycl::range<2>{pointCloud.width, pointCloud.height}};

		for (auto i = 0u; i < numSteps; ++i) {
			syclState.queue.submit([&](sycl::handler &handler) {
				const bool evenIteration = !(i % 2);

				const sycl::accessor<float, 2> source{evenIteration ? syclState.heightsBuffer : heightsCopyBuffer, handler, sycl::read_only, sycl::no_init};
				const sycl::accessor<float, 2> destination{evenIteration ? heightsCopyBuffer : syclState.heightsBuffer, handler, sycl::write_only, sycl::no_init};

				handler.parallel_for(sycl::range<2>{pointCloud.width, pointCloud.height}, [=](const sycl::item<2> &item) {
					auto accumulator = 0.f;
					auto numNonZero = 0.f;
					const auto id = item.get_id();
					const auto range = item.get_range();

					for (auto x = -1; x <= 1; ++x) {
						for (auto y = -1; y <= 1; ++y) {
							const auto index = sycl::id<2>{sycl::clamp(id[0] + x, 0ul, range[0] - 1), sycl::clamp(id[1] + y, 0ul, range[1] - 1)};
							const auto value = source[index];
							accumulator += value;
							numNonZero += static_cast<float>(value > 0);
						}
					}

					// Don't fill pixels that already have non-zero values
					destination[id] = sycl::mix(accumulator / sycl::max(numNonZero, 1.f), source[id], static_cast<float>(source[id] > 0));
				});
			});
		}

		if (numSteps % 2 == 1) {
			syclState.queue.submit([&](sycl::handler &handler) {
				const sycl::accessor<float, 2> source{heightsCopyBuffer, handler, sycl::read_only};
				const sycl::accessor<float, 2> destination{syclState.heightsBuffer, handler, sycl::write_only};

				handler.copy(source, destination);
			});
		}

		if (debug) printOutput(pointCloud, syclState, debug);
	}

	void RadialFiller::printOutput(const PointCloud &pointCloud, SYCLState &syclState, const int approximateNumLines) {
		const sycl::host_accessor heights{syclState.heightsBuffer};

		std::cout << "\nHeights:\n";
		for (std::size_t i = 0; i < pointCloud.heights.size(); i += pointCloud.heights.size() / approximateNumLines) std::cout << "\t" << i << ": " << heights[{i / pointCloud.width, i % pointCloud.width}] << "\n";
		std::cout << "\t[...]\n\n";
	}
} // EFTDEM
