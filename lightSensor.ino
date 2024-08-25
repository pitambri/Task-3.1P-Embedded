#include <Wire.h>
#include <BH1750.h>
#include <WiFiNINA.h>
#include <EMailSender.h>

// Wi-Fi credentials
const char ssid[] = "iPhone";
const char pass[] = "Pitam@2217";

// SMTP Server and login credentials
#define SMTP_SERVER "smtp.gmail.com"
#define SMTP_PORT 587

// Sender email credentials
#define SENDER_EMAIL "pitambri.1905@gmail.com"
#define SENDER_PASSWORD "ogob ikyb mmzq arwg"

// Sender name and email
#define SENDER_NAME "Pitambri"

// Recipient's email address
#define RECIPIENT_EMAIL "pitambri4836.be23@chitkara.edu.in"

BH1750 lightMeter(0x6A);
EMailSender emailSender(SENDER_EMAIL, SENDER_PASSWORD, SENDER_EMAIL, SENDER_NAME);

const int sunlightThreshold = 1000;  // Lux value to determine sunlight
bool sunlightActive = false;
float lastLux = 0.0;  // Variable to store the last recorded Lux value

void setup() {
  Serial.begin(9600);

  // Connect to Wi-Fi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to Wi-Fi");

  // Initialize the light sensor
  Wire.begin();
  lightMeter.begin();
}

void loop() {
  // Read light intensity
  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");

  // Check if sunlight is detected
  if (lux > sunlightThreshold && !sunlightActive) {
    sunlightActive = true;
    lastLux = lux;
    sendEmail("Sunlight Detected!", "Your terrarium is now receiving sunlight.\nLast recorded light intensity: " + String(lastLux) + " lx");
  } 
  // Check if sunlight has stopped
  else if (lux <= sunlightThreshold && sunlightActive) {
    sunlightActive = false;
    lastLux = lux;
    sendEmail("Sunlight Stopped!", "Your terrarium is no longer receiving sunlight.\nLast recorded light intensity: " + String(lastLux) + " lx");
  }

  delay(1000); // Delay before the next reading
}

void sendEmail(const char* subject, const String& body) {
  EMailSender::EMailMessage message;
  message.subject = subject;
  message.message = body;

  EMailSender::Response resp = emailSender.send(RECIPIENT_EMAIL, message);

 if (resp.status != true) {
    Serial.print("Error sending Email: ");
    Serial.println(resp.code);  // Print the response code for debugging
  } else {
    Serial.println("Email sent successfully!");
  }

}
