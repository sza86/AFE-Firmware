#include "AFE-EEPROM.h"                                                                                                                                       

AFEEEPROM::AFEEEPROM (){
    EEPROM.begin(EEPROM_SIZE);
}


void AFEEEPROM::write(uint16_t address, uint16_t size, String in) {
    clear(address, size);
    for (uint16_t i = 0; i < in.length(); ++i) {
        EEPROM.write(address + i, in[i]);
    }
    EEPROM.commit();
}

void AFEEEPROM::write(uint16_t address, uint16_t size, uint8_t in) {
  write(address,size,String((char)in));
}

void AFEEEPROM::write(uint16_t address, boolean in) {
    write(address,1,String(in?1:0));
}

void AFEEEPROM::write(uint16_t address, uint16_t size, long in) {
  write(address,size,String(in));
}

void AFEEEPROM::write(uint16_t address, uint16_t size, float in) {
  write(address,size,String(in));
}


String AFEEEPROM::read(uint16_t address, uint16_t size) {
    String _return;
    for (uint16_t i = address; i < address + size; ++i) {
        if (EEPROM.read(i) != 255) {
            _return += char(EEPROM.read(i));
        }
    }
    return _return;
}

boolean AFEEEPROM::read(uint16_t address) {
    return (read(address, 1).toInt() == 1 ? true : false);
}

char AFEEEPROM::readByte(uint16_t address) {
    char temp;
    temp = read(address, 1)[0];
    return temp;
}

void AFEEEPROM::clear(uint16_t address, uint16_t size) {
    for (uint16_t i = 0; i < size; ++i) {
        EEPROM.write(i + address, 255);
    }
    EEPROM.commit();
}

void AFEEEPROM::erase() {
    clear(0, EEPROM_SIZE);
}
