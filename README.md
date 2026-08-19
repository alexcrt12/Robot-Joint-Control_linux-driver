# Linux Driver for Rehab Robot
### Introduction
This is a linux driver for a Rehab Joint Controlled Robot.
The relay and board bindings to the ports are the following:

<img width="154" height="400" alt="image" src="https://github.com/user-attachments/assets/6b817f28-9e12-49c9-8484-72afd49decd1" />

The role of those relays is to release the brakes of the joints and enable the motors that move them, among other aspects that define the core functionality of the robot that's meant to be controlled via the implemented code.
If the code is implememnted correctly, in the Linux Terminal the following command shall work:
```
sweep_example
```
If it doesn't, the alternative will be advised to use:
```
sudo sweep_example
```
### Preliminary steps to take before running the linux command
Before we start we should try and use these commands to confirm that our libraries are fully functional.
```
sudo apt update
sudo apt install -y pciutils build-essential cmake git
```
As extra notes:
```
# pciutils -> lspci/setpci ; build-essential -> gcc/g++ ; cmake -> the library
# iopl/outl/inl - from glibc, through <sys/io.h> (x86_64) — nothing to install separately
```
To start off, let's confirm that the board is properly connected to the PC and the system recognizes the BARs.
```
lspci -nn | grep -i 2321
lspci -nnk -s 05:01.0
sudo lspci -nnvvv -s 05:01.0
cat /sys/bus/pci/devices/0000:05:01.0/resource
```
Expected output:
```
line 05:01.0 ... [2007:2321];
-nnk DOESN'T OUTPUT „Kernel driver in use” (PCI card should be free);
-nnvvv: Region 1: I/O ports at c000 [size=32], Control: I/O- Mem-;
resource - second line (BAR1) starts with 0x...c000 și and ends with the flag ...101 (I/O region).
```
Running the proof.c and pci2321_io.c test files:
```
echo 1 | sudo tee /sys/bus/pci/devices/0000:05:01.0/enable >/dev/null
gcc -O2 -Wall -o proof proof.c # replace proof with pci2321_io when testing it
sudo ./proof # replace proof with pci2321_io when testing it
```
Compressing the project as a library:
```
cmake -S . -B build
cmake --build build -j
sudo cmake --install build
sudo ldconfig
sudo sweep_example # this one is already mentioned at the beginning of the readme
```
Running the project without the root:
```
# 1) give the binary access to the I/O ports
sudo setcap cap_sys_rawio=ep /usr/local/bin/sweep_example
getcap /usr/local/bin/sweep_example      # confirms: cap_sys_rawio=ep
 
# 2) enables the card automatically at every boot , via a udev rule
sudo tee /etc/udev/rules.d/99-pci2321.rules >/dev/null <<'EOF'
ACTION=="add", SUBSYSTEM=="pci", ATTR{vendor}=="0x2007", ATTR{device}=="0x2321", ATTR{enable}="1"
EOF
sudo udevadm control --reload
sudo udevadm trigger --subsystem-match=pci
 
# 3) now runs as a regular command (without sudo)
sweep_example
```
### Extra Functionality added
If there's a need to use only a select number of ports and some their respective bits, under:
```
/examples/sweep_example_mod.cpp
```
there's an updated implementation for this specific purpose.
