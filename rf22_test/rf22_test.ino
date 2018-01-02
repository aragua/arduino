// rf22_server.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing server
// with the RH_RF22 class. RH_RF22 class does not provide for addressing or
// reliability, so you should only use RH_RF22  if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example rf22_client
// Tested on Duemilanove, Uno with Sparkfun RFM22 wireless shield
// Tested on Flymaple with sparkfun RFM22 wireless shield
// Tested on ChiKit Uno32 with sparkfun RFM22 wireless shield

#include <SPI.h>
#include <RH_RF22.h>

// Singleton instance of the radio driver
RH_RF22 rf22;

void setup() 
{
  Serial.begin(9600);
  if (!rf22.init())
    Serial.println("init failed");
  else
    Serial.println("init succeed");

  // Defaults after init are 434.0MHz, 0.05MHz AFC pull-in, modulation FSK_Rb2_4Fd36

   rf22.setTxPower(RH_RF22_TXPOW_20DBM);
   rf22.setModemConfig(RH_RF22::ModemConfigChoice::FSK_Rb38_4Fd19_6);
}

void loop()
{
  static uint16_t counter = 0;
  if (rf22.available())
  {
    // Should be a message for us now   
    uint8_t buf[RH_RF22_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf22.recv(buf, &len))
    {
      Serial.print("> ");
      Serial.println((char*)buf);
      
      // Send a reply
      uint8_t data[] = "Ack";
      rf22.send(data, sizeof(data));
      rf22.waitPacketSent();
      //Serial.println("Sent a reply for packet");
      Serial.print("Ack ");
      Serial.println(counter);
      counter++;
    }
    else
    {
      Serial.println("recv failed");
    }
  } 
  else
  {
    //Serial.println("RF22 not available");
  }
}

