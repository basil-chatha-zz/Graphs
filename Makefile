CXX = g++
CPPFLAGS = -g -Wall -std=c++11

# $@ : target name
# $< : first dependency
# $^ : entire dependency list

# -I*dir* where dir is relative directory path from where
# you run compile command

# all: search

search: search.cpp search.h
	$(CXX) $(CPPFLAGS) $^ -o $@

coloring: coloring.cpp search.h
	$(CXX) $(CPPFLAGS) $^ -o $@