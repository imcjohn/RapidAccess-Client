// Defines config datatype
struct Config{
  char ssid[32];
  char pass[32];
  char adminpass[32];
  char host[32];
  boolean station;
};

// Defines current status datatype
enum Status{
  Authenticated,
  Disconnected,
  Error, // error connectiong
  Incorrect, // bad key,
  Reset // reset to factory
};
