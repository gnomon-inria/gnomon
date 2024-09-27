from setuptools import setup, find_packages

short_descr = "{{short_descr}}"
readme = open("README.md")

# find packages
pkgs = find_packages('src')

setup_kwds = dict(
    name='{{package_name}}',
    version="0.1.0",
    description=short_descr,
    long_description=readme,  
    author="{{author.name}}",
    author_email="{{author.mail}}",
    url='{{url}}',
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
    # declare package plugins
    entry_points={
        # "plugin_group": ["plugin_module = package_name.import.path.plugin_module"]
    },
    keywords='',

    test_suite='nose.collector',
)

setup(**setup_kwds)
