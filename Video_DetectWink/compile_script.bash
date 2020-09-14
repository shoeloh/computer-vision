#!/bin/bash

g++ DetectWink_Video.cpp `pkg-config --cflags --libs opencv`
