Doomsday-Threatening Equilibrium Checker (DTEC)
===

See the `report` folder for details about the project. Source code is in `src` folder, and the examples are in `examples` folder.

### Compile ###
Go inside `src` directory, then:
~~~
make
~~~

### Usage ###
The compiled program is in `src` directory.

##### 1. Help
Inside `src` directory:
~~~
./dtec -h
~~~

##### 2. Run
Inside `src` directory:
~~~
./dtec -m <path to model file> -s <path to specification file>
~~~
or
~~~
./dtec -s <path to specification file> -m <path to model file> 
~~~

For example, when running traffic example 7,
~~~
./dtec -s ../examples/traffic.specs  -m ../examples/traffic_7.model
~~~

##### 3. How to write the .model and .specs files?
Please see the hello_world example.

`src/hello_world.model`
~~~
# This is a comment.

~STATES~
# States need to be seperated by a comma or a new-line.
# One-dimensional arrays allowed.
# Negative range allowed.

one_state
many_states[1..3]
another_many_states[-1..1]	# Can have negative index too.

~PARTITIONS~
# Each partition starts with { and ends with }
# Write partitions seperated by comma ',' or one in a line
# All examples below work.

{one_state, many_states[1] }
{
many_states[2],another_many_states[-1]
},{
many_states[3]
another_many_states[0..1]
}

~INITIAL STATE~
one_state

~ALPHABET~
# Rules exactly like the states.
a,b
c[1..2]

~TRANSITION FUNCTION~
# Note that this is a function and has its properties.
# * can be used to specify default behavior.

one_state = a : many_states[1] | b : many_states[2] | c[1] : many_states[3] | c[2] : another_many_states[0]
many_states[1] = * : many_states[2]
many_states[2] = * : many_states[3]
many_states[3] = * : one_state
another_many_states[0] = a : another_many_states[-1] | b : another_many_states[1] | * : another_many_states[0]
another_many_states[-1] = * : one_state
another_many_states[1] = * : one_state

~ATOMIC PROPOSITIONS~
# Define like the ~STATES~
win,lose

~LABELING FUNCTION~
# Format. state = labels seperated by seperator '|'
one_state = win | lose
another_many_states[-1] = lose
another_many_states[1] = win

~~~

`src/hello_world.specs`
~~~
# We are only implementing reachability objectives.
# Specify similar to partitions.
# The set of propositions for each player starts 
# with { and ends with }.
# Write each set seperated by comma ',' or one in a line
# It should match number of players in model

~REACHABILITY~
{win,lose}
{win,~lose}
{~win,lose}
~~~
