#include "Hashmap.h"
unsigned int Hashmap::hash(string key) const
{
	unsigned int int_key = (2*BUCKETS)/3;
	for(char c : key)
	{
		int_key *= 2;
		int_key += int(c);
		while(int_key >= BUCKETS)
			int_key = int_key % BUCKETS + ((int_key - int_key % BUCKETS)/BUCKETS);
	}
	return int_key;
}

Hashmap::Hashmap()
{
	for(int i = 0; i < BUCKETS; i++)
		buckets[i] = nullptr;
	mapSize = 0;
}

Hashmap::~Hashmap()
{
	clear();
}

void Hashmap::insert(string key, int value)
{
	int hashed_index = hash(key);

	// Empty bucket
	if(buckets[hashed_index] == nullptr)
	{
		buckets[hashed_index] = newNode(key, value);
		return;
	}

	// Non-empty buckets
	for(Node* this_node = buckets[hashed_index]; this_node != nullptr; this_node = this_node->next)
	{
		if(this_node->key == key)
		{
			this_node->value = value;
			return;
		}

		if(this_node->next == nullptr)
		{
			this_node->next = newNode(key, value);
			this_node->next->prev = this_node;
			return;
		}
	}
	throw runtime_error("Unexpected behavior: value neither added nor updated in Hashmap::insert");
}

Hashmap::Node* Hashmap::newNode(string key, int value)
{
	mapSize++;
	Node* temp_node = new Node();
	temp_node->key = key;
	temp_node->value = value;
	return temp_node;
}

bool Hashmap::contains(string key) const
{
	int hashed_index = hash(key);

	for(Node* this_node = buckets[hashed_index]; this_node != nullptr; this_node = this_node->next)
		if(this_node->key == key)
			return true;

	return false;
}

int Hashmap::get(string key) const
{
	int hashed_index = hash(key);

	for(Node* this_node = buckets[hashed_index]; this_node != nullptr; this_node = this_node->next)
		if(this_node->key == key)
			return this_node->value;

	throw invalid_argument(key + " not found in hashmap");
}

int& Hashmap::operator [](string key)
{
	int hashed_index = hash(key);

	// Empty bucket
	if(buckets[hashed_index] == nullptr)
	{
		buckets[hashed_index] = newNode(key, 0);
		return buckets[hashed_index]->value;
	}

	// Non-empty bucket
	for(Node* this_node = buckets[hashed_index]; this_node != nullptr; this_node = this_node->next)
	{
		if(this_node->key == key)
			return this_node->value;

		if(this_node->next == nullptr)
		{
			this_node->next = newNode(key, 0);
			this_node->next->prev = this_node;
		}
	}
	throw runtime_error("Unexpected behavior: value neither found nor added in Hashmap::operator[]");
}

bool Hashmap::remove(string key)
{
	int hashed_index = hash(key);
	for(Node* this_node = buckets[hashed_index]; this_node != nullptr; this_node = this_node->next)
	{
		if(this_node->key == key)
		{
			((this_node->prev == nullptr) ? buckets[hashed_index] : this_node->prev->next) = this_node->next;
			if(this_node->next != nullptr)
				this_node->next->prev = this_node->prev;

			mapSize--;
			delete this_node;
			return true;
		}
	}

	return false;
}

void Hashmap::clear()
{
	for(int i = 0; i < BUCKETS; i++)
	{
		Node* this_node = buckets[i];
		while(this_node != nullptr)
		{
			Node* temp_node = this_node;
			this_node = this_node->next;
			delete temp_node;
		}
		buckets[i] = nullptr;
	}
	mapSize = 0;
}

string Hashmap::toString() const
{
	stringstream output;
	for(int i = 0; i < BUCKETS; i++)
	{
		output << "[" << i << "]";
		for(Node* this_node = buckets[i]; this_node != nullptr; this_node = this_node->next)
			output << " " << this_node->key << " => " << this_node->value;
		output << endl;
	}
	return output.str();
}

int Hashmap::size() const
{
	return mapSize;
}

string Hashmap::toSortedString() const
{
	stringstream output;

	priority_queue<Node*, vector<Node*>, NodeCompare> nodeHeap;
	for(int i = 0; i < BUCKETS; i++)
		for(Node* this_node = buckets[i]; this_node != nullptr; this_node = this_node->next)
			nodeHeap.push(this_node);

	while(!nodeHeap.empty())
	{
		Node* this_node = nodeHeap.top();
		output << this_node->key << " => " << this_node->value << endl;
		nodeHeap.pop();
	}

	return output.str();
}
