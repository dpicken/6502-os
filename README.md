# 6502-os

An OS and applications for an [Eater](https://eater.net/6502) inspired [6502 computer](https://github.com/dpicken/6502-hw).

## Install tools

Install the cc65 C compiler and minipro chip programmer.

On Arch Linux:

    git clone https://aur.archlinux.org/cc65.git
    cd cc65
    makepkg -i
    cd ...
    git clone https://aur.archlinux.org/srecord.git
    cd srecord
    makepkg -i
    cd ..
    git clone https://aur.archlinux.org/minipro-git.git
    cd minipro-git
    makepkg -i

On macOS, via brew:

    brew install cc65
    brew install minipro

## Build

Clone the fab build system and the 6502-os project:

    git clone https://github.com/dpicken/fab
    git clone https://github.com/dpicken/6502-os

Build:

    cd 6502-os
    make

Write EEPROM:

    make install
