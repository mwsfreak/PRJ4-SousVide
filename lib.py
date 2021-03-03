import os

class Lib(object):
    @staticmethod
    def extractFilename(fileName):
        return fileName[fileName.rfind("/", 0,  len(fileName))+1:]

    @staticmethod
    def check_File_Exists(fileName):
        try:
            size = os.stat(fileName).st_size
        except: 
            size = 0
            
        return size

    @staticmethod
    def readTextTCP(client):
        text = ""
        ch = client.recv(1)
        
        while ch != b'\0':
            text += ch.decode()
            ch = client.recv(1)
        
        return text
            
    @staticmethod
    def writeTextTCP(text,  client):
        client.send((text+"\0").encode())
        
    @staticmethod
    def getFileSizeTCP(client):
        filesize = 0
        try:
            filesize = int(Lib.readTextTCP(client))
        except: 
            filesize = -1

        return filesize