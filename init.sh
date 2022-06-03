#! /bin/bash
echo '[INIT] Creating base directory...'

mkdir -p ./$1/log
mkdir -p ./$1/ped/validFiles
mkdir -p ./$1/plots/Integral/png
mkdir -p ./$1/plots/Integral/svg
mkdir -p ./$1/plots/Peak/png
mkdir -p ./$1/plots/Peak/svg
mkdir -p ./$1/validFiles/Integral
mkdir -p ./$1/validFiles/Peak