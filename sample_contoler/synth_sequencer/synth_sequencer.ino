#include <SPI.h>
#include "parameter.h"

typedef union {
  uint16_t raw;
  struct {
    unsigned int high : 8;
    unsigned int low : 8;
  } split;
} transmit16;

uint8_t transBuff[4];
transmit16 parameter, value;

void setup() {
  SPI.begin();
  digitalWrite(SS, HIGH);

  //init synth
  for (uint16_t i = 0; i <= S10_TRI; i++) {
    parameter.raw = i;
    value.raw = 0;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);
  }

  //cuttoff
  parameter.raw = LPF_CUTOFF;
  value.raw = 34579;
  struct2buff(parameter, value, transBuff);
  digitalWrite(SS, LOW);
  SPI.transfer(transBuff, 4);
  digitalWrite(SS, HIGH);

  //Q factor
  parameter.raw = LPF_Q;
  value.raw = 70;
  struct2buff(parameter, value, transBuff);
  digitalWrite(SS, LOW);
  SPI.transfer(transBuff, 4);
  digitalWrite(SS, HIGH);

  //S1 as melody
  parameter.raw = S1_SAW;
  value.raw = 20;
  struct2buff(parameter, value, transBuff);
  digitalWrite(SS, LOW);
  SPI.transfer(transBuff, 4);
  digitalWrite(SS, HIGH);

  //S2 as low
  parameter.raw = S2_SQU;
  value.raw = 20;
  struct2buff(parameter, value, transBuff);
  digitalWrite(SS, LOW);
  SPI.transfer(transBuff, 4);
  digitalWrite(SS, HIGH);
  
  //S3 as mid
  parameter.raw = S3_SQU;
  value.raw = 20;
  struct2buff(parameter, value, transBuff);
  digitalWrite(SS, LOW);
  SPI.transfer(transBuff, 4);
  digitalWrite(SS, HIGH);

  uint8_t lowPart = 0;
  uint8_t midPart = 0;

  for (uint8_t i = 0; i < 3; i++) {
    if (i > 0) {
      lowPart = 1;
    }
    if(i > 1){
      midPart = 1;
    }
    //wait to start
    delay(500);

    //start
    //1
    parameter.raw = S1_FREQ;
    value.raw = 14080;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S1_ENABLE;
    value.raw = 1;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    delay(150);

    //2
    parameter.raw = S1_FREQ;
    value.raw = 15360;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S2_FREQ;
    value.raw = 12288;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S2_ENABLE;
    value.raw = lowPart;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    delay(150);

    //3
    parameter.raw = S1_FREQ;
    value.raw = 16384;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S2_FREQ;
    value.raw = 13312;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    delay(150);

    //4
    parameter.raw = S1_FREQ;
    value.raw = 17152;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S2_FREQ;
    value.raw = 12288;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S3_FREQ;
    value.raw = 15360;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S3_ENABLE;
    value.raw = midPart;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    delay(150);

    //5
    parameter.raw = S1_FREQ;
    value.raw = 18432;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S2_FREQ;
    value.raw = 13312;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S3_FREQ;
    value.raw = 16384;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    delay(150);

    //6
    parameter.raw = S1_FREQ;
    value.raw = 19456;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S2_FREQ;
    value.raw = 15360;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S3_FREQ;
    value.raw = 17152;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    delay(150);

    //7
    parameter.raw = S1_FREQ;
    value.raw = 20224;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S2_FREQ;
    value.raw = 16384;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S3_FREQ;
    value.raw = 18432;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    delay(400);

    //8
    parameter.raw = S1_FREQ;
    value.raw = 19456;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S2_FREQ;
    value.raw = 15360;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S3_FREQ;
    value.raw = 17152;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    delay(400);
    
    parameter.raw = S2_ENABLE;
    value.raw = 0;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);
    
    parameter.raw = S3_ENABLE;
    value.raw = 0;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    //---------------2
    //1
    parameter.raw = S1_FREQ;
    value.raw = 14336;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    delay(150);

    //2
    parameter.raw = S1_FREQ;
    value.raw = 15360;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S2_FREQ;
    value.raw = 12288;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S2_ENABLE;
    value.raw = lowPart;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    delay(150);

    //3
    parameter.raw = S1_FREQ;
    value.raw = 16128;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S2_FREQ;
    value.raw = 13056;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    delay(150);

    //4
    parameter.raw = S1_FREQ;
    value.raw = 17408;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S2_FREQ;
    value.raw = 12288;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S3_FREQ;
    value.raw = 15360;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S3_ENABLE;
    value.raw = midPart;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    delay(150);

    //5
    parameter.raw = S1_FREQ;
    value.raw = 18432;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S2_FREQ;
    value.raw = 13056;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S3_FREQ;
    value.raw = 16128;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    delay(150);

    //6
    parameter.raw = S1_FREQ;
    value.raw = 19200;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S2_FREQ;
    value.raw = 15360;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S3_FREQ;
    value.raw = 17408;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    delay(150);

    //7
    parameter.raw = S1_FREQ;
    value.raw = 20480;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S2_FREQ;
    value.raw = 16128;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S3_FREQ;
    value.raw = 18432;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    delay(400);

    parameter.raw = S3_ENABLE;
    value.raw = 0;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    //8
    parameter.raw = S1_FREQ;
    value.raw = 19200;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S2_FREQ;
    value.raw = 15360;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S3_FREQ;
    value.raw = 17408;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    delay(400);

    //---------------3
    //1
    parameter.raw = S1_FREQ;
    value.raw = 14848;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    delay(150);

    //2
    parameter.raw = S1_FREQ;
    value.raw = 15872;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S2_FREQ;
    value.raw = 12800;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S2_ENABLE;
    value.raw = lowPart;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    delay(150);

    //3
    parameter.raw = S1_FREQ;
    value.raw = 16640;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S2_FREQ;
    value.raw = 13568;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    delay(150);

    //4
    parameter.raw = S1_FREQ;
    value.raw = 17920;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S2_FREQ;
    value.raw = 12800;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S3_FREQ;
    value.raw = 15872;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S3_ENABLE;
    value.raw = midPart;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    delay(150);

    //5
    parameter.raw = S1_FREQ;
    value.raw = 18944;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S2_FREQ;
    value.raw = 13568;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S3_FREQ;
    value.raw = 16640;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    delay(150);

    //6
    parameter.raw = S1_FREQ;
    value.raw = 19712;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S2_FREQ;
    value.raw = 15872;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S3_FREQ;
    value.raw = 17920;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    delay(150);

    //7
    parameter.raw = S1_FREQ;
    value.raw = 20992;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S2_FREQ;
    value.raw = 16640;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S3_FREQ;
    value.raw = 18944;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    delay(400);

    //rest
    parameter.raw = S1_ENABLE;
    value.raw = 0;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S2_ENABLE;
    value.raw = 0;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S3_ENABLE;
    value.raw = 0;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);
    delay(26);

    //Repeated
    parameter.raw = S1_FREQ;
    value.raw = 20992;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S2_FREQ;
    value.raw = 15872;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S3_FREQ;
    value.raw = 18944;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S1_ENABLE;
    value.raw = 1;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S2_ENABLE;
    value.raw = lowPart;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S3_ENABLE;
    value.raw = midPart;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    delay(150);
    //rest
    parameter.raw = S1_ENABLE;
    value.raw = 0;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S2_ENABLE;
    value.raw = 0;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S3_ENABLE;
    value.raw = 0;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    delay(26);

    //Repeated
    parameter.raw = S1_FREQ;
    value.raw = 20992;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S2_FREQ;
    value.raw = 15872;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S3_FREQ;
    value.raw = 18944;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S1_ENABLE;
    value.raw = 1;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S2_ENABLE;
    value.raw = lowPart;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S3_ENABLE;
    value.raw = midPart;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    delay(150);
    //rest
    parameter.raw = S1_ENABLE;
    value.raw = 0;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S2_ENABLE;
    value.raw = 0;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S3_ENABLE;
    value.raw = 0;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);
    
    delay(26);

    //Repeated
    parameter.raw = S1_FREQ;
    value.raw = 20992;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S2_FREQ;
    value.raw = 15872;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S3_FREQ;
    value.raw = 18944;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S1_ENABLE;
    value.raw = 1;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S2_ENABLE;
    value.raw = lowPart;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S3_ENABLE;
    value.raw = midPart;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    delay(150);

    //last
    parameter.raw = S1_FREQ;
    value.raw = 21504;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S2_FREQ;
    value.raw = 15360;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S3_FREQ;
    value.raw = 19456;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    delay(600);

    //done
    parameter.raw = S1_ENABLE;
    value.raw = 0;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S2_ENABLE;
    value.raw = 0;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);

    parameter.raw = S3_ENABLE;
    value.raw = 0;
    struct2buff(parameter, value, transBuff);
    digitalWrite(SS, LOW);
    SPI.transfer(transBuff, 4);
    digitalWrite(SS, HIGH);
  }
}

void loop() {


}

void struct2buff(transmit16 parameter, transmit16 value, uint8_t *buff) {
  buff[0] = parameter.split.low;
  buff[1] = parameter.split.high;
  buff[2] = value.split.low;
  buff[3] = value.split.high;
}
