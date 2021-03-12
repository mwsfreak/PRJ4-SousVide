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

Builder.load_file('main.kv')

#-- Constants
BUFSIZE = 1000
connStateGlobal = 0
socketConn = socket(AF_INET, SOCK_STREAM)

class HomePage(Widget):
    temperature = ObjectProperty(None)
    time = ObjectProperty(None)

    def btn_settings(self):
        show_settings()
    
    def btn_start(self):
        message = {
            "command" : "start",
            "temp" : self.temperature.text,
            "time" : self.time.text
        }
        sendSocket(socketConn, message)
 
class settingsPage(FloatLayout):
    address = ObjectProperty(None)
    port = ObjectProperty(None)
    global connStateGlobal

    def btn_connect(self):
        global socketConn
        global connStateGlobal

        print(connStateGlobal)

        if (connStateGlobal == 1):
            print(f"Closing connection")

            closeMsg = json.dumps({"command": "close"})
            Lib.writeTextTCP(closeMsg, socketConn)
            socketConn.close()

            self.ids.btn_conn.text = "Connect"
            self.ids.btn_conn.background_color = 0.5, 0, 0, 1
            connStateGlobal = 0

        else:
            # Initialize socket and establish connection to server
            socketConn = socket(AF_INET, SOCK_STREAM)

            print(f"Connecting to server at {self.address.text}:{self.port.text}")
            socketConn.connect((self.address.text,int(self.port.text)))
            self.ids.btn_conn.text = "Disconnect"
            self.ids.btn_conn.background_color = 0, 0.5, 0, 1
            print("Client connected!")
            connStateGlobal = 1
            print(f"connected is = {connStateGlobal}")

        return socketConn
        
def show_settings():
    show = settingsPage()
    if (connStateGlobal == 1):
        show.ids.btn_conn.text = "Disconnect"
        show.ids.btn_conn.background_color = 0, 0.5, 0, 1
    else:
        show.ids.btn_conn.text = "Connect"
        show.ids.btn_conn.background_color = 0.5, 0, 0, 1

    popupWindow = Popup(title="Settings page title", content=show, size_hint=(None, None), size=(dp(400),dp(300)))
    popupWindow.open()


def sendSocket(clientSocket, message):
    print(f"Sending command to server: {message}")
    Lib.writeTextTCP(json.dumps(message), clientSocket)


class SousVideApp(App):
    def build(self):
        return HomePage()

if __name__ == '__main__':
    SousVideApp().run()
