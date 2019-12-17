#!/bin/sh

l32-asm test.s && l32-link test.o && l32-dis a.out
