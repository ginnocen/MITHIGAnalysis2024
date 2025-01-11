PY8_PYTHIA8_INCLUDE=`pythia8-config --includedir`
PY8_PYTHIA8_LIB=`pythia8-config --libdir`
g++ ${1}.cc \
 -O2 -ansi -W -Wall -std=c++17 -Wshadow -m64 -Wno-shadow \
 -o ${1}.exe $PY8_PYTHIA8_LIB/libpythia8.a\
         -I$PY8_PYTHIA8_INCLUDE -L$PY8_PYTHIA8_LIB -lpythia8 \
     `root-config --cflags --glibs` `pythia8-config --root` -lz \
     `/home/yuchenc/fastjet-3.4.2/fastjet-config --cxxflags --libs --plugins` `pythia8-config --fastjet3` -lfastjettools
     # -L$PY8_YAMLCPP_LIB -I$PY8_YAMLCPP_INCLUDE -lyaml-cpp \
     # $PY8_FLAGS
