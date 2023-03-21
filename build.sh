set -xe

[[ -d build ]] || mkdir build

CommonCompilerFlags="-Wall -Wextra -ggdb"
LibCFlags="`pkg-config --cflags sdl2 glew`"
LinkerFlags="-ldl `pkg-config --libs sdl2 glew`"

gcc $CommonCompilerFlags $LibCFlags src/sdl_cortex.cpp -o build/sdl_cortex $LinkerFlags
gcc $CommonCompilerFlags -shared -Fpic src/cortex.cpp -o build/libcortex.so