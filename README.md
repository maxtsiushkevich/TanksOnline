This is a simple clone of the BattleCity game, written in C++ using the SFML library. 
This implementation of the game supports both online play between 2 players on different computers, and joint play on 1 computer.
TCP is used as the communication protocol.

# Instalation
The game was tested to run on macOS, but running on Linux may cause problems. And of course you must have the SFML library installed for a successful build.
run the command in the terminal while in the source folder:
```
$ make
```
To start the game enter:
```
$ ./Tanks
```

Enjoy the game!

# Settings
## Screen size
If you are not satisfied with the size of the application window, then you can change the `FACTOR` constant in the `main.cpp` file to whatever is convenient for you.

## Сhange port for connection
If you want to change the connection port, change the `PORT` constant in the `GameEngine.h` file.
