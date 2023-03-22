#include "program3.h"
#include <vector>
#include <cmath>

using namespace std;

//Open addressing
vector<int> keys;
vector<int> values;
int currVal = 0;

int primaryHash = 0;
int secondaryHash = 0;
int collisions = INVALID_CALL;

// Sets the size of the hash table.  Returns if it succeeded.
bool SetHashTableSize(int n) {
	if (n > 0) {
		keys.clear();
		values.clear();

		keys.resize(n, -1);
		values.resize(n, -1);

		collisions = 0;
		return true;
	}
	return false;
}

// Sets the primary and secondary hash functions for hashing.  Returns if it succeeded.
bool SetHashFunctions(int f, int s) {
	//Secondary hash function must be one smaller than primary hash function so 0 is not step value.

	if ((f > 0 && f <= 5) && (s > 0 && s < 5)) {
		//Note that the primary hash will be modded by table size
		primaryHash = f;
		//Note that the secondary hash will be modded by one less than table size then increased by 1 when done
		secondaryHash = s;
		return true;
	}
	return false;
}

// Loads the data from the array into the hash table.  Returns the number of values added.
int LoadData(int a[ARRAY_SIZE]) {
	int addedVals = 0;
	for (int i = 0; i < ARRAY_SIZE; i++) {
		if (a[i] < 0) { break; }
		//Based on our primary and secondary hash values, we must add a[i] element to hashtable.
		//First calculate the key
		//RECALL PRIMARY HASH MOD TABLE SIZE
		currVal = a[i];
		switch (primaryHash) {
			//int(53*(0.2897k mod 1)) 
		case 1:
			if (AddValue(int(53 * (fmod(0.2897 * a[i], 1))) % keys.size()))
				addedVals++;
			break;
			//k mod n
		case 2:
			if (AddValue((a[i] % keys.size()) % keys.size()))
				addedVals++;
			break;
		case 3:
			if (AddValue((a[i] % 53) % keys.size()))
				addedVals++;
			break;
		case 4:
			if (AddValue((a[i] % 48) % keys.size()))
				addedVals++;
			break;
		case 5:
			if (AddValue(1))
				addedVals++;
			break;


		}
	}
	return addedVals;
}

// Adds the key to the hash table.  Returns if it succeeded.
bool AddValue(int key) {
	int totalAddCollides = 0;

	if (key < 0) {
		return false;
	}

	if (keys.size() <= 0 ) {
		return INVALID_CALL;
	}

	while (key >= keys.size()) {
		key -= keys.size();
	}
	//RECALL SECONDARY HASH MOD TABLESIZE-1 AND INCREASE 1 AT END
	switch (secondaryHash) {
		//int(52*(0.2897kmod1)) STEP
	case 1:
		//Key is the index value of the value array
		if (keys.at(key) == -1) {
			keys.at(key) = key;
			values.at(key) = currVal;
		}
		else {
			//Collision detected :(
			//Move our position with appropriate step.
			while (keys.at(key) != -1) {
				key += ((int(52 * fmod(0.2897 * currVal, 1))) % keys.size() - 1) + 1;
				if (key >= keys.size())
					key -= keys.size();
				collisions++;
				totalAddCollides++;
				if (totalAddCollides >= MAX_COLLISIONS)
					return false;
			}
			keys.at(key) = key;
			values.at(key) = currVal;
		}
		return true;
		//k mod (n-1) STEP
	case 2:
		if (keys.at(key) == -1) {
			keys.at(key) = key;
			values.at(key) = currVal;
		}
		else {
			//Collision detected :(
			//Move our position with appropriate step.
			while (keys.at(key) != -1) {
				key += ((currVal % (keys.size() - 1)) % keys.size() - 1) + 1;
				if (key >= keys.size())
					key -= keys.size();
				collisions++;
				totalAddCollides++;
				if (totalAddCollides >= MAX_COLLISIONS)
					return false;
			}
			keys.at(key) = key;
			values.at(key) = currVal;
		}
		return true;
		//k mod 52 STEP
	case 3:
		if (keys.at(key) == -1) {
			keys.at(key) = key;
			values.at(key) = currVal;
		}
		else {
			//Collision detected :(
			//Move our position with appropriate step.
			while (keys.at(key) != -1) {
				key += ((currVal % 52) % keys.size() - 1) + 1;
				if (key >= keys.size())
					key -= keys.size();
				collisions++;
				totalAddCollides++;
				if (totalAddCollides >= MAX_COLLISIONS)
					return false;
			}
			keys.at(key) = key;
			values.at(key) = currVal;
		}
		return true;
	case 4:
		if (keys.at(key) == -1) {
			keys.at(key) = key;
			values.at(key) = currVal;
		}
		else {
			//Collision detected :(
			//Move our position with appropriate step.
			while (keys.at(key) != -1) {
				key += ((currVal % 47) % keys.size() - 1) + 1;
				if (key >= keys.size())
					key -= keys.size();
				collisions++;
				totalAddCollides++;
				if (totalAddCollides >= MAX_COLLISIONS)
					return false;
			}
			keys.at(key) = key;
			values.at(key) = currVal;
		}
		return true;
	default:
		return false;
	}
}

