/**
 * Convert json config to struct config
 * 
 * @param doc, json doc to configure from
 * @param updateExisting, if true use existing values rather than default
 * @returns loaded config, with default values where appropriatae
 */
Config jsonToStruct(StaticJsonDocument<512> &doc, boolean updateExisting){
  Config out;
  char defaultWifi[32];
  sprintf(defaultWifi, "QuickAccess_%lu", ESP.getChipId());
  if (updateExisting){
    strlcpy(out.pass, doc["wifipass"] | conf.pass, 32);
    strlcpy(out.ssid, doc["wifiname"] | conf.ssid, 32);
    strlcpy(out.host, doc["hostname"] | conf.host, 32);
    strlcpy(out.adminpass, doc["adminpass"] | conf.adminpass, 32);
  }
  else{
    strlcpy(out.pass, doc["wifipass"] | "PASSWORD", 32);
    strlcpy(out.ssid, doc["wifiname"] | defaultWifi, 32);
    strlcpy(out.host, doc["hostname"] | "rapidaccess", 32);
    strlcpy(out.adminpass, doc["adminpass"] | "password", 32);
  }
  if (doc["mode"] && (strcmp(doc["mode"],"sta") == 0)){
    out.station = true; 
  }
  else{
    out.station = false;
  }
  return out;
}

/**
 * Dump config c into json document doc
 * 
 * @param doc, document to dump into
 * @param c, config to read from
 */
void dumpConfigJson(StaticJsonDocument<512> &doc, Config c){
  doc["wifipass"] = c.pass;
  doc["wifiname"] = c.ssid;
  doc["hostname"] = c.host;
  doc["adminpass"] = c.adminpass;
  if (c.station){
    doc["mode"] = "sta";
  }
  else{
    doc["mode"] = "acc";
  }
}

/**
 * Load config from filesystem
 * 
 * @param defaults, if true, use default configuration values
 * @returns loaded config, with default values where appropriate
 */
Config loadConfig(boolean defaults){
  File file;
  bool opened = false;
  StaticJsonDocument<512> doc;
  if (!defaults && SPIFFS.exists("/config.json")){
    file = SPIFFS.open("/config.json", "r");
    opened = true;
    deserializeJson(doc, file);
  }
  if (opened){
    file.close();
  }
  return jsonToStruct(doc, false);
}

/**
 * Save configuration to filesystem
 * 
 * @param c configuration to save
 * @returns true if successful, otherwise false
 */
boolean dumpConfig(Config c){
  StaticJsonDocument<512> doc;
  dumpConfigJson(doc,c);  
  File file = SPIFFS.open("/config.json", "w");
  if (file){
    boolean result = serializeJson(doc, file);
    return result;
  }
  file.close();
  return false;
}
