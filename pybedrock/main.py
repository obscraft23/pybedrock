import pybedrock.mcberepair as mr
import pybedrock.nbt as nbt


class world():
    def __init__(self,wdname):
        self.wdname = wdname
        self.keycontents = mr.listkeys(self.wdname)

    def updatelistkeys(self):
        self.keycontents = mr.listkeys(self.wdname)

    def loadNBTdatafromkey(self,key):
        nbtbuffer = mr.getNBTfromkey(self.wdname,key)
        return nbt.loadNBT(nbtbuffer)