RapidAccess Client
==================

Overview
--------

This python library/executable is designed to provide an easy-to-install piece of software that
makes your computer automatically compatible with the RapidAccess USB Devices. These USB dongles
allow you to easily gain access to a computer that is rendered unable to receive remote access
through the traditional means. By installing this package, you will allow your computer to be 
connected to through a RapidAccess dongle, regardless of internet connectivity problems. 
For more information, or to learn how to disable RapidAccess connections once installed,
please check out our site at https://rapidaccess.dev

Building Instructions
---------------------

You can easily build/install this with a simple pip command from the repository root.

`sudo pip3 install -e rapid-access rapidaccess`

This will build the package, install the rapidaccess module, and place the binaries rapid-client and rapid-autorun in your path. For more information on these, check out our documentation at https://docs.rapidaccess.dev. In addition, this will make rapid-client autorun when a RapidAccess device is plugged in. If you do not want this behavior, simply run the below command in your terminal.

`sudo rapid-client --disable`

NOTE: This will install pyserial as part of the build process
