#include "../include/io_backend.hpp"
#include <sys/io.h>     // iopl, outl, inl
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <string>
using namespace std;

void IoBackend::enableDevice(const std::string& bdf) {
    ofstream f("/sys/bus/pci/devices/" + bdf + "/enable");
    if (!f) throw runtime_error("nu pot activa placa (root?)");
    f << "1";
}

// A doua linie din .../resource = BAR1; primul token hex = adresa de start (0xc000).
uint16_t IoBackend::readBar1Base(const string& bdf) {
    ifstream f("/sys/bus/pci/devices/" + bdf + "/resource");
    if (!f) throw runtime_error("nu pot citi resource");
    string line;

    getline(f, line);   // BAR0 (memorie)

    if (!getline(f, line) || line.empty()) { // BAR1 (I/O)
        throw runtime_error("BAR1 nu este valid sau nu exista");
    }

    uint64_t bar1_base = 0;
    istringstream iss(line);

    if (!(iss >> hex >> bar1_base)) {
        throw runtime_error("eroare de parsing BAR1");
    }

    return static_cast<uint16_t>(bar1_base);
}

IoBackend::IoBackend(const string& bdf) {
    enableDevice(bdf);
    io_base_ = readBar1Base(bdf);
    if (iopl(3) != 0) throw runtime_error("iopl a esuat (root / CAP_SYS_RAWIO)");
}

IoBackend::~IoBackend() {}

void     IoBackend::writeReg(uint32_t off, uint32_t v) { outl(v, io_base_ + off); }
uint32_t IoBackend::readReg (uint32_t off)             { return inl(io_base_ + off); }
