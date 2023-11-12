#include "access_point.h"
#include "../preference/wifi_pref.h"

// ===========================
// Access point credentials
// ===========================
const char* ssid     = "ESP32-Hotspot";
const char* password = "aezakmi1";

WiFiServer AP_server; 

void wifi_connection_attempt(){
    if(get_wifi_ssid() != NULL && get_wifi_pswd() != NULL){
        String ssid = get_wifi_ssid();
        String pswd = get_wifi_pswd();
        Serial.println(ssid);
        Serial.println(pswd);
        WiFi.begin(ssid, pswd);
        WiFi.setSleep(false);
    } else {
        return;
    }

    int connection_counter = 20;
    while (WiFi.status() != WL_CONNECTED && connection_counter > 0) {
        delay(500);
        Serial.print(".");
        connection_counter = connection_counter - 1;
    }
    Serial.println(WiFi.status());
}

void check_wifi_status(bool &ap_running){
    if(WiFi.status() == WL_CONNECTED){
        turn_off_access_point(ap_running);
        Serial.println("\nWiFi connected");
        startCameraServer();
        Serial.print("Camera Ready! Use 'http://");
        Serial.print(WiFi.localIP());
        Serial.println("' to connect");
    } else if(!ap_running){
        setup_access_point(ap_running);
    }
}

void setup_access_point(bool &ap_running){
    if(!ap_running){
        AP_server = WiFiServer(80);
        Serial.println("Setting Access Point...");
        WiFi.softAP(ssid, password);

        IPAddress IP = WiFi.softAPIP();
        Serial.print("AP IP address: ");
        Serial.println(IP);
        AP_server.begin();
        ap_running = true;
    }
}

void turn_off_access_point(bool &ap_running){
    if(ap_running){
        Serial.print("Turning Access Point off...");
        AP_server.stop();
        AP_server.close();
        ap_running = false;
    }
}

bool client_request_listener(bool &ap_running){
    WiFiClient client = AP_server.available();   // Listen for incoming clients
    bool request_status = false;
    if (client) {                          
        String currentLine = "";    // make a String to hold incoming data from the client
        String header = "";
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                Serial.write(c);
                header += c;
                if (c == '\n') {
                    if (currentLine.length() == 0) {
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println("Connection: close");
                        client.println();
            
                        if (header.indexOf(GET_CREDENTIALS) >= 0) {
                            String ssid = header.substring(header.indexOf(SSID) + strlen(SSID), header.indexOf(PARAM_DIVIDER));
                            String pswd = header.substring(header.indexOf(PSWD) + strlen(PSWD), header.indexOf(PARAM_DIVIDER, header.indexOf(PSWD) + 1));
                            save_wifi(ssid, pswd);
                            wifi_connection_attempt();
                            check_wifi_status(ap_running);
                            request_status = true;
                        }

                        client.println(WiFi.localIP());
                        client.println(); // Send the HTTP response
                        break;
                    } else {
                        currentLine = "";
                    }
                } else if (c != '\r') {
                    currentLine += c;
                }
            }
    }
    header = "";

    // Close the connection
    client.stop();
    }
    return request_status;
}
