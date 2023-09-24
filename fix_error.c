#include "common.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

uint8_t *packets[5];
int packet_sizes[5];
int set_packets = 0;
int next_slot = 0;

void fix_error(FILE *input) {
    uint8_t *data = malloc(sizeof(uint8_t) * 100000);
        for(int i = 0; i < 5; i++) {
            packets[i] = NULL;
    }
    while(!feof(input)) {
        int data_read = fread(data, 1, 6, stdin);
        int data_length = get_bits(data, 16, 32) + 1;
        data_read += fread(data + 6, 1, data_length, stdin);

        int current_slot = next_slot;
        next_slot = (next_slot + 1) % 5;
        packets[current_slot] = malloc(data_read);
        memcpy(packets[current_slot], data, data_read);
        packet_sizes[current_slot] = data_read;
        set_packets += 1;
        if(set_packets == 5) {
            for(int i = 0; i < 5; i++) {
                int bytes_written = write(fileno(stdout), packets[i], packet_sizes[i]);
                fflush(stdout);
                packets[i] = NULL;
            }
            set_packets = 0;
        }     
    }
}

int main() 
{
    fix_error(stdin);

    return 0;
}