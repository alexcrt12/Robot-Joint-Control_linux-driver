#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/io.h>        // iopl, outl, inl

#define IO_BASE  0xc000
#define P2_DATA  0x10
#define P2_DIR   0x14

int main(void) {
    if (iopl(3) != 0) { perror("iopl (ruleaza ca root)"); return 1; }
    outl(0x01, IO_BASE + P2_DIR);    // P2A -> output
    outl(0x20, IO_BASE + P2_DATA);   // P2A bit5 (pin 29) ON
    sleep(2);
    outl(0x00, IO_BASE + P2_DATA);   // OFF
    return 0;
}
