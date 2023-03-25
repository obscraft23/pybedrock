from codecs import open
from os import path
import os
from pip._internal import main as _main
try:
    from skbuild import setup
except:
    _main(['install', 'scikit-build'])
    from skbuild import setup

here = path.abspath(path.dirname(__file__))

"""
try:
    os.system('patch -p1 < CMakeLists.txt.patch')
except:
    print("WARN: A patch to enable RTTI for leveldb-mcpe is not applied. setup.py may fail.")
"""

patchcmakelists = path.join(here, 'CMakeLists.leveldb-mcpe.txt')
targetcmakelists = path.join(here, 'pybedrock/clib/leveldb-mcpe/CMakeLists.txt')
os.system('cp -r '+patchcmakelists+' '+targetcmakelists)

with open(path.join(here, 'README.md'), encoding='utf-8') as f:
    long_description = f.read()

setup(
    name='pybedrock',
    packages=['pybedrock'],

    cmake_install_dir="pybedrock/cmodules",
    cmake_source_dir = "./pybedrock/clib",

    version='0.0.6',

    license='MIT',

    install_requires=['numpy'],
    author='obscraft23',
    author_email='obscraft23@gmail.com',

    url='https://github.com/obscraft23/pybedrock',

    description='A Python package to read/write Minecraft Bedrock leveldb data',
    long_description=long_description,
    long_description_content_type='text/markdown',
    keywords='Minecraft Bedrock leveldb',

    classifiers=[
        'License :: OSI Approved :: MIT License',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: 3.7',
        'Programming Language :: Python :: 3.8',
        'Programming Language :: Python :: 3.9',
        'Programming Language :: Python :: 3.10',
        'Programming Language :: Python :: 3.11',
        'Programming Language :: C++',
    ],
)
