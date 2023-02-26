import pybedrock.mcberepair as mr
import pybedrock.nbt

def loadNBTdatafromkey(wdname,key):
    
    nbtbuffer = mr.getNBTfromkey(wdname,key)
    return nbt.loadNBT(nbtbuffer)
