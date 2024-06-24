#!/bin/sh -l

echo "Hello $1"
time=$(date)
touch a
touch b
echo "file1=./a" >> $GITHUB_OUTPUT
echo "file2=./b" >> $GITHUB_OUTPUT
echo "time=$time" >> $GITHUB_OUTPUT