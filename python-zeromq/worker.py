#!/usr/bin/env python3
# Task worker
# Connects PULL socket to tcp://localhost:5557
# Collects workloads from ventilator via that socket
# Connects PUSH socket to tcp://localhost:5558
# Sends results to sink via that socket
#
# Author: Lev Givon <lev(at)columbia(dot)edu>
import sys
import time
import zmq

context = zmq.Context()

# Initiate two connections:
#   1. To the ventilator 
#   2. To the sink
#
# With this architecture, any number of workers can be spawned
# without any modification to code, as long as the ports for the
# ventilator and sink remain the same

# Socket to receive messages on
receiver = context.socket(zmq.PULL)
receiver.connect("tcp://localhost:5557")

# Socket to send messages to
sender = context.socket(zmq.PUSH)
sender.connect("tcp://localhost:5558")


# Process tasks forever
while True:
    s = receiver.recv()
    # Simple progress indicator for the viewer
    sys.stdout.write("received " + str(s) + "\n")
    sys.stdout.flush()
    # Do the work
    time.sleep(int(s)*0.001)
    # Send results to sink
    sender.send((str(int(s))).encode('ascii'))
