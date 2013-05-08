#!/usr/bin/env python
import random

random.seed(1234)

audio = []
impulse = []
for x in xrange(16):
	audio.append(random.randint(0,10))
	impulse.append(random.randint(0,10))

print "INPUT:\t", audio
print "IMPUL:\t", impulse

