#include <iostream>

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>

#include "distribution.hpp"

NB_MODULE(wasserstein_mcf_ext, m) {
    m.doc() = "Wasserstein MCF extension module";

    // Define a simple function to demonstrate the module
    m.def("wasserstein_mcf_hello_world", []() {
        std::cout << "Hello from Wasserstein MCF!" << std::endl;
    }, "A simple hello world function for the Wasserstein MCF extension");
}
