#!/usr/bin/env python3
# Task ventilator
# Binds PUSH socket to tcp://localhost:5557
# Sends batch of tasks to workers via that socket
#
# Author: Lev Givon <lev(at)columbia(dot)edu>
import zmq
import random
import time
from zmq import *

print ("Using ZMQ " + zmq_version())

# a ZMQ context must be created, everything is tied to this context
context = zmq.Context()

# Socket to send messages on, workers/consumers will connect to this
sender = context.socket(zmq.PUSH) # specify the type of the socket (PUSH)
sender.bind("tcp://*:5557") # ZMQ is smart, it figures out the protocol

# Socket with direct access to the sink: used to syncronize start of batch
sink = context.socket(zmq.PUSH)
sink.connect("tcp://localhost:5558") # Connect to the sink

print ("Press Enter when the workers are ready: ")
_ = input()
print ("Sending tasks to workers...")

# The first message is "0" and signals start of batch
sink.send('0'.encode('ascii'))

# Initialize random number generator
random.seed()

# Send 100 tasks
total_msec = 0

for task_nbr in range(100):
	# Random workload from 1 to 100 msecs
	workload = random.randint(1, 100)
	total_msec += workload
	sender.send((str(workload)).encode('ascii'))

print ("Total expected cost: %s msec" % total_msec)

# Give 0MQ time to deliver
time.sleep(1)
