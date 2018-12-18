print """
# Traffic Model 8. (Combining Model 5 and 7)
# Now each car books a ticket each slot/iteration. 
# In the special last iteration, cars have veto to cancel
# all current bookings. We also add feature from model 5 to
# allow specific collisions and multiple cars can Go at a time.

# Car types:
# Going: (East, North, West, South)x(Left, Right, Straight)
# 		 (E,N,W,S)x(L,S)
# So types are: (EL, ES, ...)

"""

S = {'W','G','B','R','C'}
states = []
n_states = 5
n_book = 1	# Number of booking times. Used every slot.
n_slots = 2

n_cars = 0	# Number of cars. Used every slot.
car_X = ('E','N','W','S')
car_Y = ('L','S')

car_names = {}
for x in car_X:
	for y in car_Y:
		n_cars += 1
		car_names[x+y] = n_cars

collisions_names = [
('EL','SS'),
('EL','WL'),
('EL','SL'),
('EL','NL'),
('EL','WS'),
#
('ES','SS'),
('ES','WL'),
('ES','NL'),
('ES','NS'),
#
('NL','WL'),
('NL','SL'),
('NL','SS'),
#
('NS','SL'),
('NS','WL'),
('NS','WS'),
#
('WL','SL'),
#
('WS','SL'),
('WS','SS')
]

collisions = []
for col_n in collisions_names:
	collisions.append( {car_names[col_n[0]], car_names[col_n[1]]} )

def create_states(s, cars):
	if cars == n_cars:
		for i in range((n_cars+n_book)*n_slots + 1):
			s.append(str(i))
			states.append(s[:])
			del s[-1]
	else:
		for c in S:
			s.append(c)
			create_states(s, cars+1)
			del s[-1]

create_states([], 0)

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
init_s = ['W']*n_cars + ['0']
print ''.join(init_s)

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
				going = []
				# Counting Go
				for i in range(n_cars):
					if t[i] == 'G': going.append(i+1)

				# Check crashing
				crash = False
				for i in range(len(going)):
					for j in range(i+1,len(going)):
						if {going[i],going[j]} in collisions:
							crash = True

				# Collision occurs. 
				if crash:
					for i in range(n_cars):
						if t[i] == 'G': t[i] = 'C'
				else:
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
print "reach1,reach2,reach3,reach4,reach5"
AP = []
for i in range(1,n_cars+1):
	AP.append( 'reach' + str(i) )
	print 'reach' + str(i),
	if i < n_cars:
		print ',',
	else:
		print ''

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
