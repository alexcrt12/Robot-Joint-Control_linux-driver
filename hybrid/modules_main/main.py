import sys
import os
import time
from dataclasses import dataclass
import pci2321

# 1. Recreate the Relay struct using a Python dataclass
@dataclass
class Relay:
    port: pci2321.Port
    bit: int
    name: str

# Define the constants
POWER  = Relay(pci2321.Port.P2A, 5, "POWER")
BRAKE1 = Relay(pci2321.Port.P1C, 4, "BRAKE 1")
BRAKE2 = Relay(pci2321.Port.P1C, 5, "BRAKE 2")
BRAKE3 = Relay(pci2321.Port.P1C, 6, "BRAKE 3")
BRAKE4 = Relay(pci2321.Port.P1C, 7, "BRAKE 4")

# 2. Recreate the waitForEnter function
def wait_for_enter(message: str):
    input(message)

# 3. Recreate the testRelay function
def test_relay(board: pci2321.Pci2321, relay: Relay):
    print(f"Test: {relay.name}")

    if relay.bit < 0 or relay.bit > 7:
        print("Bit value out of range")
        return

    print(f"Bit used: {relay.bit}")

    wait_for_enter("ENTER pentru activarea releului...")
    board.write_channel(relay.port, relay.bit, True)
    time.sleep(2) # Replaces this_thread::sleep_for

    wait_for_enter("Verifică fizic releul. ENTER pentru oprire...")
    board.write_channel(relay.port, relay.bit, False)
    time.sleep(2)
    print("Test finalized successfully!\n")

# 4. Recreate the main execution block
def main():
    try:
        # Initialize the board
        board = pci2321.Pci2321()

        # =============================================================
        # ETAPA A - TEST INDIVIDUAL
        # =============================================================

        # test_relay(board, POWER)
        # test_relay(board, BRAKE1)
        # test_relay(board, BRAKE2)
        # test_relay(board, BRAKE3)
        # test_relay(board, BRAKE4)

        # =============================================================
        # ETAPA B - SECVENȚA FINALĂ
        # =============================================================

        # Configurează ca OUTPUT porturile folosite de POWER și frâne.
        print("Configuring hardware ports...")
        board.configure_port(pci2321.Port.P2A, pci2321.Dir.Output) # For POWER
        board.configure_port(pci2321.Port.P1C, pci2321.Dir.Output) # For all 4 BRAKES
        print("Ports configured successfully.\n")
        board.write_port(pci2321.Port.P1A, 0)
        board.write_port(pci2321.Port.P1B, 0)
        board.write_port(pci2321.Port.P1C, 0)
        board.write_port(pci2321.Port.P2A, 0)

        wait_for_enter("Apasa ENTER pentru a porni POWER...")
        board.write_channel(POWER.port, POWER.bit, True)
        time.sleep(2)

        wait_for_enter("Apasa ENTER pentru a porni BRAKE 1...")
        board.write_channel(BRAKE1.port, BRAKE1.bit, True)
        time.sleep(2)

        wait_for_enter("Apasa ENTER pentru a porni BRAKE 2...")
        board.write_channel(BRAKE2.port, BRAKE2.bit, True)
        time.sleep(2)

        wait_for_enter("Apasa ENTER pentru a porni BRAKE 3...")
        board.write_channel(BRAKE3.port, BRAKE3.bit, True)
        time.sleep(2)

        wait_for_enter("Apasa ENTER pentru a porni BRAKE 4...")
        board.write_channel(BRAKE4.port, BRAKE4.bit, True)
        time.sleep(2)

        print("POWER + 4 BRAKES active")

        wait_for_enter("Apasa ENTER pentru a opri testul...")

        board.write_channel(BRAKE1.port, BRAKE1.bit, False)
        board.write_channel(BRAKE2.port, BRAKE2.bit, False)
        board.write_channel(BRAKE3.port, BRAKE3.bit, False)
        board.write_channel(BRAKE4.port, BRAKE4.bit, False)
        time.sleep(2)

        board.write_channel(POWER.port, POWER.bit, False)

        board.write_port(pci2321.Port.P1A, 0)
        board.write_port(pci2321.Port.P1B, 0)
        board.write_port(pci2321.Port.P1C, 0)
        board.write_port(pci2321.Port.P2A, 0)
        time.sleep(1)
        print("All ports safely reset to 0.")

    except Exception as e:
        sys.stderr.write(f"Eroare: {e}\n")
        # Safety catch: Try to force ports to 0 even if the script crashes!
        try:
            board.write_port(pci2321.Port.P1A, 0)
            board.write_port(pci2321.Port.P1B, 0)
            board.write_port(pci2321.Port.P1C, 0)
            board.write_port(pci2321.Port.P2A, 0)
        except:
            pass
        sys.exit(1)

if __name__ == "__main__":
    main()