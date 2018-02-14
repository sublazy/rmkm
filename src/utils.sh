#!/bin/bash

feed() {
    echo -e $@ > /dev/median
}

ans() {
    cat /dev/median
}

