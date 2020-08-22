if [ -z $SHELL ] ; then
echo "Uruchom z shella gita/msys/cygwina (bash)"
exit
fi

rm -rf glDeps
mkdir glDeps
cd glDeps
git clone --depth 1 --branch master https://github.com/nigels-com/glew
cd glew/auto
make -j `nproc`
make -j `nproc`
cd ..
make -j `nproc`
cd ..
git clone --depth 1 --branch master https://github.com/glfw/glfw
cd glfw
cmake . -G "MinGW Makefiles"
make -j `nproc` glfw
cd ..
git clone --depth 1 --branch master https://github.com/g-truc/glm
cp glew/lib/glew32.dll ../glew32.dll
echo "Koniec"