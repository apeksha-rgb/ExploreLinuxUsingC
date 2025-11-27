#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PACKET_SIZE 188   // MPEG2TS packet size

/* ------------ TIMER UTILITY ------------- */
double now_seconds() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

/* ------------ MAIN COPY LOGIC ------------- */
int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Usage: %s <source_file> <destination_file>\n", argv[0]);
        return 1;
    }

    FILE *src = fopen(argv[1], "rb");
    if (!src) {
        perror("Source open failed");
        return 1;
    }

    FILE *dst = fopen(argv[2], "wb");
    if (!dst) {
        perror("Destination open failed");
        fclose(src);
        return 1;
    }

    unsigned char packet[PACKET_SIZE];
    size_t bytes;

    double start = now_seconds();

    // Copy loop: one MPEG2TS packet at a time
    while ((bytes = fread(packet, 1, PACKET_SIZE, src)) == PACKET_SIZE) {
        fwrite(packet, 1, bytes, dst);
    }

    double end = now_seconds();
    printf("Non-buffered MPEG2TS copy completed in %.6f seconds\n", end - start);

    fclose(src);
    fclose(dst);

    return 0;
}