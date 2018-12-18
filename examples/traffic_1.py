print """
# Traffic Model 1. Doesn't work.
# Four-way intersection. Four types of cars.
# Only one can go at a time. More than one go then collide.
# Decide order: 1 - 2 - 3 - 4. Then exeryone executes.
# Each car can decide to:
#	W: Wait
#	G: Go
# Each car can be in a state:
#	W: Wait
#	G: Go
#	R: Reached
#	C: Crashed
# 
# Initially, everyone is in W (Waiting) state.
# While in W, if only one car decides G (Go), it reaches R (Reached).
# If more than one cars decide G (Go), they reach C (Crashed).
# Once a car is in R or C, it stays there.
# We assume that if some cars crash this time step it doesn't affect
# next time step and other cars can reach successfully then.
#
# The combined state is written as 5 characters. First 4 characters
# correspond to states of 4 cars, last one specifies who makes the
# current move and it ranges from 0 to 4. 0: for the execution by
# system depending upon other moves, and it is fixed. In our model
# there is no system, so we assign the state to 1 but give only
# one option.
"""

S = {'W','G','R','C'}
states = []

for a in S:
	for b in S:
		for c in S:
			for d in S:
				for i in range(5):
					s = [a,b,c,d,str(i)]
					states.append(s)

print "\n~STATES~"
print "# Some of these states not useful. But adding for simplicity"
for s in states:
	print ''.join(s)

print "\n~PARTITIONS~"
print "# Player 1 & 0 (system default)"
print "{",
i = 4**4 * 2
for s in states:
	if s[4] == '0' or s[4] == '1':
		print ''.join(s),
		i -= 1
		if i > 0: print ',',
print "}"

for p in range(2,5):
	print "# Player %d" % p
	print "{",
	i = 4**4
	for s in states:
		if s[4] == str(p):
			print ''.join(s),
			i -= 1
			if i > 0: print ',',
	print "}"

print "\n~INITIAL STATE~"
print "WWWW1"

print "\n~ALPHABET~"
print "GO,WAIT"

print "\n~TRANSITION FUNCTION~"

for s in states:
	if s[4] == '0':
		t = s[:]
		t[4] = '1'
		count  = 0
		# Counting Go
		for i in range(4):
			if t[i] == 'G': count += 1
		# More than 1 cars Go
		if count > 1:
			for i in range(4):
				if t[i] == 'G': t[i] = 'C'
		# One car Go and reach
		if count == 1:
			for i in range(4):
				if t[i] == 'G': t[i] = 'R'
		print ''.join(s) + ' = * : ' + ''.join(t)

for p in range(1,5):
	print "# Player %d transition" % p
	for s in states:
		if s[4] == str(p):
			print ''.join(s) + ' =',
			t = s[:]
			t[4] = str ((p+1)%5)
			if s[p-1] == 'W':
				print 'WAIT : ' + ''.join(t),
				t[p-1] = 'G'
				print '| GO : ' + ''.join(t)
			else:
				print '* : ' + ''.join(t)

print "\n~ATOMIC PROPOSITIONS~"
print "# reach states one for each"
print "reach1,reach2,reach3,reach4"
AP = ['reach1','reach2','reach3','reach4']

print "\n~LABELING FUNCTION~"
for s in states:
	ok = []
	for i in range(4):
		if s[i] == 'R':
			ok.append(i)
	if len(ok) > 0:
		print ''.join(s) + ' =',
		for i in range(len(ok)):
			print AP[ok[i]],
			if i < len(ok)-1: print '|',
			else: print ''

