#!/bin/bash

SOURCE_CODE := direct-mapped.c
TEST_FILE := gcc_ld_trace.txt

build:
	gcc -Wno-all -o test $(SOURCE_CODE)
	./test ./cache_sample/$(TEST_FILE)
