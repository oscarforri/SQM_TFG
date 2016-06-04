void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}


void save_data(){
    addr = last_addr;
    EEPROM.write(addr, monthDay);
    next_addres();
    EEPROM.write(addr, month);
    next_addres();
    EEPROM.write(addr, year);
    next_addres();
    EEPROM.write(addr, hour);
    next_addres();
    EEPROM.write(addr, minute);
    next_addres();
    save_floatdata(t);
    next_addres();
    save_floatdata(h);
    next_addres();
    save_floatdata(t);
    next_addres();
    last_addr = addr;
}

void next_addres(){
 addr ++;
    if (addr=1024){
      addr=3;
    }
}

void save_floatdata(val){
 a= val * 100;
 byte lowbyte;
 byte highbyte;
 lowbyte = lowByte(a);
 highbyte = highByte(a): 
 EEPROM.write(addr, highbyte);
 addr ++;
 EEPROM.write(addr, lowbyte);
}

