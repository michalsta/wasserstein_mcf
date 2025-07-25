#ifndef PYLMCF_SPECTRUM_HPP
#define PYLMCF_SPECTRUM_HPP

#include <array>
#include <vector>
#include <stdexcept>
#include <span>
#include <utility>


#include "pylmcf/basics.hpp"

#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/array.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/function.h>

namespace nb = nanobind;

class Distribution {
    using Point_t = std::pair<const py::array*, size_t>;
    const nb::array py_positions;
    const nb::array_t<LEMON_INT> py_intensities;
public:
    const std::span<const LEMON_INT> intensities;

    Distribution(nb::array positions, nb::array_t<LEMON_INT> intensities)
        : py_positions(positions), py_intensities(intensities), intensities(numpy_to_span(intensities)) {
        if (positions.shape()[1] != intensities.shape()[0]) {
            throw std::invalid_argument("Positions and intensities must have the same size");
        }
    }

    size_t size() const {
        return intensities.size();
    }

    Point_t get_point(size_t idx) const {
        if (idx >= size()) {
            throw std::out_of_range("Index out of range");
        }
        return {&py_positions, idx};
    }

    std::pair<std::vector<size_t>, std::vector<LEMON_INT>> closer_than(
        const Point_t point,
        const nb::function* wrapped_dist_fun,
        LEMON_INT max_dist
    ) const
    {
        std::vector<size_t> indices;
        std::vector<LEMON_INT> distances;

        nb::object distances_obj = (*wrapped_dist_fun)(point, py_positions);
        nb::array_t<LEMON_INT> distances_array = distances_obj.cast<py::array_t<LEMON_INT>>();
        nb::buffer_info distances_info = distances_array.request();
        LEMON_INT* distances_ptr = static_cast<LEMON_INT*>(distances_info.ptr);
        if (distances_info.ndim != 1) {
            throw std::invalid_argument("Only 1D arrays are supported");
        }
        for (size_t ii = 0; ii < size(); ++ii) {
            if(distances_ptr[ii] <= max_dist) {
                indices.push_back(ii);
                distances.push_back(distances_ptr[ii]);
            }
        }
        return {indices, distances};
    }

    const nb::array& py_get_positions() const {
        return py_positions;
    }

    const nb::array_t<LEMON_INT>& py_get_intensities() const {
        return py_intensities;
    }
};

#endif // PYLMCF_SPECTRUM_HPP