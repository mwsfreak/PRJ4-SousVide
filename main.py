import kivy
kivy.require('1.0.6') # replace with your current kivy version !

from kivy.app import App
from kivy.uix.floatlayout import FloatLayout
from kivy.uix.gridlayout import GridLayout
from kivy.uix.widget import Widget
from kivy.uix.button import Button

#from android.permissions import request_permissions, Permission
#request_permissions([Permission.INTERNET])

from socket import socket, AF_INET, SOCK_STREAM
from lib import Lib

PORT = 9000
BUFSIZE = 1000

class HomePage(Widget):

    def runSocket(self):
        print("runSocket Called!")
        serverName = 'bisgaardfranks.dk'
        filePath = 'images/audi.jpg'

        # Initialize socket and establish connection to server
        clientSocket = socket(AF_INET, SOCK_STREAM)

        print(f"Client is connecting to server at {serverName}:{PORT}")
        clientSocket.connect((serverName,PORT))

        print("Client connected!")

        # Send filename to server
        Lib.writeTextTCP(filePath, clientSocket)
        print(f"Requesting file from server: {filePath}")

        # Get size of requested file
        filesize = Lib.getFileSizeTCP(clientSocket)

        if filesize <= 0:
            print(f"File not found on server. Aborting!")
        else:
            print(f"File exists on server, transferring {filesize} bytes!")

            filename = Lib.extractFilename(filePath) 
            self.receiveFile(filename, clientSocket)

    def receiveFile(self,fName,  conn):
        chunkCounter = 1
        '''
        # If filename exists, delete file before downloading
        if Lib.check_File_Exists(fName):
            os.remove(fName)
        '''
        while True:
            chunk = conn.recv(BUFSIZE)
            print(f"Received chunk #{chunkCounter}")

            # If chunk is empty (nothing received)
            if not chunk:
                break
            '''
            # Writing to file 
            with open(fName, "ab") as file1: 
                # Writing data to a file 
                file1.write(chunk)
            '''
            chunkCounter += 1


class SousVideApp(App):
    def build(self):
        return HomePage()


if __name__ == '__main__':
    SousVideApp().run()
