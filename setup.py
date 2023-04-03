from codecs import open
from os import path
import os
from pip._internal import main as _main
import shutil
import platform
import sysconfig
import sys

try:
    from skbuild import setup
except:
    _main(['install', 'scikit-build'])
    from skbuild import setup

here = path.abspath(path.dirname(__file__))

patchcmakelists = path.join(here, 'patch/CMakeLists.txt')
targetcmakelists = path.join(here, 'pybedrock/clib/leveldb-mcpe/CMakeLists.txt')

shutil.copy(patchcmakelists,targetcmakelists)

if platform.system() == 'Windows':
    patchcmakelists = path.join(here, 'patch/zlib_compressor.cc')
    targetcmakelists = path.join(here, 'pybedrock/clib/leveldb-mcpe/db/zlib_compressor.cc')
    shutil.copy(patchcmakelists,targetcmakelists)

with open(path.join(here, 'README.md'), encoding='utf-8') as f:
    long_description = f.read()

if platform.system() == 'Windows':
    
    libdir = sysconfig.get_path('include').replace("Include","libs")
    
    if libdir != None:
        os.environ["LIBPATH"] = libdir+";"+r"C:\vcpkg\installed\x64-windows\lib"
        try:
            os.environ["LIB"] += ";"+libdir+";"+r"C:\vcpkg\installed\x64-windows\lib"
        except:
            os.environ["LIB"] = libdir+";"+r"C:\vcpkg\installed\x64-windows\lib"
    cmake_args=['-DCMAKE_BUILD_TYPE=Release','-DVCPKG_TARGET_TRIPLET=x64-windows',r'-DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake']

else:
    libdir = sysconfig.get_path('include').replace("Include","libs")
    pyversion = str(sys.version_info.major)+"."+str(sys.version_info.minor)
    os.environ["PYTHON_MAC_LIBRARIES_ENV"] = "/Library/Frameworks/Python.framework/Versions/"+pyversion+"/lib/libpython"+pyversion+".dylib"
    if platform.system() == "Linux":
        os.environ["PYTHON_MAC_LIBRARIES_ON"] = "off"
        cmake_args = []
    else:
        os.environ["PYTHON_MAC_LIBRARIES_ON"] = "on"
        cmake_args = ['-Wno-error=unused-command-line-argument-hard-error-in-future']

setup(
    name='pybedrock',
    packages=['pybedrock'],
    
    cmake_install_dir="pybedrock/cmodules",
    cmake_source_dir = "./pybedrock/clib",
    cmake_args=cmake_args,
    version='0.0.7',

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