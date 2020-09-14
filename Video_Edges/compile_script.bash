#!/bin/bash

g++ video_edges.cpp `pkg-config --cflags --libs opencv`
