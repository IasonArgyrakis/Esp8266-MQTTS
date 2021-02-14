

#define WIFI_SSID "SSID_HERE"
#define WIFI_PASSWORD "WIFI_PASSWORD"

#define MQTT_USER  "MQTT_USER_Name"
#define MQTT_PASSWORD "PASS"

// Test Mosquitto server, various ports available
// 1883 is unencrypted, 8883 is encrypted no certificate required
// 8884 : MQTT, encrypted, client certificate required
// https://test.mosquitto.org/
#define MQTT_SERVER "SERVER_IP"
#define MQTT_PORT 8883



// Fingerprint of the MQTT broker CA - for Letsencrypt certificates this is chain.pem I think
// openssl x509 -in  chain.pem -sha1 -noout -fingerprint
const char* fingerprint = "5C:FB:FB:9B:3B:D5:05:5B:FB:40:FB:FB:FB:98:FB:20:FB:4A:FB:D5";
