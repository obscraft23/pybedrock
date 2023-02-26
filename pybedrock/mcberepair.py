import os
import subprocess

CMAKE_BIN_DIR = os.path.join(os.path.dirname(__file__), "mcberepair")

class world():
    def __init__(self,wdname):
        self.wdname = wdname

    def _mcberepair(self,operation,inputs,nbt=None):
        cmd = [os.path.join(CMAKE_BIN_DIR, "bin/mcberepair"),operation]+inputs
    
        if nbt == None:
            return subprocess.run(cmd,stdout=subprocess.PIPE, stderr=subprocess.STDOUT).stdout
        else:
            return subprocess.run(cmd,stdout=subprocess.PIPE, stderr=subprocess.STDOUT, input=nbt).stdout

    def listkeys(self,worldDirPath):
        
        rawoutput = self._mcberepair("listkeys",[worldDirPath])
        
        keycontentlist = rawoutput.decode('utf-8').split('\n')[1:]
        tsvkeycontentlist = [keycontent.split('\t') for keycontent in keycontentlist]

        return tsvkeycontentlist

    def removekeys(self,worldDirPath,key):

        return self._mcberepair("rmkeys",[worldDirPath,key])

    def getNBTfromkey(self,worldDirPath,key):
        
        return self._mcberepair("dumpkey",[worldDirPath,key])

    def putNBTtokey(self,worldDirPath,key,nbt):
        
        return self._mcberepair("writekey",[worldDirPath,key],nbt=nbt)