# mhr-charm-item-editor
A Charm/Item editor for Monster Hunter Rise.

There are currently 2 separate projects in this directory. Both fulfill the same purpose, but one of them functions as a portable executable to be launched alongside Monster Hunter Rise. The other one is a dll that incorporates itself into REFramework.

## Executable
The project is written in C++/CLR (yes, I know, it's shit). I was too lazy to use some proper C++ GUI library so this seemed like the easiest way to go at the start.
Using CLR ended up making things much more complicated than they needed to be tho lol.

## REFramework Version
The REFramework version of the editor is the new version, and this version will be the one receiving updates from now on, and already has a couple features that the executable does not. The project is written in pure C++20.

By default it supports all languages supported by the game, although *quality of translation and implementation may vary across languages*.

# Building
## C++/CLR Project
You need the C++/CLI/CLR Package from the Visual Studio Installer. Then simply open the solution in VS2019/2022 and build. It should build out of the box.
Intellisense marks some errors with array access in Window.cpp but you can ignore those, intellisense just doesn't work properly for C++/CLR.

## REFramework DLL Project
The project depends on a couple libraries. You can manually install those or make your life easier by installing them with a package manager like vcpkg.

The vcpkg installation command for the necessary libraries is the following.
```
./vcpkg install minhook fmt nlohmann-json imgui --triplet=x64-windows-static
```
Afterwards building in Visual Studio should work fine. Make sure you set the configuration to Release, x64.
