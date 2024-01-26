#!/bin/bash

for (( i = 0; i <= 9; ++i )); do
    diff "pplot0${i}.ps" "plot0${i}.ps"
done

for (( i = 0; i <= 9; ++i )); do
    diff "pplot1${i}.ps" "plot1${i}.ps"
done
