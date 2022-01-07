#!/usr/bin/env python
# -*- coding: utf-8 -*-


from setuptools import setup, find_packages
from gnomon_utils.gnomonPlugin import gnomon_declare_plugins

short_descr = "{{short_descr}}"
readme = open("README.md")

# find packages
pkgs = find_packages('src')



setup_kwds = dict(
    name='plugin_{{package_name}}',
    version="0.1.0",
    description=short_descr,
    long_description=readme,  
    author="{{author.name}}",
    author_email="{{author.mail}}",
    url='',
    license='{{license}}',
    zip_safe=False,

    packages=pkgs,

    package_dir={'': 'src'},
    setup_requires=[

    ],
    install_requires=[

    ],
    tests_require=[

    ],
    entry_points={
        'entry_points': gnomon_declare_plugins('src/plugin_{{package_name}}'),
    },
    keywords='',

    test_suite='nose.collector',
)


setup(**setup_kwds)
