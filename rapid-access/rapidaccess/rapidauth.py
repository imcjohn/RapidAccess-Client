import json
import random
import time
import logging
from .ed25519 import checkvalid

pk = bytes([201, 131, 169, 176, 3, 49, 193, 225, 136, 10, 42, 218, 79, 190, 13, 232, 121, 226, 116, 42, 177, 150, 33, 147, 38, 179, 164, 200, 171, 16, 81, 100])

"""
/**
 * These methods execute an authentication handshake as follows
 * Client - Send 'S', repeats until getting 'G'
 * ESP - Sends back 'G', waits for string
 * Client - Sends 16-byte string to be signed
 * ESP - Sends string back as JSON, with brackets on each side
 * Client - Sends 'C' if string is correct, otherwise anything else
 * This will repeat until it works or something crashes etc
 */
"""


def gen_sequence(length):
    """
    Generates a test sequence to hash of size length
    :param length: size of sequence
    :return: bytes sequence of length length
    """
    options = ['a', 'b', 'c', 'd', 'e', 'f']
    string = ''.join([random.choice(options) for _ in range(length)])
    return string.encode()


def check_authentication(conn, reset_factory=False, max_retries=5):
    """
    Tries to perform authentication handshake with client, checking signature on the way
    :param max_retries: attempts to retry the handshake
    :param reset_factory: if true, tell dongle to reset to factory after successful handshake
    :param conn: serial connection to ESP
    :return: True if successful, otherwise False
    """
    seq = gen_sequence(16)
    success = False
    logging.info("Starting handshake with test string %s" % str(seq))
    for i in range(max_retries):
        conn.write(b'S')  # tell ESP to start authentication
        ch = conn.read()
        if ch == b'G':
            logging.info("Got correct response!")
            success = True
            break  # synced up, ready to go
        time.sleep(2)  # wait a bit, then try again
    if not success:
        return False
    conn.write(seq)
    resp = conn.read_until(b']').decode()
    logging.debug("Received resp: '%s'" % resp)
    data = bytes(json.loads(resp))
    if checkvalid(data, seq, pk):
        if reset_factory:
            conn.write(b"R")
        else:
            conn.write(b"C")  # correct signature
        logging.info("Correct signature!")
        return True
    else:
        conn.write(b"X")  # incorrect signature
        logging.error("Incorrect signature, retrying")
        time.sleep(1)  # wait a bit, then try again
        return check_authentication(conn, reset_factory, max_retries - 1)
