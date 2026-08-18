#ifndef PRACTICA_2026_PCI2321_HPP
#define PRACTICA_2026_PCI2321_HPP

#pragma once
#include "io_backend.hpp"
#include <cstdint>
#include <string>
using namespace std;

enum class Port { P1A, P1B, P1C, P2A, P2B, P2C };
enum class Dir  { Input, Output };

class Pci2321 {
public:
    explicit Pci2321(const std::string& bdf = "0000:05:01.0") : io_(bdf) {}

    void    configurePort(Port port, Dir dir);
    void    writeChannel (Port port, int bit, bool on); // 1 canal, RMW pe 32b
    void    writePort    (Port port, uint8_t value);    // 8 biti ai portului, RMW pe 32b
    uint8_t readPort     (Port port);

private:
    IoBackend io_;
    struct PortInfo {
        uint32_t data_off;
        uint32_t dir_off;
        int byte_index;
        int dir_bit;
    };
    static constexpr PortInfo port_mapping(Port port);
};

#endif //PRACTICA_2026_PCI2321_HPP
