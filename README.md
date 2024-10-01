# Nurik-Alik-TP



## Conway's Game of Life

If you have never played the game of life, then you should definitely do it!
This game represents a cellular automata on a toric grid where user can mark a cells,
put templates, choose rules and watch the result.


## Linux

For Linux users following packages have to be installed with appropriate package manager of your distribution:

```
sudo apt update
sudo apt install \
    libxrandr-dev \
    libxcursor-dev \
    libudev-dev \
    libopenal-dev \
    libflac-dev \
    libvorbis-dev \
    libgl1-mesa-dev \
    libegl1-mesa-dev \
    libdrm-dev \
    libgbm-dev
```

## How to download and launch the app?

```
git clone git@gitlab.akhcheck.ru:shalbai.a/nurik-alik-tp.git
mkdir build && cd build
cmake ..
cmake --build .
./run
```

## How to play?

After game is launched you can do following:

- Zoom in/out using 'Num1/Num2' buttons on keyboard. (When zoom out you will see the grid borders)
- Choose templates on left panel, after put it on grid on arbitrary position by clicking Right Mouse Button (RMB).
- Mark living cells on paused game by clicking Left Mouse Button (LMB) on any position on grid. (then you should restart(continue) the game).
- Enjoy!

![alt text](<resources/example.png>)

## Release notes

- Update from 30.03.24 contains following features:
    - View movement available in zoom mode
    - Game speed control with 'Up' and 'Down' arrow keys
    - Choosen target can be placed multiple times and released with 'Backspace' button
    - Game pause and resume can be done by pressing 'Space'
    - Press 'Left Shfit' button to clear grid
- Update from 19.04.24 provides following:  
    - Better speed performance
    - Game preview
    - Custom game rules to choose

## Developers

- Moldanazarov Nurdaulet: https://gitlab.akhcheck.ru/jNurik
- Shalbay Alisher https://gitlab.akhcheck.ru/shalbai.a

