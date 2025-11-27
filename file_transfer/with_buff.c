#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CHUNK_SIZE 128
#define BUFFER_SIZE (16 * 1024)   // 16 KB

/* ------------ TIMER UTILITY ------------- */
double now_seconds() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

/* ------------ GLOBAL BUFFERS (OPTIONAL BUFFERED IO) ------------- */
#ifdef USE_BUFFERED_IO
static unsigned char read_buffer[BUFFER_SIZE];
static size_t read_buf_pos = 0;
static size_t read_buf_len = 0;

static unsigned char write_buffer[BUFFER_SIZE];
static size_t write_buf_pos = 0;
#endif

/* ------------ READ FUNCTION ------------- */
size_t read_chunk(FILE *fp, unsigned char *out) {

#ifndef USE_BUFFERED_IO
    // Direct read: 188 bytes
    return fread(out, 1, CHUNK_SIZE, fp);

#else
    // Buffered read
    if (read_buf_pos + CHUNK_SIZE > read_buf_len) {
        // refill buffer
        read_buf_len = fread(read_buffer, 1, BUFFER_SIZE, fp);
        read_buf_pos = 0;

        if (read_buf_len == 0)
            return 0; // no more data
    }

    size_t bytes = (read_buf_pos + CHUNK_SIZE <= read_buf_len)
                       ? CHUNK_SIZE
                       : read_buf_len - read_buf_pos;

    memcpy(out, read_buffer + read_buf_pos, bytes);
    read_buf_pos += bytes;

    return bytes;
#endif
}

/* ------------ WRITE FUNCTION ------------- */
void write_chunk(FILE *fp, const unsigned char *data, size_t len) {

#ifndef USE_BUFFERED_IO
    // Direct write
    fwrite(data, 1, len, fp);

#else
    // Buffered write
    if (write_buf_pos + len >= BUFFER_SIZE) {
        fwrite(write_buffer, 1, write_buf_pos, fp);
        write_buf_pos = 0;
    }

    memcpy(write_buffer + write_buf_pos, data, len);
    write_buf_pos += len;
#endif
}

/* ------------ FLUSH BUFFERED WRITE ------------- */
#ifdef USE_BUFFERED_IO
void flush_write(FILE *fp) {
    if (write_buf_pos > 0) {
        fwrite(write_buffer, 1, write_buf_pos, fp);
        write_buf_pos = 0;
    }
}
#endif

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

    unsigned char chunk[CHUNK_SIZE];
    size_t bytes;

    double start = now_seconds();

    // Copy loop
    while ((bytes = read_chunk(src, chunk)) > 0) {
        write_chunk(dst, chunk, bytes);
    }

    // flush writes
#ifdef USE_BUFFERED_IO
    flush_write(dst);
#endif

    double end = now_seconds();
    printf("Copy completed in %.6f seconds\n", end - start);

    fclose(src);
    fclose(dst);

    return 0;
}
