#!/bin/bash

g++ video_read.cpp `pkg-config --cflags --libs opencv`
