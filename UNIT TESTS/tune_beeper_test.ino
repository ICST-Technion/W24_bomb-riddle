<<<<<<< HEAD
// Change this depending on where you put your piezo buzzer

const int TONE_OUTPUT_PIN = 27;

// The ESP32 has 16 channels which can generate 16 independent waveforms
// We'll just choose PWM channel 0 here
const int TONE_PWM_CHANNEL = 0; 

void setup() {
  // ledcAttachPin(uint8_t pin, uint8_t channel);
  ledcAttachPin(TONE_OUTPUT_PIN, TONE_PWM_CHANNEL);
}

void loop() {
  // Plays the middle C scale
  ledcWriteNote(TONE_PWM_CHANNEL, NOTE_A, 4);
  delay(80);
  ledcWriteNote(TONE_PWM_CHANNEL, NOTE_F, 4);
  delay(80);
  ledcWriteNote(TONE_PWM_CHANNEL, NOTE_E, 4);
  delay(130);
  ledcWrite(TONE_PWM_CHANNEL, 0); // Stop tone 
  delay(2000);
  ledcWriteNote(TONE_PWM_CHANNEL, NOTE_Cs, 4);
  delay(300);

=======
// Change this depending on where you put your piezo buzzer

const int TONE_OUTPUT_PIN = 27;

// The ESP32 has 16 channels which can generate 16 independent waveforms
// We'll just choose PWM channel 0 here
const int TONE_PWM_CHANNEL = 0; 

void setup() {
  // ledcAttachPin(uint8_t pin, uint8_t channel);
  ledcAttachPin(TONE_OUTPUT_PIN, TONE_PWM_CHANNEL);
}

void loop() {
  // Plays the middle C scale
  ledcWriteNote(TONE_PWM_CHANNEL, NOTE_A, 4);
  delay(80);
  ledcWriteNote(TONE_PWM_CHANNEL, NOTE_F, 4);
  delay(80);
  ledcWriteNote(TONE_PWM_CHANNEL, NOTE_E, 4);
  delay(130);
  ledcWrite(TONE_PWM_CHANNEL, 0); // Stop tone 
  delay(2000);
  ledcWriteNote(TONE_PWM_CHANNEL, NOTE_Cs, 4);
  delay(300);

>>>>>>> 21a185f (Before final presentation)
}