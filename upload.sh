#!/usr/bin/sh

rm -rf pybedrock.egg-info
rm -rf build
rm -rf dist

python setup.py bdist_wheel register upload
twine upload --repository pypi dist/*
