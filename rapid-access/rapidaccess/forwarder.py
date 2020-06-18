import logging
import threading
import subprocess


class SSHForwarder:
    def __init__(self, conn, command='/usr/sbin/sshd -i', max_read_size=4096):
        # first declare constants
        self.max_read = max_read_size
        self.command = command
        self.end_sequence = list([x.encode() for x in 'END_SEQUENCE'])  # store terminator as byte list
        self.end_length = len(self.end_sequence)
        # then declare working vars
        self.serial_conn = conn
        self.to_serial = None
        self.from_serial = None
        self.ssh_process = None
        self.end_flag = False
        self.end_index = 0

    def check_ending(self, ch):
        """
        Keeps track of character state, in order to see when end sequence passes by
        """
        if ch == self.end_sequence[self.end_index]:
            self.end_index += 1
            if self.end_index == self.end_length:
                self.end_index = 0
                return True  # end sequence reached
        elif self.end_index != 0:
            self.end_index = 0
            self.check_ending(ch)  # after resetting, have to check if it is the start of an end seq
        return False

    def forward_to_serial(self):
        """
        Forward data from ssh to serial, until stopped
        """
        while self.from_serial.isAlive():
            data = self.ssh_process.stdout.read1(self.max_read)
            self.serial_conn.write(data)

    def forward_to_cmd(self):
        """
        Starts forwarding data from serial to ssh, loops until termination condition
        Terminates on no data received in timeout time, or end sequence sent
        """
        while True:
            data = self.serial_conn.read()
            if not data or self.check_ending(data):  # end on either end sequence or timeout
                logging.info('got end sequence, closing connection')
                self.serial_conn.flushInput()
                self.serial_conn.flushOutput()
                break
            self.ssh_process.stdin.write(data)
            self.ssh_process.stdin.flush()

    def launch_ssh(self):
        """
        Starts ssh connection from inetd, and intializes copying threads
        """
        logging.info('launching ssh process/threads')
        self.ssh_process = subprocess.Popen(self.command.split(' '), stdout=subprocess.PIPE, stdin=subprocess.PIPE)
        self.to_serial = threading.Thread(target=self.forward_to_serial, daemon=True)
        self.from_serial = threading.Thread(target=self.forward_to_cmd, daemon=True)
        self.from_serial.start()
        self.to_serial.start()

    def run_and_wait(self):
        """
        Start ssh connection, and run until completion (disconnect)
        """
        try:
            self.launch_ssh()
            self.from_serial.join()
        finally:
            self.ssh_process.terminate()