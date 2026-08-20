#include "../include/pci2321.hpp"
#include <chrono>
#include <thread>
using namespace std;

int main() {
    Pci2321 dev;
    pair<Port, int> ports[] = {
        {Port::P1C, 20}, {Port::P1C, 21}, {Port::P1C, 22}, {Port::P1C, 23}, {Port::P2A, 29} // releases the brakes for the knee (bits 20, 21) and hip (bits 22, 23) joints
    };

    for (auto [pt, bit] : ports) {
        dev.configurePort(pt, Dir::Output);

        dev.writeChannel(pt, bit, true);
        this_thread::sleep_for(std::chrono::seconds(2));
        dev.writeChannel(pt, bit, false);
        this_thread::sleep_for(std::chrono::seconds(2));
    }
    return 0;
}
