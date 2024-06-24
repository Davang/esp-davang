#!/bin/sh -l

echo "Hello $1"
time=$(date)
touch a
echo "file=./a" >> $GITHUB_OUTPUT
echo "time=$time" >> $GITHUB_OUTPUT