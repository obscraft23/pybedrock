try:
  from pybedrock.cmodules.lib.libleveldbhandler import *
except:
  try:
    from pybedrock.cmodules.lib64.libleveldbhandler import *
  except:
    from pybedrock.cmodules.lib.leveldbhandler import *