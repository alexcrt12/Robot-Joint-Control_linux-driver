#include <io_backend.hpp>
#include <pci2321.hpp>

#include <pybind11/pybind11.h>

namespace py = pybind11;

// "pci2321" will be the name of the Python module you import
PYBIND11_MODULE(pci2321, m) {
    m.doc() = "Python bindings for the PCI-2321 IO card library";

    // 1. Expose the IoBackend class
    py::class_<IoBackend>(m, "IoBackend")
        .def(py::init<const std::string&>(), py::arg("bdf"), "Initialize with BDF string")
        .def("writeReg", &IoBackend::writeReg, py::arg("offset"), py::arg("value"))
        .def("readReg", &IoBackend::readReg, py::arg("offset"));

    // 2. Expose the Port enum
    py::enum_<Port>(m, "Port")
        .value("P1A", Port::P1A)
        .value("P1B", Port::P1B)
        .value("P1C", Port::P1C)
        .value("P2A", Port::P2A)
        .value("P2B", Port::P2B)
        .value("P2C", Port::P2C);

    // 3. Expose the Dir enum
    py::enum_<Dir>(m, "Dir")
        .value("Input", Dir::Input)
        .value("Output", Dir::Output);

    // 4. Expose the Pci2321 class
    py::class_<Pci2321>(m, "Pci2321")
        // The '= "0000:05:01.0"' tells pybind11 to use this default if Python omits it
        .def(py::init<const std::string&>(), py::arg("bdf") = "0000:05:01.0")
        .def("configurePort", &Pci2321::configurePort, py::arg("port"), py::arg("dir"))
        .def("writeChannel", &Pci2321::writeChannel, py::arg("port"), py::arg("bit"), py::arg("on"))
        .def("writePort", &Pci2321::writePort, py::arg("port"), py::arg("value"))
        .def("readPort", &Pci2321::readPort, py::arg("port"));
}