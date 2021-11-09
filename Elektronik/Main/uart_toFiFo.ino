#define SBUS_FAILSAFE_INACTIVE 0
#define SBUS_FAILSAFE_ACTIVE   1
#define SBUS_STARTBYTE         0x0f
#define SBUS_ENDBYTE           0x00

#define minIn 0
#define maxIn 2047
#define minOut 0
#define maxOut 255


static byte buffer[25];
static byte rawBuffer[25];
static byte stateCounter = 0;
static byte bufferValid[25];
static byte feedState = 0;
long _goodFrames;
static byte buffer_index = 0;

ISR(TIMER5_OVF_vect) //Wenn Timer ausgelößt wurde, wurde für 10ms nicht gesendet. Syncronisierung!
{
  TCNT5 = 0;
  feedState = 0;
}

ISR(USART3_RX_vect)
{
  TCNT5 = 45536;
  byte dataIn = UDR3;
  switch (feedState)
  {
    case 0:
      if (dataIn = 0x0f) //Data OK
      {
        stateCounter = 0;
        feedState = 1;
        rawBuffer[stateCounter] = dataIn;
        rawBuffer[24] = 0xff;
        stateCounter++;
      }
      break;
    case 1:
      rawBuffer[stateCounter] = dataIn;
      stateCounter++;
      if (stateCounter == 25) //last byte recieved
      {
        stateCounter = 0;
        feedState = 0;
        if (rawBuffer[24] == 0x00) //Packet OK
        {
          memcpy(rawBuffer, buffer, 25);
          newData = 1;
        }
      }
      break;
  }
}



long rxGetGoodFrame()
{
  return _goodFrames;
}
void resetGetGoodFrame()
{
  _goodFrames = 0;
}

void rxProcessing(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  for (;;) // A Task shall never return or exit.
  {
    if (newData == 1)
    {
      newData = 0;
      _goodFrames++;
      rxDataIn[0]  =  map(((buffer[1] | buffer[2] << 8) & 0x07FF), minIn, maxIn, minOut, maxOut);
      rxDataIn[1]  = map(((buffer[2] >> 3 | buffer[3] << 5) & 0x07FF), minIn, maxIn, minOut, maxOut);
      rxDataIn[2]  = map(((buffer[3] >> 6 | buffer[4] << 2 | buffer[5] << 10) & 0x07FF), minIn, maxIn, minOut, maxOut);
      rxDataIn[3]  = map(((buffer[5] >> 1 | buffer[6] << 7) & 0x07FF), minIn, maxIn, minOut, maxOut);
      rxDataIn[4]  = map(((buffer[6] >> 4 | buffer[7] << 4) & 0x07FF), minIn, maxIn, minOut, maxOut);
      rxDataIn[5]  = map(((buffer[7] >> 7 | buffer[8] << 1 | buffer[9] << 9) & 0x07FF), minIn, maxIn, minOut, maxOut);
      rxDataIn[6]  = map(((buffer[9] >> 2 | buffer[10] << 6) & 0x07FF), minIn, maxIn, minOut, maxOut);
      rxDataIn[7]  = map(((buffer[10] >> 5 | buffer[11] << 3) & 0x07FF), minIn, maxIn, minOut, maxOut);
      rxDataIn[8]  = map(((buffer[12]   | buffer[13] << 8) & 0x07FF), minIn, maxIn, minOut, maxOut);
      rxDataIn[9]  = map(((buffer[13] >> 3 | buffer[14] << 5) & 0x07FF), minIn, maxIn, minOut, maxOut);
      rxDataIn[10] = map(((buffer[14] >> 6 | buffer[15] << 2 | buffer[16] << 10) & 0x07FF), minIn, maxIn, minOut, maxOut);
      rxDataIn[11] = map(((buffer[16] >> 1 | buffer[17] << 7) & 0x07FF), minIn, maxIn, minOut, maxOut);
      rxDataIn[12] = map(((buffer[17] >> 4 | buffer[18] << 4) & 0x07FF), minIn, maxIn, minOut, maxOut);
      rxDataIn[13] = map(((buffer[18] >> 7 | buffer[19] << 1 | buffer[20] << 9) & 0x07FF), minIn, maxIn, minOut, maxOut);
      rxDataIn[14] = map(((buffer[20] >> 2 | buffer[21] << 6) & 0x07FF), minIn, maxIn, minOut, maxOut);
      rxDataIn[15] = map(((buffer[21] >> 5 | buffer[22] << 3) & 0x07FF), minIn, maxIn, minOut, maxOut);

      ((buffer[23])      & 0x0001) ? rxDataIn[16] = 255 : rxDataIn[16] = 0;
      ((buffer[23] >> 1) & 0x0001) ? rxDataIn[17] = 255 : rxDataIn[17] = 0;

    }
    vTaskDelay(10 / portTICK_PERIOD_MS ); // wait for one second

  }
}