// Removes the key from the hash table.  Returns the index that the key was stored in.
int RemoveValue(int key) {
	//If the SetHashTableSize was never called, return INVALID_CALL
	if (values.size() == 0) {
		return INVALID_CALL;
	}
	if (key < 0) {
		return false;
	}

	while (key >= keys.size()) {
		key -= keys.size();
	}

	int locationOfKey = NO_LOCATION;
	int totalRemoveCollides = 0;
	//RECALL SECONDARY HASH MOD TABLESIZE-1 AND INCREASE 1 AT END
	switch (secondaryHash) {
		//int(52*(0.2897kmod1)) STEP
	case 1:
		if (keys.at(key) != -1) {
			locationOfKey = GetHashTableLocation(key);
			keys.at(locationOfKey) = -1;
			values.at(locationOfKey) = -1;
			return locationOfKey;
		}
		else {
			//Collision detected :(
			//Move our position with appropriate step.
			while (keys.at(key) == -1) {
				key += ((int(52 * fmod(0.2897 * currVal, 1))) % keys.size() - 1) + 1;
				if (key >= keys.size())
					key -= keys.size();
				totalRemoveCollides++;
				if (totalRemoveCollides >= MAX_COLLISIONS)
					return NO_LOCATION;
			}
			locationOfKey = GetHashTableLocation(key);
			keys.at(locationOfKey) = -1;
			values.at(locationOfKey) = -1;
			return locationOfKey;
		}
		//k mod (n-1) STEP
	case 2:
		if (keys.at(key) != -1) {
			locationOfKey = GetHashTableLocation(key);
			keys.at(locationOfKey) = -1;
			values.at(locationOfKey) = -1;
			return locationOfKey;
		}
		else {
			//Collision detected :(
			//Move our position with appropriate step.
			while (keys.at(key) == -1) {
				key += ((currVal % (keys.size() - 1)) % keys.size() - 1) + 1;
				if (key >= keys.size())
					key -= keys.size();
				totalRemoveCollides++;
				if (totalRemoveCollides >= MAX_COLLISIONS)
					return NO_LOCATION;
			}
			locationOfKey = GetHashTableLocation(key);
			keys.at(locationOfKey) = -1;
			values.at(locationOfKey) = -1;
			return locationOfKey;
		}
		//k mod 52 STEP
	case 3:
		if (keys.at(key) != -1) {
			locationOfKey = GetHashTableLocation(key);
			keys.at(locationOfKey) = -1;
			values.at(locationOfKey) = -1;
			return locationOfKey;
		}
		else {
			//Collision detected :(
			//Move our position with appropriate step.
			while (keys.at(key) == -1) {
				key += ((currVal % 52) % keys.size() - 1) + 1;
				if (key >= keys.size())
					key -= keys.size();
				totalRemoveCollides++;
				if (totalRemoveCollides >= MAX_COLLISIONS)
					return NO_LOCATION;
			}
			locationOfKey = GetHashTableLocation(key);
			keys.at(locationOfKey) = -1;
			values.at(locationOfKey) = -1;
			return locationOfKey;
		}
	case 4:
		if (keys.at(key) != -1) {
			locationOfKey = GetHashTableLocation(key);
			keys.at(locationOfKey) = -1;
			values.at(locationOfKey) = -1;
			return locationOfKey;
		}
		else {
			//Collision detected :(
			//Move our position with appropriate step.
			while (keys.at(key) == -1) {
				key += ((currVal % 47) % keys.size() - 1) + 1;
				if (key >= keys.size())
					key -= keys.size();
				totalRemoveCollides++;
				if (totalRemoveCollides >= MAX_COLLISIONS)
					return NO_LOCATION;
			}
			locationOfKey = GetHashTableLocation(key);
			keys.at(locationOfKey) = -1;
			values.at(locationOfKey) = -1;
			return locationOfKey;
		}
	default:
		return NO_LOCATION;
	}
}

// Finds the key in the hash table and returns its index.
int GetHashTableLocation(int key) {
	if (values.size() == 0) {
		return INVALID_CALL;
	}
	for (int i = 0; i < keys.size(); i++) {
		if (keys.at(i) == key)
			return i;
	}
	return NO_LOCATION;
}

// Returns the number of collisions that have occurred since the hash table was set up.
int GetNumberOfCollisions() {
	return collisions;
}
