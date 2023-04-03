from pybedrock.main import *
try:
  from pybedrock.cmodules.lib.libleveldbhandler import *
except:
  try:
    from pybedrock.cmodules.lib64.libleveldbhandler import *
  except:
    from pybedrock.cmodules.lib.leveldbhandler import *

__copyright__    = 'Copyright (C) 2023 obscraft23'
__version__      = '0.0.7'
__author__       = 'obscraft23'
__author_email__ = 'obscraft23@gmail.com'
__url__          = 'https://github.com/obscraft23/pybedrock'
__all__ = []
