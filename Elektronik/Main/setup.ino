void TaskBlink( void *pvParameters );
void rxProcessing( void *pvParameters );
void failsafeDetection( void *pvParameters );

void setup()
{
  Serial.begin(250000);
  Serial.println("Start");
  delay(50);
  UBRR3L = 9; // UART3 - 8E2 - 100000 Baud - Interrupt füllt Ringbuffer
  UCSR3B |= (1 << RXCIE3) | (1 << RXEN3);
  UCSR3C |= (1 << UPM31) | (1 << USBS3) | (1 << UCSZ31) | (1 << UCSZ30);

  TCCR5A = 0;
  TCCR5B = 0;
  TCCR5C = 0;
  
  TCNT5 = 45536;
  TCCR5B |= (1<<CS51);
  TIMSK5 |= (1<<TOIE5);

  setupPwmMotor();
  setupFlugSteuerung();
  mainServo[0].write(180);   
  interrupts();
  
  delay(50);
  xTaskCreate(TaskBlink, "LED Control", 64, NULL, 0, NULL); //Kontrolle der Status LED
  xTaskCreate(rxProcessing, "RX", 128, NULL, 0, NULL); //Bearbeitung des Ringbuffers, wenn neu Daten anliegen
  xTaskCreate(failsafeDetection, "FS Detec.", 128, NULL, 0, NULL); // Jede Sekunde Überprüfen, ob genug DAten vom Reciever ankommen
  //xTaskCreate(FlugSteuerung, "Flight Controll", 256, NULL, 1, NULL); // 

  
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
