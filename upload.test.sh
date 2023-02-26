#!/usr/bin/sh

rm -rf pybedrock.egg-info
rm -rf build
rm -rf dist

python setup.py bdist_wheel
twine upload --repository testpypi dist/*