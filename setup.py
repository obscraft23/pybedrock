from codecs import open
from os import path
import os
from pip._internal import main as _main
import shutil
import platform
import tarfile

try:
    from skbuild import setup
    import requests
except:
    _main(['install', 'scikit-build'])
    _main(['install', 'requests'])
    from skbuild import setup
    import requests

here = path.abspath(path.dirname(__file__))

patchcmakelists = path.join(here, 'patch/CMakeLists.txt')
targetcmakelists = path.join(here, 'pybedrock/clib/leveldb-mcpe/CMakeLists.txt')

shutil.copy(patchcmakelists,targetcmakelists)

if platform.system() == 'Windows':
    patchcmakelists = path.join(here, 'patch/zlib_compressor.cc')
    targetcmakelists = path.join(here, 'pybedrock/clib/leveldb-mcpe/db/zlib_compressor.cc')
    shutil.copy(patchcmakelists,targetcmakelists)

"""
if platform.system() == 'windows':
    pwd = os.getcwd()
    zlibpath = path.join(here,"zlib/")
    os.makedirs(zlibpath,exist_ok=True)
    os.chdir(zlibpath)
    res = requests.get('https://zlib.net/zlib-1.2.13.tar.gz')
    with open(zlibpath+'zlib-1.2.13.tar.gz') as f:
        f.write(res.content)
    with tarfile.open('test.tar.gz', 'r:gz') as tar:
        tar.extractall(path='./')
    os.chdir('zlib-1.2.13')
    if os.path.exists('build/'):
        shutil.rmtree('build/')
    os.makedirs('build/', exist_ok=True)
    os.chdir()
    os.system('cmake .. -G "Visual Studio 17 2022" -A x64 -T host=x64 -DCMAKE_INSTALL_PREFIX="c:/zlib-1.2.12"')
    os.system('cmake --build . --config RELEASE')
    os.system('cmake --build . --config RELEASE --target INSTALL')
    os.chdir(pwd)
"""

with open(path.join(here, 'README.md'), encoding='utf-8') as f:
    long_description = f.read()

if platform.system() == 'Windows':
    import sysconfig
    libdir = sysconfig.get_path('include').replace("Include","libs")
    print("########################################################")
    print(platform.system())
    if libdir != None:
        os.environ["LIBPATH"] = libdir
        try:
            os.environ["LIB"] += ";"+libdir
        except:
            os.environ["LIB"] = libdir
        print(libdir)
        
        print(os.environ["LIB"])
        print(os.environ["LIBPATH"])
        #print(os.environ["ZLIB_INCLUDE_DIRS"])
        #print(os.environ["ZLIB_LIBRARIES"])

    else:
        print(here)
        print("NO LIB!!!!!")
    print("########################################################")
    setup(
        name='pybedrock',
        packages=['pybedrock'],

        cmake_install_dir="pybedrock/cmodules",
        cmake_source_dir = "./pybedrock/clib",
        cmake_args=['-DCMAKE_BUILD_TYPE=Release','-DVCPKG_TARGET_TRIPLET=x64-windows-static-md','-DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake'],

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

else:
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