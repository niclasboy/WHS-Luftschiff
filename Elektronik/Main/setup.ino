void TaskBlink( void *pvParameters );
void rxProcessing( void *pvParameters );
void failsafeDetection( void *pvParameters );


void setup()
{
  xTaskCreate(TaskBlink, "Alive LED", 128, NULL, 0, NULL); //Kontrolle der Status LED
  xTaskCreate(rxProcessing, "Process Incoming Data From RX", 1024, NULL, 0, NULL); //Bearbeitung des Ringbuffers, wenn neu Daten anliegen
  xTaskCreate(failsafeDetection, "Failsafe Detection", 512, NULL, 0, NULL); // Jede Sekunde Überprüfen, ob genug DAten vom Reciever ankommen

  Serial.begin(250000);
  Serial.println("Start");
  delay(50);
  UBRR3L = 9; // UART3 - 8E2 - 100000 Baud - Interrupt füllt Ringbuffer
  UCSR3B |= (1 << RXCIE3) | (1 << RXEN3);
  UCSR3C |= (1 << UPM31) | (1 << USBS3) | (1 << UCSZ31) | (1 << UCSZ30);
  interrupts();
}

void loop() {
  // put your main code here, to run repeatedly:

}
