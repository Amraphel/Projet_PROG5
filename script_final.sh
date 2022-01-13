#!/bin/bash

if [ $# -eq 1 ]
then
    if [ $1 -lt 0 ] && [ $1 -gt 5 ]
    then
        echo "Argument inconnu"
    else
        if [ $1 -eq 1 ] || [ $1 -eq 0 ]
        then
        echo -e "test_phase1_part1\n"
        ./test_phase1_part1
        fi

        if [ $1 -eq 2 ] || [ $1 -eq 0 ]
        then
        echo -e "\ntest_phase1_part2\n"
        ./test_phase1_part2
        fi

        if [ $1 -eq 4 ] || [ $1 -eq 0 ]
        then
        echo -e "\ntest_phase1_part4\n"
        ./test_phase1_part4
        fi
    fi
else
    echo "Nombre d'arguments incorrects"
fi