# CppSTK

This is like my repo PySTK. It was rewrittenn by ChatGPT in C++.
It does work, but lacks almost all the features PySTK has.
For example:
- Trackborders 
- Laptime
- Lapcount
- Who has won
- …

But you can play it on every potato, even though you should probably do something else with your time. 

## Compile
To compile this you will need cmake. You can download it via your packagemanager or with an installer from the web.
Then you can go into the src/ directory and enter following commands:
```
cmake .
make
```
or make the build.sh script executable with 
```
chmod +x build.sh
```
and then execute it with
```
./build.sh
```
This won't work in every shell. You need something like bash or zsh. 
