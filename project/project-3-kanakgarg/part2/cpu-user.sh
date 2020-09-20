#!/bin/bash

cp mem.circ my_tests/circ_files
cp ../part1/alu.circ alu.circ 
cp ../part1/regfile.circ regfile.circ
cp alu.circ my_tests/circ_files
cp regfile.circ my_tests/circ_files
cp cpu.circ my_tests/circ_files
cd my_tests/circ_files
./modular_test.py
cd ../..
