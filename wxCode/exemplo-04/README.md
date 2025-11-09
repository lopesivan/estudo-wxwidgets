
#  <exe id="minimal" template="wxlike">
```{bash}
$ ./build.sh
writing Makefile
g++ -c -o gccgtk3_dll/minimal_minimal.o  -g   -I/usr/lib/x86_64-linux-gnu/wx/include/gtk3-unicode-3.2 -I/usr/include/wx-3.2 -D_FILE_OFFSET_BITS=64 -DWXUSINGDLL -D__WXGTK__ -pthread -MTgccgtk3_dll/minimal_minimal.o -MF`echo gccgtk3_dll/minimal_minimal.o | sed -e 's,\.o$,.d,'` -MD -MP ../minimal.cpp
g++ -o gccgtk3_dll/minimal gccgtk3_dll/minimal_minimal.o  -g   `/bin/wx-config  --unicode=yes --static=no --toolkit=gtk3 --version=3.2 --libs core,base`
```


#  <exe id="minimal" template="wx">
```{bash}
$ ./build.sh
writing Makefile
g++ -c -o gccgtk3_dll/minimal_minimal.o -g `/bin/wx-config --cxxflags  --unicode=yes --static=no --toolkit=gtk3 --version=3.2`  -I/usr/lib/x86_64-linux-gnu/wx/include/gtk3-unicode-3.2 -I/usr/include/wx-3.2 -D_FILE_OFFSET_BITS=64 -DWXUSINGDLL -D__WXGTK__ -pthread -MTgccgtk3_dll/minimal_minimal.o -MF`echo gccgtk3_dll/minimal_minimal.o | sed -e 's,\.o$,.d,'` -MD -MP ../minimal.cpp
g++ -o gccgtk3_dll/minimal gccgtk3_dll/minimal_minimal.o  -g   `/bin/wx-config  --unicode=yes --static=no --toolkit=gtk3 --version=3.2 --libs core,base`
```

