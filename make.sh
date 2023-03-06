#!/bin/bash

g++ src/Client.cpp -o bin/Client -lpthread
g++ src/Server.cpp -o bin/Server -lpthread