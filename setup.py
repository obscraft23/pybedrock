from setuptools import setup
from codecs import open
from os import path

from distutils.command.register import register as register_orig
from distutils.command.upload import upload as upload_orig

here = path.abspath(path.dirname(__file__))

with open(path.join(here, 'README.md'), encoding='utf-8') as f:
    long_description = f.read()

class register(register_orig):

    def _get_rc_file(self):
        return os.path.join('.', '.pypirc')

class upload(upload_orig):

    def _get_rc_file(self):
        return os.path.join('.', '.pypirc')

setup(
    name='pybedrock',
    packages=['pybedrock'],

    version='0.0.1',

    license='MIT',

    install_requires=['numpy','tqdm'],
    author='obscraft23',
    author_email='obscraft23@gmail.com',

    url='https://github.com/obscraft23/pybedrock',

    description='A Python package for analysis and modification of world dataset in Minecraft Bedrock',
    long_description=long_description,
    long_description_content_type='text/markdown',
    keywords='Minecraft Bedrock leveldb NTB',

    classifiers=[
        'License :: OSI Approved :: MIT License',
        'Programming Language :: Python :: 3.8',
    ],
)
