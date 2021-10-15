#! /usr/bin/sh
g++ $@ -std=c++17 src/*.cpp -o ipresolve $(curl-config --cflags --libs)