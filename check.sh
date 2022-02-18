#!/bin/bash

SRC="src"
TST="test"

function header() {
    local BAR="+------------------+"
    echo $BAR
    echo "| "$1
    echo $BAR
}

function check() {
    local INC="include"
    local INCALG=$INC"/alglib"
    cppcheck -I$INC -I$INCALG \
        -UAE_INT32_T -UAE_INT64_T -UAE_UINT64_T -UAE_NWORKERS \
        --platform=unix64 \
        --check-config \
        --enable=all \
        --std=c++11 \
        --suppress=missingIncludeSystem \
        $1 -I $1
}

header "CHECKING PROJECT"
check $SRC
header "CHECKING TESTS"
check $TST
