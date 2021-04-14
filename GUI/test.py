#!python3

#-- Python imports
from socket import socket, AF_INET, SOCK_STREAM
from lib import Lib
import json

#-- Kivy imports

import kivy
kivy.require('1.0.6') # replace with your current kivy version !

from kivy.app import App
from kivy.lang import Builder 
from kivy.uix.floatlayout import FloatLayout
#from kivy.uix.gridlayout import GridLayout
from kivy.uix.widget import Widget
#from kivy.uix.button import Button
from kivy.properties import ObjectProperty
from kivy.uix.popup import Popup
from kivy.metrics import dp, sp

Builder.load_file('test.kv')

#-- Constants
BUFSIZE = 1000
connected = 0
socketConn = socket(AF_INET, SOCK_STREAM)

class HomePage(Widget):
    pass
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

            closeMsg = json.dumps({"command": "close"})
            Lib.writeTextTCP(closeMsg, socketConn)
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
    message = {
        "command" : "start",
        "temp" : "60",
        "time" : "240"
    }

    # Send filename to server
    print(f"Sending command to server: {message}")
    Lib.writeTextTCP(json.dumps(message), clientSocket)


class myTest(App):
    def build(self):
        return HomePage()

if __name__ == '__main__':
    myTest().run()
