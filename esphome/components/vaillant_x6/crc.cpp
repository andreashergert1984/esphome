#include <stdio.h>
#include <string>
#include <vector>

std::vector<uint8_t> command;

uint8_t calccrc(std::vector<uint8_t> command) {
    uint8_t checksum = 0;
        for (auto b: command) {
            if (checksum & 0x80) {
                checksum = (checksum << 1 | 1) & 0xFF;
                checksum = checksum ^ 0x18;

            } else {
                checksum = checksum << 1;   
            }
            checksum = checksum ^ b;
        }
    return checksum;
}

int main() {
    printf("crc testing program...\n");
    uint8_t pcommand = 0x39;
    command.push_back(0x07);   
    command.push_back(0x00);   
    command.push_back(0x00);   
    command.push_back(0x00);   
    command.push_back(pcommand);   
    command.push_back(0x00); //exp length
    command.push_back(calccrc(command));

    for (int i=0; i<command.size(); i++) {
        printf("0x%02x ",command.at(i));
    }
    
    
    
}