import kivy
kivy.require('1.0.6') # replace with your current kivy version !

from kivy.app import App
from kivy.uix.label import Label
from kivy.uix.gridlayout import GridLayout
from kivy.uix.textinput import TextInput
from kivy.uix.button import Button
from kivy.uix.widget import Widget

class HomePage(Widget):
    pass

class SousVideApp(App):

    def build(self):
        return HomePage()


if __name__ == '__main__':
    SousVideApp().run()
