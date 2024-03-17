# D100FS

Tools and Library for interacting with disk images formatted in PC/MS-DOS 1.00

Documentation is a bit lacking at the moment but I'll get round to it. In the meantime, there
are plenty of doxygen comments in each of the header and source files

To compile and install D100FS, use the following commands:

```
git clone https://github.com/CPI-CoolPeopleInc/D100FS.git
cd D100FS
make
sudo make install
```

By default, D100FS will install to /usr/local. To change this, change your make install command like this:

```
sudo make PREFIX=/path/to/install/dir install
```
