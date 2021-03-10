#!python3

import kivy
kivy.require('1.0.6') # replace with your current kivy version !

from kivy.app import App
from kivy.uix.floatlayout import FloatLayout
#from kivy.uix.gridlayout import GridLayout
from kivy.uix.widget import Widget
#from kivy.uix.button import Button
from kivy.properties import ObjectProperty
from kivy.uix.popup import Popup
from kivy.metrics import dp, sp

from socket import socket, AF_INET, SOCK_STREAM
from lib import Lib

BUFSIZE = 1000
connected = 0
socketConn = socket(AF_INET, SOCK_STREAM)

class HomePage(Widget):
    def btn(self):
        show_settings()
    
    def btn_start(self):
        sendSocket(socketConn)
 
class settingsPage(FloatLayout):
    address = ObjectProperty(None)
    port = ObjectProperty(None)

    def btn_connect(self):
        global connected
        global socketConn

        print(connected)

        if (connected == 1):
            print(f"Closing connection")

            Lib.writeTextTCP("Close me!", socketConn)
            socketConn.close()
            self.ids.btn_connect.text = "Connect"

            connected = 0

        else:
            # Initialize socket and establish connection to server
            socketConn = socket(AF_INET, SOCK_STREAM)

            print(f"Connecting to server at {self.address.text}:{self.port.text}")
            socketConn.connect((self.address.text,int(self.port.text)))
            self.ids.btn_connect.text = "Disconnect"
            print("Client connected!")
            connected = 1
            print(f"connected is = {connected}")

        return socketConn
        
def show_settings():
    show = settingsPage()
    popupWindow = Popup(title="Settings page title", content=show, size_hint=(None, None), size=(dp(400),dp(300)))
    popupWindow.open()

def sendSocket(clientSocket):
    filePath = 'images/audi.jpg'

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
        receiveFile(filename, clientSocket)
    
    clientSocket.close()

def receiveFile(fName,  conn):
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
