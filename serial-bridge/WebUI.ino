const char * admin_username = "admin";

void statusToString(char *out, Status stat){
  switch (stat){
    case Authenticated:
    {   
      strcpy(out,"Authentication Success");
      break;
    }
    case Disconnected:
    {   
      strcpy(out,"Not Connected");
      break;
    }
    case Error:
    {   
      strcpy(out,"Authentication Error");
      break;
    }
    case Incorrect:
    {   
      strcpy(out,"Invalid Key");
      break;
    }
    case Reset:
    {
      strcpy(out,"Factory Reset");
      break;
    }
  }
}

void handleConfig(){
  if (!webInterface.authenticate(admin_username, conf.adminpass))
      return webInterface.requestAuthentication();
  StaticJsonDocument<512> doc;
  dumpConfigJson(doc, conf); // convert current config into JSON
  char out[600];
  // before serialization, add static display values
  String ip =  wifiIP.toString();
  doc["staticDeviceIp"] = ip;
  if (clientConnected)
    doc["staticWirelessClient"] = "Connected";
  else
    doc["staticWirelessClient"] = "Not Connected";
  char usbStatus[32];
  statusToString(usbStatus,auth);
  doc["staticUsbStatus"] = usbStatus;
  size_t len = serializeJson(doc,out);
  webInterface.send(200, "application/json", out, len);
}

void handleUpdate(){
  if (!webInterface.authenticate(admin_username, conf.adminpass))
    return webInterface.requestAuthentication();
  StaticJsonDocument<512> doc;
  for (int i = 0; i < webInterface.args(); i++){
    String param = webInterface.argName(i);
    String value = webInterface.arg(i);
    doc[param] = value;  
  }
  conf = jsonToStruct(doc, true); // perhaps think of a better way to handle all this than globals
  boolean result = dumpConfig(conf); // save updated config to file
  webInterface.send(200, "text/html", "<script>window.onload = function(){alert('Config update complete! ESP is restarting, reconnect once it is back online. NOTE: You may have to unplug and re-plug the RapidAccess USB in order for the computer to see it again.');}</script>");
  delay(1000);
  ESP.restart();
}

void startWebUI(){
  webInterface.onNotFound([](){
    if (!webInterface.authenticate(admin_username, conf.adminpass))
      return webInterface.requestAuthentication();
    File file = SPIFFS.open("/index.html","r");
    webInterface.streamFile(file, "text/html");
    file.close();
  });
  webInterface.on("/config.json", handleConfig);
  webInterface.on("/networkUpdate", handleUpdate);
  webInterface.begin();
}
