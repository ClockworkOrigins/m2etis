#!/usr/bin/env python
__author__ = 'Andreas M. Wahl'

import logging
import paramiko


logging.getLogger("paramiko").setLevel(logging.WARNING)


class SSHClient:

    def __init__(self, hostname, port, username, key_filename):
        self.hostname = hostname
        self.port = port
        self.username = username
        self.key_filename = key_filename
        self.client = paramiko.SSHClient()
        self.client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        self.client.load_system_host_keys()

    def connect(self):
        """
        Connect to the remote server.
        """
        self.client.connect(hostname=self.hostname, username=self.username, key_filename=self.key_filename)

    def disconnect(self):
        """
        Disconnect from the remote server.
        """
        self.client.close()

    def execute(self, command):
        """
        Execute a command on the remote server and retrieve the remote I/O-Streams.
        :param command: command to be executed
        :return: list containg the stdin, stdout and stderr of the executed command
        """
        inpt, outpt, errpt = self.client.exec_command(command)
        return inpt, outpt, errpt

    def copy_file(self, file_path, local_file):
        """
        Copy a local file to the remote server.
        :param file_path: target path on the remote server
        :param local_file: local file to be copied
        """
        sftp_client = self.client.open_sftp()
        sftp_client.put(local_file, file_path)
        sftp_client.close()

    def delete_file(self, file_path):
        """
        Delete a file from the remote sever.
        :param file_path: path to the file to be deleted
        """
        sftp_client = self.client.open_sftp()
        sftp_client.remove(file_path)
        sftp_client.close()

    def read_file(self, file_path):
        """
        Read a file from the remote server.
        :param file_path: path to the file to be read
        :return: contents of the remote file
        """
        sftp_client = self.client.open_sftp()
        fileObject = sftp_client.file(file_path)
        fileContent = fileObject.readlines()
        fileObject.close()
        sftp_client.close()
        return fileContent

    def stat(self, file_path):
        """
        Check if a file exists on the remote server.
        :param file_path: path of the file to be checked
        :return: True if the file exists, False otherwise
        :raise: IOError if there is a problem with the remote file system
        """
        sftp_client = self.client.open_sftp()
        try:
            sftp_client.stat(file_path)
        except IOError, e:
            if e[0] == 2:
                return False
            raise
        else:
            return True
        finally:
            sftp_client.close()
