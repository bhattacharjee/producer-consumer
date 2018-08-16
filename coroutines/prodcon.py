#!/usr/bin/env python3

def consumer():
    while True:
        input = (yield)
        if (None != input):
            print (input)
        if (input == -1):
            break


def producer():
    x = consumer()
    for i in range(10):
        x.__next__()
        x.send(i)

producer()
