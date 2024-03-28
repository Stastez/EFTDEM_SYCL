#pragma once
#include <limits>
#include <vector>

#include "Point.h"

namespace EFTDEM {
	class PointCloud {
	public:
		unsigned long width, height;
		Point<double> mins{std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()};
		Point<double> maxs{-std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity()};

		std::vector<Point<double> > points;
		std::vector<float> heights;

		PointCloud(const unsigned long width, const unsigned long height) : width(width), height(height) {
		}
	};
}