//      if (buffer_index == 0 && currentData != SBUS_STARTBYTE) {
//        //incorrect start byte, out of sync
//        _decoderErrorFrames++;
//        continue;
//      }
//      buffer[buffer_index++] = currentData;
//      if (buffer_index == 25) {
//        buffer_index = 0;
//        if (buffer[24] != SBUS_ENDBYTE) {
//          //incorrect end byte, out of sync
//          _decoderErrorFrames++;
//          continue;
//        }
//        _goodFrames++;
//        rxDataIn[0]  =  map(((buffer[1]| buffer[2] << 8)& 0x07FF),minIn,maxIn,minOut,maxOut);
//        rxDataIn[1]  = map(((buffer[2] >> 3 | buffer[3] << 5)&0x07FF),minIn,maxIn,minOut,maxOut);
//        rxDataIn[2]  = map(((buffer[3] >> 6 | buffer[4] << 2 | buffer[5] << 10)&0x07FF),minIn,maxIn,minOut,maxOut);
//        rxDataIn[3]  = map(((buffer[5] >> 1 | buffer[6] << 7)&0x07FF),minIn,maxIn,minOut,maxOut);
//        rxDataIn[4]  = map(((buffer[6] >> 4 | buffer[7] << 4)&0x07FF),minIn,maxIn,minOut,maxOut);
//        rxDataIn[5]  = map(((buffer[7] >> 7 | buffer[8] << 1 | buffer[9] << 9)&0x07FF),minIn,maxIn,minOut,maxOut);
//        rxDataIn[6]  = map(((buffer[9] >> 2 | buffer[10] << 6)&0x07FF),minIn,maxIn,minOut,maxOut);
//        rxDataIn[7]  = map(((buffer[10] >> 5 | buffer[11] << 3)&0x07FF),minIn,maxIn,minOut,maxOut);
//        rxDataIn[8]  = map(((buffer[12]   | buffer[13] << 8)&0x07FF),minIn,maxIn,minOut,maxOut);
//        rxDataIn[9]  = map(((buffer[13] >> 3 | buffer[14] << 5)&0x07FF),minIn,maxIn,minOut,maxOut);
//        rxDataIn[10] = map(((buffer[14] >> 6 | buffer[15] << 2 | buffer[16] << 10)&0x07FF),minIn,maxIn,minOut,maxOut);
//        rxDataIn[11] = map(((buffer[16] >> 1 | buffer[17] << 7)&0x07FF),minIn,maxIn,minOut,maxOut);
//        rxDataIn[12] = map(((buffer[17] >> 4 | buffer[18] << 4)&0x07FF),minIn,maxIn,minOut,maxOut);
//        rxDataIn[13] = map(((buffer[18] >> 7 | buffer[19] << 1 | buffer[20] << 9)& 0x07FF),minIn,maxIn,minOut,maxOut);
//        rxDataIn[14] = map(((buffer[20] >> 2 | buffer[21] << 6)&0x07FF),minIn,maxIn,minOut,maxOut);
//        rxDataIn[15] = map(((buffer[21] >> 5 | buffer[22] << 3)&0x07FF),minIn,maxIn,minOut,maxOut);
//
//        ((buffer[23])      & 0x0001) ? rxDataIn[16] = 255 : rxDataIn[16] = 0;
//        ((buffer[23] >> 1) & 0x0001) ? rxDataIn[17] = 255 : rxDataIn[17] = 0;
//
//        if ((buffer[23] >> 3) & 0x0001) {
//          _failsafe = SBUS_FAILSAFE_ACTIVE;
//        } else {
//          _failsafe = SBUS_FAILSAFE_INACTIVE;
//        }
//
//        if ((buffer[23] >> 2) & 0x0001) {
//          _lostFrames++;
//        }
//      }
