#include "../include/pci2321.hpp"
#include <chrono>
#include <thread>

int main() {
    Pci2321 dev;
    const Port ports[] = {Port::P1A,Port::P1B,Port::P1C,Port::P2A,Port::P2B,Port::P2C};
    for (Port pt : ports) dev.configurePort(pt, Dir::Output);

    for (Port pt : ports) {
        for (int bit = 0; bit < 8; ++bit) {
            dev.writeChannel(pt, bit, true);
            this_thread::sleep_for(std::chrono::seconds(2));
            dev.writeChannel(pt, bit, false);
            this_thread::sleep_for(std::chrono::seconds(2));
        }
    }
    return 0;
}
