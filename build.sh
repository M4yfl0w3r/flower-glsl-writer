#!/bin/bash 

OPT=$1

xmake f -m debug
if xmake build; then
    if [[ $OPT == test ]] then
        xmake run test 
    else
        xmake run app
    fi
fi
