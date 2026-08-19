#include "../include/pci2321.hpp"
#include <chrono>
#include <thread>
using namespace std;

int main() {
    Pci2321 dev;
    pair<Port, int> ports[] = {
        {Port::P1C, 18}, {Port::P1C, 19}, {Port::P2A, 28},
        {Port::P2A, 29}, {Port::P2A, 30},
        {Port::P2A, 31}
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
