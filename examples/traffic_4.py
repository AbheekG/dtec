print """
# Traffic Model 4. Doesn't work.
# The difference from Model 2 is that can move from G to W also.
"""

S = {'W','G','R','C'}
states = []
n_states = 4
n_players = 4
n_times = 4

for a in S:
	for b in S:
		for c in S:
			for d in S:
				for i in range(n_players*n_times + 1):
					s = [a,b,c,d,str(i)]
					states.append(s)

print "\n~STATES~"
print "# Some of these states not useful. But adding for simplicity."
for s in states:
	print ''.join(s)

print "\n~PARTITIONS~"
for p in range(1,n_players+1):
	print "# Player %d" % p
	print "{",
	i = n_states**n_players * (n_times+((-1)%n_players+1 == p))
	for s in states:
		if (int(s[n_players])-1)%n_players+1 == p:
			print ''.join(s),
			i -= 1
			if i > 0: print ',',
	print "}"

print "\n~INITIAL STATE~"
print "WWWW0"

print "\n~ALPHABET~"
print "GO,WAIT"

print "\n~TRANSITION FUNCTION~"

for s in states:
	if s[n_players] == '0':
		t = s[:]
		t[n_players] = '1'
		count  = 0
		# Counting Go
		for i in range(n_players):
			if t[i] == 'G': count += 1
		# More than 1 cars Go
		if count > 1:
			for i in range(n_players):
				if t[i] == 'G': t[i] = 'C'
		# One car Go and reach
		if count == 1:
			for i in range(n_players):
				if t[i] == 'G': t[i] = 'R'
		print ''.join(s) + ' = * : ' + ''.join(t)

for p in range(1,n_players+1):
	print "# Player %d transition" % p
	for s in states:
		if (int(s[n_players])-1)%n_players+1 == p and s[n_players] != '0':
			print ''.join(s) + ' =',
			t = s[:]
			t[n_players] = str ( (int(s[n_players])+1)%(n_players*n_times+1) )
			if s[p-1] == 'W' or s[p-1] == 'G':
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
	for i in range(n_players):
		if s[i] == 'R':
			ok.append(i)
	if len(ok) > 0:
		print ''.join(s) + ' =',
		for i in range(len(ok)):
			print AP[ok[i]],
			if i < len(ok)-1: print '|',
			else: print ''
