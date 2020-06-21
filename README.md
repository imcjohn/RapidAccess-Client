This is the working repository for the python client for my RapidAccess dongle, more details available at our website at https://rapidaccess.dev.

If you are interested in building this repository, check out the instructions at https://docs.rapidaccess.dev, at least until we can get a more detailed README set up here.

Also, this project has a hackaday.io page at https://hackaday.io/project/171969-rapidaccess, feel free to check it out!

Overview on each folder:

* frontend: html frontend that runs on the RapidAccess device
* rapid-access: python module for interfacing with the hardware, also contains executables rapid-client and rapid-autorun (both python scripts)
* serial-bridge: the software that runs on the esp8266, in order to bridge the rapid-client serial connection with an ssh socket
* SPIFFS-Upload: a simple arduino shim that I used to upload the index.html file to the esp8266 internal rom
