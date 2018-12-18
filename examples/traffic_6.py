print """
# Traffic Model 6. (Booking tickets to Go.)
# The difference from Model 2 is that now we have extra
# B (Booked) state.
#
# Four-way intersection. Four types of cars.
# Only one can go at a time. More than one go then collide.
# Decide order: 1 - 2 - 3 - 4. Then book ticket for current slot.
# There can be multiple slots.
# After a few book ticket iterations. Execution happens.
# Each car can decide to:
#	W: Wait
#	G: Go
# Each car can be in a state:
#	W: Wait
#	G: Go
#	R: Reached
#	C: Crashed
#	B: Booked (Booked a slot for going)
# 
# Initially, everyone is in W (Waiting) state. Between each booking,
# while in W, if only one car decides G (Go), it reaches B (Booked).
# If more than one cars decide G (Go), they reach C (Crashed).
# Once a car is in R or C, it stays there.
# After some booking iterations, there is an execute step.
# We assume that if some cars crash current time step it doesn't affect
# next time step and other cars can reach successfully then.
#
# The combined state is written as 5 characters. First 4 characters
# correspond to states of 4 cars, last one specifies who makes the
# current move. It can be one of the four players/cars
# or the booking system or the execution system.
"""

S = {'W','G','B','R','C'}
states = []
n_states = 5
n_cars = 4	# Number of cars. Used every slot.
n_book = 1	# Number of booking times. Used every slot.
n_slots = 4

for a in S:
	for b in S:
		for c in S:
			for d in S:
				for i in range((n_cars+n_book)*n_slots + 1):
					s = [a,b,c,d,str(i)]
					states.append(s)

print "\n~STATES~"
print "# Some of these states not useful. But adding for completeness."
for s in states:
	print ''.join(s)

print "\n~PARTITIONS~"
for p in range(1,n_cars+1):
	print "# Player %d" % p
	print "{",
	i = n_states**n_cars
	i *= n_slots*(1+int(p == n_cars)*n_book) + int(p == n_cars)
	for s in states:
		in_partition = ((int(s[n_cars])-1)%(n_cars+n_book)+1 == p)
		in_partition |= ( int(p == n_cars) 
			and (int(s[n_cars])-1)%(n_cars+n_book)+1 >= p  )
		if in_partition:
			print ''.join(s),
			i -= 1
			if i > 0: print ',',
	print "}"


print "\n~INITIAL STATE~"
print "WWWW0"

print "\n~ALPHABET~"
print "GO,WAIT"

print "\n~TRANSITION FUNCTION~"
print "# For execution slot 0. Booked cars reach."
for s in states:
	if s[n_cars] == '0':
		t = s[:]
		t[n_cars] = '1'
		# One car Go and reach
		for i in range(n_cars):
			if t[i] == 'B': t[i] = 'R'
		print ''.join(s) + ' = * : ' + ''.join(t)

print "# For booking execution slots. Too many Gs"
print "# then crash (C), otherwise booked (B)"
for p in range(n_cars+1,n_cars+n_book+1):
	print "# Booking %d transition" % p
	for s in states:
		if (int(s[n_cars])-1)%(n_cars+n_book)+1 == p and s[n_cars] != '0':
			t = s[:]
			t[n_cars] = str ( (int(s[n_cars])+1)%((n_cars+n_book)*n_slots+1) )
			count  = 0
			# Counting Go
			for i in range(n_cars):
				if t[i] == 'G': count += 1
			# More than 1 cars Go
			if count > 1:
				for i in range(n_cars):
					if t[i] == 'G': t[i] = 'C'
			# One car can get booked
			if count == 1:
				for i in range(n_cars):
					if t[i] == 'G': t[i] = 'B'
			print ''.join(s) + ' = * : ' + ''.join(t)

print "# For Cars"
for p in range(1,n_cars+1):
	print "# Player %d transition" % p
	for s in states:
		if (int(s[n_cars])-1)%(n_cars+n_book)+1 == p and s[n_cars] != '0':
			print ''.join(s) + ' =',
			t = s[:]
			t[n_cars] = str ( (int(s[n_cars])+1)%((n_cars+n_book)*n_slots+1) )
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
	for i in range(n_cars):
		if s[i] == 'R' and s[n_cars] == '0':
			ok.append(i)
	if len(ok) > 0:
		print ''.join(s) + ' =',
		for i in range(len(ok)):
			print AP[ok[i]],
			if i < len(ok)-1: print '|',
			else: print ''
