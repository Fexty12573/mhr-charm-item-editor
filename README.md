# mhr-charm-item-editor
A Charm/Item editor for Monster Hunter Rise.

The project is written in C++/CLR (yes, I know, it's shit). I was too lazy to use some proper C++ GUI library so this seemed like the easiest way to go at the start.
Using CLR ended up making things much more complicated than they needed to be tho lol.

# Building
You need the C++/CLI/CLR Package from the Visual Studio Installed installed. Then simply open the solution in VS2019/2022 and build. It should build out of the box.
Intellisense marks some errors with array access in Window.cpp but you can ignore those, intellisense just doesn't work properly for C++/CLR.
