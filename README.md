cpp3ds [![Build Status](https://travis-ci.org/cpp3ds/cpp3ds.png?branch=master)](https://travis-ci.org/cpp3ds/cpp3ds)
======

Basic C++ gaming framework and library for Nintendo 3DS.

cpp3ds is essentially a barebones port of SFML with a parallel native 3ds emulator built on top of it. The goal is to completely abstract the developer from the hardware SDK and provide a nice object-oriented C++ framework for clean and easy coding. And the emulator is designed to provide a means of surface-level realtime debugging (with GDB or whatever you prefer).

Won't be stable and usable until v1.0

Documentation
-------------
Will be released with v1.0 stable

Requirements
------------
 
- DevkitARM
- ctrulib

For emulator:

- [SFML 2.1](http://www.sfml-dev.org/index.php)
- [Qt 5](https://qt-project.org/)

For unit tests:

- [Google Test](https://code.google.com/p/googletest/)

Credit and Thanks
-----------------
- [Kevin Lee](https://github.com/kleemedia) - tween engine port
- [smealum](https://github.com/smealum) - ctrulib
- [minexew](https://github.com/minexew) - ctrgl
