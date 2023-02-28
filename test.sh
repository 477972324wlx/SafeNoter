#!/bin/bash
rm test/*
for i in {1..10}
do
    printf "%dbcde\n" $i > test/num${i}
done