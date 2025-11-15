#!/bin/sh
g++ question-6-info.cpp -o question-6-info --std=c++20
./question-6-info
git add .
git commit -m "question 6 info"
git push -u origin main
g++ question-6.cpp -o question-6 --std=c++20
./question-6
git add .
git commit -m "question 6"
git push -u origin main
