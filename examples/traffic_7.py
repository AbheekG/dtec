print """
# Traffic Model 7. (Booking tickets to Go, with Veto cancellation.)
# The difference from Model 6 is that now we have extra
# round where each voter has a veto and can cancel execution.
#
# Four-way intersection. Four types of cars.
# Only one can goes at a time. More than one goes then collide.
# Decide order: 1 - 2 - 3 - 4. Then book ticket for current slot.
# There can be multiple slots.
# After a few book ticket iterations, first Veto, then Execution happens.
# Each car can decide to:
#	W: Wait
#	G: Go (Or Veto in special iteration)
# Each car can be in a state:
#	W: Wait
#	G: Go (Or Veto in special iteration)
#	R: Reached
#	C: Crashed
#	B: Booked (Booked a slot for going)
# 
# Initially, everyone is in W (Waiting) state. Between each booking,
# while in W, if only one car decides G (Go), it reaches B (Booked).
# If more than one cars decide G (Go), they reach C (Crashed).
# Once a car is in R or C, it stays there.
# After some booking iterations, there is an execute step.
# Just before execute, any car can veto and cancel everything.
# We assume that if some cars crash in current time step it doesn't affect
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
n_slots = 5

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
		# Count Veto, if any cancel all bookings.
		count_veto = 0
		for i in range(n_cars):
			if t[i] == 'G': count_veto += 1
		if count_veto == 0: next_s = 'R'
		else: next_s = 'W'
		# Cars Go and reach or reset to W
		for i in range(n_cars):
			if t[i] == 'B' or t[i] == 'G': t[i] = next_s
		print ''.join(s) + ' = * : ' + ''.join(t)

print "# For booking execution slots. Too many Gs"
print "# then crash (C), otherwise booked (B)"
for p in range(n_cars+1,n_cars+n_book+1):
	print "# Booking %d transition" % p
	for s in states:
		if (int(s[n_cars])-1)%(n_cars+n_book)+1 == p and s[n_cars] != '0':
			t = s[:]
			t[n_cars] = str ( (int(s[n_cars])+1)%((n_cars+n_book)*n_slots+1) )
			
			# Do nothing last slot/iteration
			if (int(s[n_cars])-1)/(n_cars+n_book) < n_slots-1:
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
			else:
				print ''.join(s) + ' = * : ' + ''.join(t)


print "# For Cars"
for p in range(1,n_cars+1):
	print "# Player %d transition" % p
	for s in states:
		if (int(s[n_cars])-1)%(n_cars+n_book)+1 == p and s[n_cars] != '0':
			print ''.join(s) + ' =',
			t = s[:]
			t[n_cars] = str ( (int(s[n_cars])+1)%((n_cars+n_book)*n_slots+1) )
			# Normal booking slot/iteration
			if (int(s[n_cars])-1)/(n_cars+n_book) < n_slots-1:
				if s[p-1] == 'W':
					print 'WAIT : ' + ''.join(t),
					t[p-1] = 'G'
					print '| GO : ' + ''.join(t)
				else:
					print '* : ' + ''.join(t)
			# Veto slot/iteration
			else:
				print 'WAIT : ' + ''.join(t),
				t[p-1] = 'G'
				print '| GO : ' + ''.join(t)
			

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
