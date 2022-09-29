#include "Arduino.h"

int siz = 10;
uint8_t *array_int;

struct PIR {
    const uint8_t PIN;
    bool enbl;
};

//interrupt initialization 
unsigned long button_time = 0;  
unsigned long last_button_time = 0; 

PIR enblpir = {12, false}; //object 
PIR disbl = {13, false}; 

void IRAM_ATTR isr() {
    button_time = millis();
    if (button_time - last_button_time > 250)
    {
    enblpir.enbl = true;
    last_button_time = button_time;
    }
}

void IRAM_ATTR isr_dis() {
    button_time = millis();
    if (button_time - last_button_time > 250)
    {
    disbl.enbl = true;
    last_button_time = button_time;
    }
}

void interrupt (int siz){
    int count=0;

    int available_PSRAM_size = ESP.getFreePsram();
    Serial.println((String)"PSRAM Size available (bytes): " + available_PSRAM_size);
    
    array_int = (uint8_t *) ps_malloc(100); //Create an integer array of n_elements
    
    for (int i=0; i<100; i++ ){
          array_int [i]= {0x1};
    }
    Serial.println((String)"PSRAM Size available (bytes): " + ESP.getFreePsram());

    for (int i=0; i<=siz; i++ ){
          Serial.println (String(array_int[i]));
          count++;
          Serial.println ("count: "+ String(count));  
    }

      int available_PSRAM_size_after = ESP.getFreePsram();
      Serial.println((String)"PSRAM Size available (bytes): " + available_PSRAM_size_after); // Free memory space has decreased
      int array_size = available_PSRAM_size - available_PSRAM_size_after;
      Serial.println((String)"Array size in PSRAM in bytes: " + array_size);
    
}



void inter_disable(){
    //uint8_t *array_int;
    //Delete array
    free(array_int); //The allocated memory is freed.
    Serial.println("PSRAM freed ");
    Serial.println((String)"PSRAM Size available (bytes): " +ESP.getFreePsram());
  
}


void setup(){
    Serial.begin(115200);
    //Init
    if(psramInit()){
    Serial.println("\nPSRAM is correctly initialized");
    }else{
    Serial.println("PSRAM not available");
    }


    //enable inturrpt 
    pinMode(enblpir.PIN,INPUT_PULLUP);
    pinMode(disbl.PIN,INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(enblpir.PIN), isr, RISING);
    attachInterrupt(digitalPinToInterrupt(disbl.PIN), isr_dis, RISING);
}

void loop() {
//65000
//int arr[siz];
if (enblpir.enbl) {
    enblpir.enbl = false;
    interrupt(siz);//= *arr;
}

if (disbl.enbl) {
    disbl.enbl = false;
    inter_disable();
}

}
