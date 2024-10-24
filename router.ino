#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

// Replace with the MAC Address you wanna give to this Arduino board
byte mac[] = { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };


IPAddress ip(192, 168, 3, 2);


IPAddress broadcastIp(192, 168, 3, 255);

// replace with your device's MAC address
byte targetMac[] = { 0xA0, 0xB1, 0xC2, 0xD3, 0xE4, 0xF5 };

EthernetUDP Udp;

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing Ethernet...");

  Ethernet.begin(mac, ip);
  delay(1000); 

  Serial.print("Arduino IP Address: ");
  Serial.println(Ethernet.localIP());

  Udp.begin(9);

  // Send the WOL packets
  for (int i = 0; i < 5; i++) {
    sendWOL(targetMac);
    delay(1000);  // Wait for 1 second between packets
  }

  Serial.println("Entering silent state.");
  while (true) {

  }
}

void sendWOL(byte *macAddress) {
  byte magicPacket[102];
  
  // Construct magic packet
  for (int i = 0; i < 6; i++) {
    magicPacket[i] = 0xFF;
  }
  for (int i = 1; i <= 16; i++) {
    for (int j = 0; j < 6; j++) {
      magicPacket[i * 6 + j] = macAddress[j];
    }
  }


  Serial.print("Magic packet: ");
  for (int i = 0; i < 102; i++) {
    Serial.print(magicPacket[i], HEX);
    Serial.print(" ");
  }
  Serial.println();


  Serial.print("Sending packet to ");
  Serial.print(broadcastIp);
  Serial.println(":9");

  Udp.beginPacket(broadcastIp, 9);
  Udp.write(magicPacket, sizeof(magicPacket));
  Udp.endPacket();
  
  Serial.println("WOL packet sent");
}

void loop() {
}
