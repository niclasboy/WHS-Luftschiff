const int blinkCodes[2][2] = {{200,200},{50,950}};


void TaskBlink(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  pinMode(LED_BUILTIN, OUTPUT);
  for (;;) // A Task shall never return or exit.
  {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    vTaskDelay( blinkCodes[currentBlinkCode][0] / portTICK_PERIOD_MS ); // wait for one second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    vTaskDelay( blinkCodes[currentBlinkCode][1] / portTICK_PERIOD_MS ); // wait for one second
  }
}
