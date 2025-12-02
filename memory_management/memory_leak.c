#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define CHUNK_SIZE (1024 * 1024) // 1 MB
#define NUM_CHUNKS 1000          // ~1 GB

typedef struct Node {
    void *ptr;
    struct Node *next;
} Node;

int main() {
    Node *head = NULL;

    printf("Allocating %d chunks of 1MB each...\n", NUM_CHUNKS);

    for (int i = 0; i < NUM_CHUNKS; i++) {
        void *p = malloc(CHUNK_SIZE);
        if (!p) {
            printf("malloc failed at chunk %d\\n", i);
            break;
        }

        // Touch the memory so Linux actually commits it
        memset(p, 0xAA, CHUNK_SIZE);

        Node *n = malloc(sizeof(Node));
        n->ptr = p;
        n->next = head;
        head = n;

        printf("Allocated: %d MB\n", i + 1);
        usleep(10000); // slow down so you can watch top/htop update
    }

    printf("\nMemory allocated. Check RAM usage now.\n");
    printf("Press ENTER to free all memory...");
    getchar();

    // Free all memory
    Node *curr = head;
    while (curr) {
        free(curr->ptr);
        Node *tmp = curr;
        curr = curr->next;
        free(tmp);
    }

    printf("All memory freed. Check RAM usage again.\n");
    return 0;

}