GEK - openGl nicE renderer

Mikroprojekt na zajęcia Grafiki Komputerowej na PUT Poznań.

## Prerequisites ##
*   OpenGL 3.3 or newer
*   GLFW
*   GLM
*   GNU Make
*   C/C++ Compiler (e.g. gcc)

## Download ##
> git clone --recursive https://github.com/05032MB/gek

## Install ##
### (k)Ubuntu ###
> sudo apt install libglfw3-dev libglew-dev mesa-common-dev libglm-dev
### Windows ###
Zainstaluj:
* Git bash + przyjaciele którzy z nim przychodzą
* MinGW albo podobny kompilator
* CMake
* Make (jeśli jeszcze go nie ma z którymś wyżej)
* Wszystkie z nich dodane do %PATH

## Build ##
### Linux ###
> make

### Windows ###
Uruchom `prepWindows.sh`. Musisz to zrobić w shellu basha/cygwina/możliwie MSYS. **Tylko raz, jeśli wszystko się skompiluje, nie trzeba więcej.** Zajmie trochę czasu.

GLEW jest upośledzony. Jeśli nie skompiluje się, albo nie utworzy biblioteki `glew32.dll` (albo podobnej) w folderze `lib` skompiluj go ręcznie, odpowiednio modyfikując skrypt.

Po wszystkim `make`. Zmień rozszerzenie na `exe` (możesz ustawić mu w Makefile) i gotowe.
