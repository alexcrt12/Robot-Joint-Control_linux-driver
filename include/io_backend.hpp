#ifndef PRACTICA_2026_IO_BACKEND_HPP
#define PRACTICA_2026_IO_BACKEND_HPP

#include <cstdint>
#include <string>
using namespace std;

class IoBackend {
public:
    explicit IoBackend(const std::string& bdf);   // enable + citeste baza + iopl(3)
    ~IoBackend();
    IoBackend(const IoBackend&) = delete;
    IoBackend& operator=(const IoBackend&) = delete;

    void     writeReg(uint32_t offset, uint32_t value);  // outl (32-bit)
    uint32_t readReg (uint32_t offset);                  // inl  (32-bit)

private:
    uint16_t io_base_ = 0;    // ex: 0xc000, citit din sysfs
    static uint16_t readBar1Base(const string& bdf);
    static void     enableDevice(const string& bdf);
};

#endif //PRACTICA_2026_IO_BACKEND_HPP
