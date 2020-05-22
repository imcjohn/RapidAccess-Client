import setuptools

long_description = '''
This python library/executable is designed to provide an easy-to-install piece of software that
makes your computer automatically compatible with the RapidAccess USB Devices. These USB dongles
allow you to easily gain access to a computer that is rendered unable to receive remote access
through the traditional means. By installing this package, you will allow your computer to be 
connected to through a RapidAccess dongle, regardless of internet connectivity problems. 
For more information, or to learn how to disable RapidAccess connections once installed,
please check out our site at https://rapidaccess.dev
'''

setuptools.setup(
    name="rapidaccess",
    version="0.1.0",
    author="Ian McJohn",
    author_email="info@rapidaccess.dev",
    description="Python client for RapidAccess hardware",
    long_description=long_description,
    long_description_content_type="text/plain",
    url="https://rapidaccess.dev",
    packages=setuptools.find_packages(),
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: GNU General Public License v3 (GPLv3)",
        "Operating System :: POSIX :: Linux",
    ],
    python_requires='>=3',
    install_requires=['pyserial'],
    scripts=['bin/rapid-client']
)