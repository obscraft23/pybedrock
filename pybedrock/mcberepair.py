import os
import subprocess

CMAKE_BIN_DIR = os.path.join(os.path.dirname(__file__), "mcberepair")

def _mcberepair(operation,inputs,nbt=None):
    cmd = [os.path.join(CMAKE_BIN_DIR, "bin/mcberepair"),operation]+inputs

    if nbt == None:
        return subprocess.run(cmd,stdout=subprocess.PIPE, stderr=subprocess.STDOUT).stdout
    else:
        return subprocess.run(cmd,stdout=subprocess.PIPE, stderr=subprocess.STDOUT, input=nbt).stdout

def listkeys(worldDirPath):
    
    rawoutput = _mcberepair("listkeys",[worldDirPath])
    
    keycontentlist = rawoutput.decode('utf-8').split('\n')[1:]
    tsvkeycontentlist = [keycontent.split('\t') for keycontent in keycontentlist]

    return tsvkeycontentlist

def removekeys(worldDirPath,key):

    return _mcberepair("rmkeys",[worldDirPath,key])

def getNBTfromkey(worldDirPath,key):
    
    return _mcberepair("dumpkey",[worldDirPath,key])

def putNBTtokey(worldDirPath,key,nbt):
    
    return _mcberepair("writekey",[worldDirPath,key],nbt=nbt)