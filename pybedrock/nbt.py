import amulet_nbt
from amulet_nbt import utf8_escape_decoder 

def loadNBT(path_or_buffer):
    bedrock_named_tag = amulet_nbt.load(path_or_buffer,compressed=False,little_endian=True,string_decoder=utf8_escape_decoder)

    return bedrock_named_tag