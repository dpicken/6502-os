# 6502-os

An OS and applications for an [Eater](https://eater.net/6502) derived 6502 computer.

## Install tools

Install cc65 (C compiler).  On Arch Linux:

    git clone https://aur.archlinux.org/cc65.git
    cd cc65
    makepkg -i

Install minipro (EEPROM programmer).  On Arch Linux:

    git clone https://aur.archlinux.org/srecord.git
    cd srecord
    makepkg -i
    cd ..
    git clone https://aur.archlinux.org/minipro-git.git
    cd minipro-git
    makepkg -i

## Build

Clone the fab build system and the 6502-os project:

    git clone https://github.com/dpicken/fab
    git clone https://github.com/dpicken/6502-os

Build:

    cd 6502-os
    make

## Software
