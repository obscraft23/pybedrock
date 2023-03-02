from codecs import open
from os import path
from skbuild import setup

here = path.abspath(path.dirname(__file__))

with open(path.join(here, 'README.md'), encoding='utf-8') as f:
    long_description = f.read()

setup(
    name='pybedrock',
    packages=['pybedrock'],

    cmake_install_dir="pybedrock/cmodules",
    cmake_source_dir = "./pybedrock/clib",

    version='0.0.3',

    license='MIT',

    install_requires=['amulet-nbt~=2.0.0','numpy','tqdm'],
    author='obscraft23',
    author_email='obscraft23@gmail.com',

    url='https://github.com/obscraft23/pybedrock',

    description='A Python package to read/write Minecraft Bedrock leveldb data',
    long_description=long_description,
    long_description_content_type='text/markdown',
    keywords='Minecraft Bedrock leveldb',

    classifiers=[
        'License :: OSI Approved :: MIT License',
        'Programming Language :: Python :: 3.8',
        'Programming Language :: C++',
    ],
)