#!/bin/bash

RAIZ=`dirname $0`
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$RAIZ/bin/release

$RAIZ/bin/release/Tonatiuh