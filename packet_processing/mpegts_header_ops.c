#include <stdio.h>
#include <stdint.h> 

// Temporarily change structure alignment so that

// every field is packed on 1-byte boundaries (no padding)
#pragma pack(push, 1)

// MPEG-TS Header (4 bytes)
typedef struct {
uint8_t sync_byte;
unsigned transport_error_indicator : 1;
unsigned payload_unit_start_indicator : 1;
unsigned transport_priority : 1;
unsigned pid : 13;

unsigned scrambling_control : 2;
unsigned adaptation_field_control : 2;
unsigned continuity_counter : 4;
} TSHeader;

#pragma pack(pop) // Restore previous settings

// ---------------------------------------
// Print buffer in hex
// ---------------------------------------
void print_hex(uint8_t *buf, int len) {
for (int i = 0; i < len; i++)
printf("%02X ", buf[i]);
printf("\n");
}
// ---------------------------------------
// Print bits of a byte
// ---------------------------------------
void print_byte_binary(uint8_t b) {
for (int i = 7; i >= 0; i--)
printf("%d", (b >> i) & 1);
}


// ---------------------------------------
// Print buffer in binary
// ---------------------------------------
void print_binary(uint8_t *buf, int len) {
for (int i = 0; i < len; i++) {
print_byte_binary(buf[i]);
printf(" ");
}
printf("\n");
}
int main(){
    uint8_t packet[188] = {0};

// Initialize with a valid TS header
TSHeader *hdr = (TSHeader *)packet;

hdr->sync_byte = 0x47; // sync byte
hdr->transport_error_indicator = 0;
hdr->payload_unit_start_indicator = 1;
hdr->transport_priority = 0;
hdr->pid = 256;  // PID = 0x0100
hdr->scrambling_control = 0;
hdr->adaptation_field_control = 1; // payload only
hdr->continuity_counter = 5;

printf("Before manipulation:\n");
print_hex(packet, 4);
print_binary(packet, 4);

// --------------------------------------------------
// Manipulations
// --------------------------------------------------
printf("\nManipulating fields...\n");

hdr->pid = 0x1234;                          // change PID
hdr->continuity_counter = (hdr->continuity_counter + 1) & 0x0F;
hdr->payload_unit_start_indicator = 0;      // clear flag
hdr->scrambling_control = 2;                // set scrambling bits

printf("\nAfter manipulation:\n");
print_hex(packet, 4);
print_binary(packet, 4);

// Print interpreted fields
printf("\nDecoded fields: \n");
printf("sync_byte = 0x%02X \n", hdr->sync_byte);
printf("payload_unit_start_indicator = %d \n",
       hdr->payload_unit_start_indicator);
printf("PID = %d (0x%04X) \n", hdr->pid, hdr->pid);
printf("scrambling_control = %d \n", hdr->scrambling_control);
printf("continuity_counter = %d \n", hdr->continuity_counter);

return 0;
}