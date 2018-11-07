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

bool _update = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  attachInterrupt(BUTTON, blink, RISING);
}

void loop() {
  // put your main code here, to run repeatedly:
  debounce_check();
  if (_update == true) {
    switch (operating_mode) {
      case 0:
        SetBrightness(0);
        break;
      case 1:
        SetBrightness(PWM_MAX / 3);
        break;
      case 2:
        SetBrightness(PWM_MAX * 2 / 3);
        break;
      case 3:
        SetBrightness(PWM_MAX);
        break;
      case 4:
        Flash_Light();
        break;
    }
    _update = false;
  }
}

void blink() {
  button_state += 1;
}

void debounce_check() {
  if (button_state != old_button_state) {
    if (millis() - debounce_time > debounce_delay) {
      operating_mode += 1;
      operating_mode %= 5;
      _update = true;
      debounce_time = millis();
      old_button_state = button_state;
    }
  }
}

void SetBrightness(int intensity) {
  analogWrite(LED, intensity);
}

void Flash_Light() {
  for (int i = 0; i <= PWM_MAX / 2; i++) {
    SetBrightness(i);
    delay(2);
  }
  for (int i = PWM_MAX / 2 ; i >= 0; i--) {
    SetBrightness(i);
    delay(2);
  }
}

