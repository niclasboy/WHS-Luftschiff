void failsafeDetection(void *pvParameters)  // This is a task.
{
  (void) pvParameters;  
  for (;;) // A Task shall never return or exit.
  {
    Serial.println(rxGetGoodFrame());
    if(rxGetGoodFrame() >= 10)
    {
      //Genug Daten
      resetGetGoodFrame();
      currentBlinkCode = 1;
      failSafe= false;
    }
    else
    {
      //Fehlerhafte Daten
      resetGetGoodFrame();
      currentBlinkCode = 0;
      failSafe = true;
    }
    
    vTaskDelay(1000 / portTICK_PERIOD_MS ); // wait for one second
  }
}
