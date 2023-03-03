#!/usr/bin/sh

rm -rf pybedrock.egg-info
rm -rf build
rm -rf dist
rm -rf _skbuild

python setup.py sdist
pip uninstall pybedrock -y
python  setup.py install 
#pip install dist/pybedrock-0.0.3-cp38-cp38-linux_x86_64.whl