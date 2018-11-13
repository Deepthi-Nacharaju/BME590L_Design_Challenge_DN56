#include <avr/io.h>
#include <avr/interrupt.h>

#define LED 1
#define BUTTON 2
#define BLINK_RATE 2; //Hz

int PWM_MAX = 255;
int button_state = 0;
int old_button_state = 0;
// operating_mode: 0 - off, 1 - bright, 2 - midbright, 3 - dim, 4 - 2Hz
int operating_mode = 0;
int debounce_delay = 250;
int debounce_time = 0;
bool pin_mode = LOW;
bool _update = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  pinMode(BUTTON, INPUT);
  GIMSK = 0b00100000;
  PCMSK = 0b0000100;
  sei();
}

void loop() {
  // put your main code here, to run repeatedly:
  debounce_check();
  Update_LED();
}

ISR(PCINT0_vect) {
  button_state += 1;
}

void debounce_check() {
  if (button_state - old_button_state > 1) {
    if (millis() - debounce_time > debounce_delay) {
      operating_mode += 1;
      operating_mode %= 5;
      _update = true;
      debounce_time = millis();
      old_button_state = button_state;
    }
  }
  //button_state%1000;
}

void SetBrightness(int intensity) {
  analogWrite(LED, intensity);
}

void Flash_Light() {
  for (int i = 0; i <= PWM_MAX / 2; i++) {
    SetBrightness(i);
    delay(1);
  }
  for (int i = PWM_MAX / 2 ; i >= 0; i--) {
    SetBrightness(i);
    delay(1);
  }
}

void Update_LED(){
    if (_update == true) {
    switch (operating_mode) {
      case 0:
        SetBrightness(0);
        break;
      case 1:
        SetBrightness(1);
        break;
      case 2:
        SetBrightness(10);
        break;
      case 3:
        SetBrightness(PWM_MAX);
        break;
      case 4:
        while(operating_mode == 4){
        Flash_Light();
        debounce_check();
        }
        break;
    }
    //delay(500);
    _update = false;
  }
}

void two_state_LED(){
   if ((_update == true) && (pin_mode == HIGH)){
    pin_mode = LOW;
    digitalWrite(LED, pin_mode);
    _update == false;
    delay(500);
  }
  else if ((_update == true) && (pin_mode == LOW)){
    pin_mode = HIGH;
    digitalWrite(LED, pin_mode);
    _update == false;
    delay(500);
  }
}

