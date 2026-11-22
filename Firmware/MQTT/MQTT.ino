
  #include <WiFi.h>
  #include <PubSubClient.h>

  // --- GLOBALS ---
  const char* ssid = "wifiname";
  const char* password = "wifipassword";
  const char* mqtt_server = "laptopip"; // Replace with your laptop's IP address

  /*
 * ============================================================================
 * MQTT & NETWORK TROUBLESHOOTING NOTES (If connection fails with rc=-2)
 * ============================================================================
 * 1. 2.4 GHz Band Only:
 *    The ESP32 only supports 2.4 GHz Wi-Fi. Ensure the router/MiFi is not set 
 *    to 5 GHz only.
 *
 * 2. Windows Network Profile:
 *    New Wi-Fi networks default to "Public" on Windows, which blocks incoming 
 *    MQTT traffic on port 1883. Fix: Settings > Network & Internet > Wi-Fi > 
 *    [Your Network] -> switch Network Profile to "Private".
 *
 * 3. AP / Client Isolation:
 *    Some MiFis/routers enable "AP Isolation" or "Privacy Separator" by 
 *    default, blocking devices on the same Wi-Fi from pinging each other. 
 *    If Wi-Fi connects but MQTT fails, disable AP Isolation in the router 
 *    admin page.
 *
 * TIP - Static IP:
 *    Assign a Static IP (DHCP Reservation) to your laptop in the router/MiFi 
 *    settings so the broker IP doesn't change after a reboot.
 * ============================================================================
 */

  WiFiClient espClient;         // Creates the raw network connection
  PubSubClient client(espClient); // Wraps the network connection in MQTT protocol

  unsigned long lastMsg = 0;    // Variable to track time for our millis() timer

  void setup() {
    Serial.begin(115200);
    delay(4000);
    // 1. Standard WiFi Connection Loop
    Serial.print("Connecting to WiFi");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("\nWiFi connected!");

    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
  }

  void loop() {
    // This must be called every loop to keep the MQTT connection alive
    client.loop(); 

    // 3. THE TWO-SECOND TIMER
    unsigned long now = millis();
    
    if ((now - lastMsg) > 2000) {
      // Reset the timer
      lastMsg = now;
      
      // Publish the message
      client.publish("amr/telemetry/status", "ESP32 alive");
    }
    if(!client.connected()){
      reconnect();  
    }
  }

  void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
      Serial.print("Attempting MQTT connection...");
      // Attempt to connect with a client ID "AMR_Client"
      if (client.connect("AMR_Client")) {
        Serial.println("connected to broker!");
      } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        delay(5000);
      }
      client.subscribe("amr/command");
    }
  }



void callback(char* topic, byte* payload, unsigned int length) {
  // 1. The library hands us raw bytes. We must convert them into a standard character array.
  char message[length + 1];
  for (int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0'; // Add the null-terminator to make it a valid C-string

  Serial.print("Message arrived on topic: ");
  Serial.println(topic);

  // --- JAY'S PARSING LOGIC GOES HERE ---
  // You now have a character array called 'message' (e.g., "START" or "destination|set|bxb5").
  // 1. Use strtok() to extract the first command word.

  char* command_word = strtok(message, "|");
  char val3[] = "START";
  if (strcmp(message, val3) == 0) {
  Serial.println("Navigation starting...");
  }
}



