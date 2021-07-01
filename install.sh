#!/bin/sh

mkdir build && cd build && cmake .. && make;
cp  ../src/NeuralNetwork/OCR/Data ../Save.txt ../Text . -r
