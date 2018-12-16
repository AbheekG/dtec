#include "model.hh"

/*
Checking only reachability objectives.
Let G = ( S , P , s init , , ) be a game arena, and ( T i ) 1 ≤ i ≤ n be n subsets of S. Let
for the reachability objectives ( Reach ( T i )) 1 ≤ i ≤ n . Let s be the first state in outcome ( ) such that s ∈ i T i . Then every player has a
strategy from s, against all the other players, to reach his target set.
*/

set<int> intersect(const set<int>&, const set<int>&);

int Model::check () {

	// States from which a player can force reach its objective.
	vector< set<int> > force_reach_for_me (n_partitions);

	// States from which a player can force -- I reach or no-one reaches.
	vector< set<int> > force_i_or_no_one (n_partitions);

	// 


	// Eventually reach computation.
	for (int p = 0; p < n_partitions; ++p) {

		force_reach_for_me[p] = reach_set[p];
		set<int> old_reach;

		while (old_reach != force_reach_for_me[p]) {
			
			old_reach = force_reach_for_me[p];
			for (int s = 0; s < n_states; ++s) {
				
				if (force_reach_for_me[p].count(s) > 0) {
					continue;
				}

				bool good = false;
				if (partitions[p].count(s) > 0) {
					// My turn
					good = false;
					for (const auto & next_s : TF_list[s]) {
						if (force_reach_for_me[p].count(next_s) > 0) {
							good = true;
							// cout << "\nCase 1 insert due to : " << int_to_state[next_s] << endl;
							break;
						}
					}
				} else {
					// Other's turn.
					good = true;
					for (const auto & next_s : TF_list[s]) {
						if (force_reach_for_me[p].count(next_s) == 0) {
							good = false;
							break;
						}
					}
					// if (good) {
					// 	cout << "\nCase 2 insert due to : ";
					// 	for (const auto & next_s : TF_list[s]) cout << int_to_state[next_s] << " ";
					// 	cout << endl;
					// }
				}
				if (good) {
					force_reach_for_me[p].insert(s);
					// cout << "Inserting " << int_to_state[s] << " for player " << p << endl;
				}
			}
		}
		cout << "For player " << p << " I win set size = " << force_reach_for_me[p].size() << endl;
	}

	// Safety computation.
	// Computing the starting set.
	set<int> no_one_reached;
	for (int s = 0; s < n_states; ++s) {
		bool good = true;
		for (const auto &l : LF[s]) {
			if (l) good = false;
		}
		if (good) no_one_reached.insert(s);
	}

	for (int p = 0; p < n_partitions; ++p) {

		force_i_or_no_one[p] = force_reach_for_me[p];
		force_i_or_no_one[p].insert(no_one_reached.begin(), no_one_reached.end());
		set<int> old_reach;

		while (old_reach != force_i_or_no_one[p]) {
			
			old_reach = force_i_or_no_one[p];
			for (const auto& s : force_i_or_no_one[p]) {
				
				if (force_i_or_no_one[p].count(s) == 0) {
					continue;
				}

				bool good = false;
				if (partitions[p].count(s) > 0) {
					// My turn
					good = false;
					for (const auto & next_s : TF_list[s]) {
						if (force_i_or_no_one[p].count(next_s) > 0) {
							good = true;
							break;
						}
					}
				} else {
					// Other's turn.
					good = true;
					for (const auto & next_s : TF_list[s]) {
						if (force_i_or_no_one[p].count(next_s) == 0) {
							good = false;
							break;
						}
					}
				}
				if (not good) {
					force_i_or_no_one[p].erase(s);
					// cout << "Erasing " << int_to_state[s] << " for player " << p << endl;
				}
			}
		}

		cout << "For player " << p << " I or no-one wins set size = " << force_i_or_no_one[p].size() << endl;
	}


	// Everyone can force reach.
	// Essentially, intersection of individual force_reach_for_me
	set<int> everyone_can_force_reach;
	assert(n_partitions > 0);
	everyone_can_force_reach = force_reach_for_me[0];
	for (int p = 0; p < n_partitions; ++p) {
		everyone_can_force_reach = intersect(
			everyone_can_force_reach, force_reach_for_me[p]);
	}
	cout << "Everyone can force reach size = " << everyone_can_force_reach.size() << endl;

	// Everyone can reach or retaliate.
	// Essentially, intersection of individual force_i_or_no_one
	set<int> everyone_can_reach_or_retaliate;
	assert(n_partitions > 0);
	everyone_can_reach_or_retaliate = force_i_or_no_one[0];
	for (int p = 0; p < n_partitions; ++p) {
		everyone_can_reach_or_retaliate = intersect(
			everyone_can_reach_or_retaliate, force_i_or_no_one[p]);
	}
	cout << "Everyone can reach or retaliate size = " << everyone_can_reach_or_retaliate.size() << endl;


	// Now doing BFS to find path as given in second point of paper.
	vector<bool> visited(n_states);
	queue<int> next_states;
	next_states.push(init_state);

	int eq_exists = -1;
	while (not next_states.empty()) {
		int s = next_states.front();
		next_states.pop();
		cout << "Checking state = " << int_to_state[s] << endl;

		// Finding player.
		int player = -1;
		for (int p = 0; p < n_partitions; ++p) {
			if (partitions[p].count(s) > 0) {
				player = p;
			}
		}
		assert(player >= 0);

		if (everyone_can_force_reach.count(s) > 0 && reach_set[player].count(s) > 0) {
			eq_exists = s;
			break;
		} else if (everyone_can_reach_or_retaliate.count(s) > 0) {
			for (const auto& next_s : TF_list[s]) {
				cout << next_s << " $ ";
				next_states.push(next_s);
			}
		}
	}

	if (eq_exists < 0) {
		cout << "No equilibrium found!\n";
	} else {
		cout << "Hurray! Equibrium found at state = " << int_to_state[eq_exists] << endl;
	}

	return 0;
}

set<int> intersect(const set<int>& s1, const set<int>& s2) {
	set<int> s3;
	std::set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(),
		std::inserter(s3, s3.begin()));
	return s3;
}