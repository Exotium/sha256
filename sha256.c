#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <arpa/inet.h> // included for htonl and ntohl


// circular right shift
uint rot(uint n, uint shamm) { return (n >> shamm) | (n << 32 - shamm); }

// get first prime larger than n
int nextprime(int n) {
    for (int s, i; s = 1, n++;) {
        int isprime = 1;
        for (i = 2; i <= n / 2; i++) isprime &= n % i != 0;
        if (isprime) return n;
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        puts("expected 1 argument");
        exit(1);
    }

    int textlen = strlen(argv[1]);
    // add 1 for "1"-bit at end of message
    // add 8 for message length at end of block
    // round up to multiple of 64
    int buflen = (textlen + 1 + 8) + 63 & ~63;

    // init message block
    char *buf = malloc(buflen);
    memset(buf, 0, buflen);
    memcpy(buf, argv[1], textlen);
    buf[textlen] = 128; // left-aligned 1
    *(int *) (buf + buflen - 4) = htonl(textlen * 8);

    // generate primes for round constant and initial hash value generation
    int primes[64];
    for (int i = 0, p = 1; i < 64; ++i) primes[i] = p = nextprime(p);

    // init hash values
    int h[8];
    for (int i = 0; i < 8; ++i) {
        double froot = sqrt(primes[i]);
        long long iroot = *(long long *) &froot;
        int exp = (iroot >> 52 & 2047) - 1023;
        h[i] = iroot >> 20 - exp;
    }

    // init round constants
    int K[64];
    for (int i = 0; i < 64; ++i) {
        double froot = cbrt(primes[i]);
        long long iroot = *(long long *) &froot;
        int exp = (iroot >> 52 & 2047) - 1023;
        K[i] = iroot >> 20 - exp;
    }

    // for each 512-bit chunk in the message block
    for (int i = 0; i < buflen / 64; ++i) {
        // copy chunk into message schedule [0,15] and extend to fill it
        uint w[64];
        for (int j = 0; j < 16; ++j) w[j] = ntohl(((uint *) buf)[j + i * 16]);
        for (int j = 0; j < 48; ++j)
            w[j + 16] = w[j] + w[j + 9] +
                        (rot(w[j + 1], 7) ^ rot(w[j + 1], 18) ^ (w[j + 1] >> 3)) +
                        (rot(w[j + 14], 17) ^ rot(w[j + 14], 19) ^ (w[j + 14] >> 10));

        // initialize working variables
        int working[8];
        memcpy(working, h, 32);

        // compression main loop
        for (int j = 0; j < 64; ++j) {
            int e = working[4];
            int S1 = rot(e, 6) ^ rot(e, 11) ^ rot(e, 25);
            int ch = (e & working[5]) ^ (~e & working[6]);
            int temp1 = working[7] + S1 + ch + K[j] + w[j];
            int a = working[0];
            int S0 = rot(a, 2) ^ rot(a, 13) ^ rot(a, 22);
            int maj = (a & working[1]) ^ (a & working[2]) ^ (working[1] & working[2]);
            int temp2 = S0 + maj;

            memmove(working + 1, working, 28);
            working[0] = temp1 + temp2;
            working[4] += temp1;
        }

        // add compressed chunk to hash value
        for (int j = 0; j < 8; ++j) h[j] += working[j];
    }

    // print the hash
    for (int i = 0; i < 8; ++i) printf("%08x", h[i]);

    free(buf);
}
