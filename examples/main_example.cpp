#include <iostream>
#include <string>
#include <cstdio>
#include <chrono>
#include <thread>
#include <pci2321/pci2321.hpp>
using namespace std;

struct Relay {
    Port port;
    int bit;
    const char* name;
};

const Relay POWER  = { Port::P2A, 5, "POWER" };
const Relay BRAKE1 = { Port::P1C, 4, "BRAKE 1" };
const Relay BRAKE2 = { Port::P1C, 5, "BRAKE 2" };
const Relay BRAKE3 = { Port::P1C, 6, "BRAKE 3" };
const Relay BRAKE4 = { Port::P1C, 7, "BRAKE 4" };

void waitForEnter(const string& message)
{
    int c = 0;
    cout << message << endl;
    while (c != '\n' && c != EOF) {
        c = getchar();
    }
}

void testRelay(Pci2321& board, const Relay& relay)
{
    cout << "Test: " << relay.name << endl;

    if (relay.bit < 0 || relay.bit > 7) {
        cout << "Bit value out of range\n";
        return;
    }

    cout << "Bit used:" << relay.bit << endl;

    board.configurePort(relay.port, Dir::Output);

    waitForEnter("ENTER pentru activarea releului...");
    board.writeChannel(relay.port, relay.bit, true);
    this_thread::sleep_for(chrono::seconds(2));

    waitForEnter("Verifică fizic releul. ENTER pentru oprire...");
    board.writeChannel(relay.port, relay.bit, false);
    this_thread::sleep_for(chrono::seconds(2));
    cout << "Test finalized successfully!" << endl;
}

int main()
{
    try {
        Pci2321 board;

        // =============================================================
        // ETAPA A - TEST INDIVIDUAL
        // La început se lasa NECOMENTATĂ O SINGURĂ linie testRelay().
        // Rulezi programul, verifici fizic, apoi oprești programul.
        // După confirmare comentezi linia și treci la următorul releu.
        // =============================================================

        // TODO 1: testează mai întâi POWER.
        // testRelay(board, POWER);

        // TODO 2: numai după ce POWER a fost confirmat, testează BRAKE 1.
        // testRelay(board, BRAKE1);

        // TODO 3: apoi BRAKE 2.
        // testRelay(board, BRAKE2);

        // TODO 4: apoi BRAKE 3.
        // testRelay(board, BRAKE3);

        // TODO 5: apoi BRAKE 4.
        // testRelay(board, BRAKE4);

        // =============================================================
        // ETAPA B - SECVENȚA FINALĂ
        // Rulează această parte NUMAI DUPĂ ce toate cele 5 relee
        // au fost testate separat și confirmate.
        // =============================================================

        // TODO: configurează ca OUTPUT porturile folosite de POWER și frâne.
        board.configurePort(POWER.port, Dir::Output);
        board.configurePort(BRAKE1.port, Dir::Output);
        board.configurePort(BRAKE2.port, Dir::Output);
        board.configurePort(BRAKE3.port, Dir::Output);
        board.configurePort(BRAKE4.port, Dir::Output);

        // cere ENTER înainte de POWER.
        waitForEnter("Apasa ENTER pentru a porni POWER...");
        // pornește POWER folosind board.writeChannel(...).
        board.writeChannel(POWER.port, POWER.bit, true);
        this_thread::sleep_for(chrono::seconds(2));
        // cere confirmare înainte să continui.
        waitForEnter("Apasa ENTER pentru a porni BRAKE 1...");


        // activează BRAKE 1.
        board.writeChannel(BRAKE1.port, BRAKE1.bit, true);
        this_thread::sleep_for(chrono::seconds(2));
        // cere confirmare înainte de BRAKE 2.
        waitForEnter("Apasa ENTER pentru a porni BRAKE 2...");

        // activează BRAKE 2.
        board.writeChannel(BRAKE2.port, BRAKE2.bit, true);
        this_thread::sleep_for(chrono::seconds(2));
        // cere confirmare înainte de BRAKE 3.
        waitForEnter("Apasa ENTER pentru a porni BRAKE 3...");

        // activează BRAKE 3.
        board.writeChannel(BRAKE3.port, BRAKE3.bit, true);
        this_thread::sleep_for(chrono::seconds(2));
        // cere confirmare înainte de BRAKE 4.
        waitForEnter("Apasa ENTER pentru a porni BRAKE 4...");

        // activează BRAKE 4.
        board.writeChannel(BRAKE4.port, BRAKE4.bit, true);
        this_thread::sleep_for(chrono::seconds(2));
        cout << "POWER + 4 BRAKES active" << endl;

        // așteaptă ENTER înainte de oprirea testului.
        waitForEnter("Apasa ENTER pentru a opri testul...");

        // oprește releele de frână în ordinea stabilită la laborator.
        board.writeChannel(BRAKE1.port, BRAKE1.bit, false);
        board.writeChannel(BRAKE2.port, BRAKE2.bit, false);
        board.writeChannel(BRAKE3.port, BRAKE3.bit, false);
        board.writeChannel(BRAKE4.port, BRAKE4.bit, false);
        this_thread::sleep_for(chrono::seconds(2));
        // oprește POWER la final.
        board.writeChannel(POWER.port, POWER.bit, false);

        return 0;
    }
    catch (const exception& e) {
        cerr << "Eroare: " << e.what() << endl;
        return 1;
    }
}
