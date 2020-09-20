#!/bin/bash

cp mem.circ tests/circ_files
cp ../part1/alu.circ alu.circ 
cp alu.circ tests/circ_files
cp ../part1/regfile.circ regfile.circ
cp regfile.circ tests/circ_files
cp cpu.circ tests/circ_files
cd tests/circ_files
./sanity_test.py
cd ../..
