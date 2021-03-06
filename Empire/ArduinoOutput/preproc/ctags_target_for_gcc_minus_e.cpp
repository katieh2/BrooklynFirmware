# 1 "/home/techgarage/BrooklynFirmware/Empire/ISPSlaveTest.ino"
# 2 "/home/techgarage/BrooklynFirmware/Empire/ISPSlaveTest.ino" 2
# 12 "/home/techgarage/BrooklynFirmware/Empire/ISPSlaveTest.ino"
volatile uint8_t buffer[100];
volatile int idx = 0;
volatile int ridx = 0;

uint8_t header;
uint8_t cmd;
uint8_t datalen;
uint8_t data[10];
uint8_t ck1;
uint8_t ck2;



void LED(uint8_t color){
    switch (color){
        case 1:
            digitalWrite(0, 0x0);
            digitalWrite(A2, 0x1);
            digitalWrite(A3, 0x1);
            break;
        case 2:
            digitalWrite(0, 0x1);
            digitalWrite(A2, 0x0);
            digitalWrite(A3, 0x1);
            break;
        case 3:
            digitalWrite(0, 0x1);
            digitalWrite(A2, 0x1);
            digitalWrite(A3, 0x0);
            break;
        case 4:
            digitalWrite(0, 0x1);
            digitalWrite(A2, 0x1);
            digitalWrite(A3, 0x1);
            break;
    }
}


# 50 "/home/techgarage/BrooklynFirmware/Empire/ISPSlaveTest.ino" 3
extern "C" void __vector_17 /* SPI Serial Transfer Complete */ (void) __attribute__ ((signal,used, externally_visible)) ; void __vector_17 /* SPI Serial Transfer Complete */ (void)

# 51 "/home/techgarage/BrooklynFirmware/Empire/ISPSlaveTest.ino"
{
    LED(3);
    buffer[idx] = 
# 53 "/home/techgarage/BrooklynFirmware/Empire/ISPSlaveTest.ino" 3
                 (*(volatile uint8_t *)((0x2E) + 0x20))
# 53 "/home/techgarage/BrooklynFirmware/Empire/ISPSlaveTest.ino"
                     ;
    
# 54 "/home/techgarage/BrooklynFirmware/Empire/ISPSlaveTest.ino" 3
   (*(volatile uint8_t *)((0x2E) + 0x20))
# 54 "/home/techgarage/BrooklynFirmware/Empire/ISPSlaveTest.ino"
       =20;
    if(idx==99){
        idx=0;
    }
    idx+=1;
}

uint8_t readData(){
    while(idx==ridx){}
    uint8_t data = buffer[ridx];
    if(ridx==99){
        ridx=0;
    }
    ridx+=1;
    return(data);
}

bool receivePacket(){
    header = readData();
    if(header==255){
        cmd = readData();
        datalen = readData();
        for(int i=0;i<datalen;i++){
            data[i] = readData();
        }
        ck1 = readData();
        ck2 = readData();
        if(calcChecksum()){
            return true;
        }
    }
    return false;
}

bool calcChecksum(){
    int packetSum = 0;
    packetSum += header;
    packetSum += cmd;
    packetSum += datalen;
    for(int i=0;i<datalen;i++){
        packetSum += data[i];
    }
    if(floor(packetSum / 256) == ck1){
        if(packetSum % 256 == ck2){
            return true;
        }
    }
    return false;
}

void SPISend(uint8_t data){
    
# 105 "/home/techgarage/BrooklynFirmware/Empire/ISPSlaveTest.ino" 3
   (*(volatile uint8_t *)((0x2E) + 0x20)) 
# 105 "/home/techgarage/BrooklynFirmware/Empire/ISPSlaveTest.ino"
        = data;
    readData();
}

void SPISendPacket(uint8_t data[]){
    int packetSum = 0;
    SPISend(data[0]);
    packetSum+=data[0];
    SPISend(data[1]);
    packetSum+=data[1];
    SPISend(data[2]);
    packetSum+=data[2];
    for(int i=0;i<data[2];i++){
        packetSum+=data[3+i];
        SPISend(data[3+i]);
    }
    ck1 = floor(packetSum / 256);
    ck2 = packetSum % 256;
    SPISend(ck1);
    SPISend(ck2);
}

void setup(){
    pinMode(0, 0x1);
    pinMode(A2, 0x1);
    pinMode(A3, 0x1);
    pinMode(MISO,0x1);
    
# 132 "/home/techgarage/BrooklynFirmware/Empire/ISPSlaveTest.ino" 3
   (*(volatile uint8_t *)((0x2C) + 0x20)) 
# 132 "/home/techgarage/BrooklynFirmware/Empire/ISPSlaveTest.ino"
        |= 
# 132 "/home/techgarage/BrooklynFirmware/Empire/ISPSlaveTest.ino" 3
           (1 << (6))
# 132 "/home/techgarage/BrooklynFirmware/Empire/ISPSlaveTest.ino"
                   ;
    
# 133 "/home/techgarage/BrooklynFirmware/Empire/ISPSlaveTest.ino" 3
   (*(volatile uint8_t *)((0x2C) + 0x20)) 
# 133 "/home/techgarage/BrooklynFirmware/Empire/ISPSlaveTest.ino"
        &= ~(
# 133 "/home/techgarage/BrooklynFirmware/Empire/ISPSlaveTest.ino" 3
             (1 << (4))
# 133 "/home/techgarage/BrooklynFirmware/Empire/ISPSlaveTest.ino"
                      ); //Arduino is Slave
    
# 134 "/home/techgarage/BrooklynFirmware/Empire/ISPSlaveTest.ino" 3
   (*(volatile uint8_t *)((0x2E) + 0x20)) 
# 134 "/home/techgarage/BrooklynFirmware/Empire/ISPSlaveTest.ino"
        = 0x67; //test value
    
# 135 "/home/techgarage/BrooklynFirmware/Empire/ISPSlaveTest.ino" 3
   (*(volatile uint8_t *)((0x2C) + 0x20)) 
# 135 "/home/techgarage/BrooklynFirmware/Empire/ISPSlaveTest.ino"
        |= 
# 135 "/home/techgarage/BrooklynFirmware/Empire/ISPSlaveTest.ino" 3
           (1 << (7))
# 135 "/home/techgarage/BrooklynFirmware/Empire/ISPSlaveTest.ino"
                    ; //we not using SPI.attachInterrupt() why?
    
# 136 "/home/techgarage/BrooklynFirmware/Empire/ISPSlaveTest.ino" 3
   __asm__ __volatile__ ("sei" ::: "memory")
# 136 "/home/techgarage/BrooklynFirmware/Empire/ISPSlaveTest.ino"
        ;
    LED(2);
}

void loop(void){
    uint8_t data[] = {255,1,1,72};
    if(receivePacket()){
        LED(3);
        switch(cmd){
            case 0x00 /* GET MOTOR SPEED*/:

                SPISendPacket(data);
                break;
        }
    }
}
