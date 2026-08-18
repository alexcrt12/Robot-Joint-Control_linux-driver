#include "../include/pci2321.hpp"

constexpr Pci2321::PortInfo Pci2321::port_mapping(Port port) {
    switch (port) {
        case Port::P1A: return { 0x00, 0x04, 0, 0 };
        case Port::P1B: return { 0x00, 0x04, 1, 1 };
        case Port::P1C: return { 0x00, 0x04, 2, 2 };
        case Port::P2A: return { 0x10, 0x14, 0, 0 };
        case Port::P2B: return { 0x10, 0x14, 1, 1 };
        case Port::P2C: return { 0x10, 0x14, 2, 2 };
    }
    // Fallback implicit pentru a evita warning-urile de compilare
    return { 0x00, 0x00, 0, 0 };
}

void Pci2321::writeChannel(Port port, int bit, bool on) {
    PortInfo info = port_mapping(port);
    uint32_t data_off   = info.data_off;
    int      byte_index = info.byte_index;
    int      bitpos     = byte_index * 8 + bit;      // pozitia in registrul de 32b

    uint32_t reg = io_.readReg(data_off);            // read
    if (on) reg |=  (1u << bitpos);                  // modify
    else    reg &= ~(1u << bitpos);
    io_.writeReg(data_off, reg);                     // write (toti 32 de biti)
}

void Pci2321::configurePort(Port port, Dir dir) {
    PortInfo info = port_mapping(port);
    uint32_t reg = io_.readReg(info.dir_off);
    if (dir == Dir::Output) {
        reg |=  (1u << info.dir_bit);
    } else {
        reg &= ~(1u << info.dir_bit);
    }

    io_.writeReg(info.dir_off, reg);
}
void Pci2321::writePort(Port port, uint8_t value) {
    PortInfo info = port_mapping(port);

    int shift = info.byte_index * 8;
    uint32_t reg = io_.readReg(info.data_off);

    // MODIFY
    reg &= ~(0xFFu << shift);
    reg |= (static_cast<uint32_t>(value) << shift);

    io_.writeReg(info.data_off, reg);
}

uint8_t Pci2321::readPort(Port port) {
    PortInfo info = port_mapping(port);

    int shift = info.byte_index * 8;
    // READ
    uint32_t reg = io_.readReg(info.data_off);
    // EXTRACT
    uint8_t value = static_cast<uint8_t>((reg >> shift) & 0xFFu);

    return value;
}

