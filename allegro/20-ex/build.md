mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel
./wx_allegro_paint        # Linux/macOS
wx_allegro_paint.exe      # Windows
