monik.cpp is open source library for asynchronous logging written in C++11/C++14.

SUPPORTED PLATFORMS
===================

Platforms:

*   **Linux**
*   **Windows**
*   **OS X**
*   **FreeBSD**
*   **Ubuntu**

INSTALLATION
============

Minimal dependencies
--------------------

-   C++ compiler with good C++11 support. Compilers which are tested to have
    everything needed are **Clang** >= 3.7 and **MSVC** 2015.
*   **CMake** >= 2.6

Compilation
-----------

The library can be built using these four commands:

    mkdir build
    cd build
    cmake .. 
    make

To build the library with remote logging use one of these commands:

* win_setup/deploy.bat 
* win_setup/deploy.bat Debug
* unix_setup/deploy.sh
* unix_setup/deploy.sh Debug
    
LICENSE
=======

monik.cpp is licensed under MIT license, see [LICENSE](LICENSE) file for
details.