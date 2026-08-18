#include <stdio.h>
#include <stdint.h>
#include <sys/io.h>        // iopl, inl, outl

#define IO_BASE  0xc000
#define P1_DATA  0x00      // byte0=P1A, byte1=P1B, byte2=P1C
#define P1_DIR   0x04
#define P2_DATA  0x10      // byte0=P2A, byte1=P2B, byte2=P2C
#define P2_DIR   0x14

static inline void wr(unsigned off, uint32_t v){ outl(v, IO_BASE + off); }

int main(void) {
    if (iopl(3) != 0) { perror("iopl (ruleaza ca root)"); return 1; }

    wr(P1_DIR, 0x07);      // ambele blocuri: A/B/C = output
    wr(P2_DIR, 0x07);
    wr(P1_DATA, 0);
    wr(P2_DATA, 0);

    struct { unsigned dat; const char *port; int shift; int dio0; } grp[] = {
        { P1_DATA, "P1A", 0,  0 }, { P1_DATA, "P1B", 8,  8 }, { P1_DATA, "P1C", 16, 16 },
        { P2_DATA, "P2A", 0, 24 }, { P2_DATA, "P2B", 8, 32 }, { P2_DATA, "P2C", 16, 40 },
    };

    for (unsigned g = 0; g < 6; ++g) {
        for (int bit = 0; bit < 8; ++bit) {
            wr(P1_DATA, 0);                                       // stinge tot (32-bit)
            wr(P2_DATA, 0);
            wr(grp[g].dat, (uint32_t)1 << (grp[g].shift + bit));  // un singur bit
            printf("%s bit %d  (DIO %2d, byte 0x%02x) -> ce releu/LED e ON? [ENTER] ",
                   grp[g].port, bit, grp[g].dio0 + bit, 1u << bit);
            fflush(stdout);
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
        }
    }
    wr(P1_DATA, 0);
    wr(P2_DATA, 0);
    puts("Gata. Toate iesirile stinse.");
    return 0;
}
