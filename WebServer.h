#ifndef WebServer_h
#define WebServer_h

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "Arduino.h"



class WebServer {
  public:
    WebServer();
    void start();
    void loop();
  private:
    void handleRoot();
    void handleStylesheet();
    void handleJavascript();
    void handleNotFound();
    void redirect(String url);
    String padding(int number, byte width);
    static ESP8266WebServer _webServer;
    String ACCESS_POINT_SSID;
    String ACCESS_POINT_PWD;
    void StartAccessPoint();
    
};

#endif
