import logging
import time
import serial
import sys

from .rapidauth import check_authentication
from .forwarder import SSHForwarder

def restore_factory(port):
    '''
    Restore RapidAccess dongle to factory settings
    :param port: serial port the RA dongle is on
    '''
    print('Resetting RapidAccess to factory settings, please wait...')
    ser = serial.Serial(port, 115200, timeout=600)
    if not check_authentication(ser, reset_factory=True):
        sys.stderr.write("Failed to authenticate with RapidAccess, cannot reset settings")
        sys.exit(1)
    else:
        time.sleep(.5)
        print("Successful reset! Unplug and reconnect the dongle, it will start up with factory settings")


def start_forward(port):
    '''
    Start the rapidaccess forwarding process
    :param port: serial port of the dongle
    '''
    logging.info('Starting the forwarding process...')
    ser = serial.Serial(port, 115200, timeout=600)
    conn = SSHForwarder(ser)
    debounce = 1
    if not check_authentication(ser):
        logging.warning('Failed authentication')
        sys.exit(1)

    while True:
        if ser.in_waiting:
            logging.info('new connection found, starting ssh')
            conn.run_and_wait()
        else:
            time.sleep(debounce)
