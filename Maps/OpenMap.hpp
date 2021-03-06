#include <iostream>
#include <string>
#include <cmath>

#ifndef OPENMAP_HPP_
#define OPENMAP_HPP_
#include <stdexcept>
#include "Dictionary.hpp"
#include "vector.hpp"
using std::pair;
using std::string;
using std::to_string;

#ifndef PRIME_CHECK_
#define PRIME_CHECK_
bool isPrime(long long int n)
{
	for(int i=2; i<=sqrt(n); i++)
		if(n%i==0)
			return false;
	return true;
}
#endif

namespace shukla
{
    template<typename Key, typename Value>
    class OpenMap  : public Dictionary<Key,Value>
    {
        void rehash();
        vector<pair<Key,Value> >* m;
        bool* marker;
        int size;
        int capacity;
        int Hash(const Key& key);
    public:
        OpenMap();
        OpenMap(const int& num);
        OpenMap(OpenMap<Key, Value>& ht);
        ~OpenMap();
        void printMap();
        Value& operator[](const Key& key);
        virtual bool has(const Key& key);
        virtual void remove(const Key& key);
        virtual Value get(const Key& key);
        virtual void put(const Key& key, const Value& value);
    };
}

using namespace shukla;

template <typename Key,typename Value>
void OpenMap<Key,Value>::printMap()
{
	std::cout<<"TableIndex\tKey\n";
	for(int i=0; i<capacity	; i++)
		if(m[i].size()==1)
			std::cout<<"  "<<i<<"\t\t  "<<m[i][0].first<<"\n";
}

template <typename Key,typename Value>
void OpenMap<Key,Value>::rehash()
{
	int origSize = capacity;
	capacity *= 2;
	while(!isPrime(capacity)) capacity++;
	vector<pair<Key,Value> >* reMap;
    reMap = new vector<pair<Key,Value> >[capacity];

	for(int i=0; i<origSize; i++)
	{
		if(m[i].size()==1)
		{
			pair<Key,Value> it = m[i][0];

			int h = Hash(it.first);
			while(reMap[h].size()!=0)
				h = (h+1)%capacity;
			reMap[h].push_back(it);
		}
	}
	m = reMap;
}

template <typename Key,typename Value>
int OpenMap<Key,Value>::Hash(const Key& key)
{
	long long int SEED = 2166136261;
	long long int PRIME = 16777619;
    
    string k = to_string(key);
    long long int hash = SEED;
    for(int i=0;i<k.size();i++)
    	hash = (((k.c_str()[i]-'0')^hash)*PRIME)%capacity;
    return hash;
}

template <typename Key,typename Value>
OpenMap<Key,Value>::OpenMap()
{
	size = 0;
    capacity = 101;
    m = new vector<pair<Key,Value> >[capacity];
    marker = new bool[capacity];
}

template <typename Key,typename Value>
OpenMap<Key,Value>::OpenMap(const int& num)
{
	size = 0;
    capacity = num;
    m = new vector<pair<Key,Value> >[capacity];
    marker = new bool[capacity];
}

template <typename Key,typename Value>
OpenMap<Key,Value>::OpenMap(OpenMap<Key,Value>& ht)
{
    size = ht.size;
    capacity = ht.capacity;
    m = new vector<pair<Key,Value> >[capacity];
    marker = new bool[capacity];
    for(int i=0; i<capacity; i++)
    {
    	m[i] = ht.m[i];
    	marker[i] = ht.marker[i];
    }
}

template <typename Key,typename Value>
OpenMap<Key,Value>::~OpenMap()
{
    delete [] m;
}

template <typename Key,typename Value>
Value& OpenMap<Key,Value>::operator[](const Key& key)
{
	int h = Hash(key);
	while(m[h].size()!=0 || (m[h].size()==0 && marker[h]))
	{
		if(m[h].size()!=0 && m[h][0].first==key)
			return m[h][0].second;
		else if(m[h].size()==0 && marker[h])
		{
			Value temp;
			m[h].push_back({key,temp});
			size++;
			Value* ans = &m[h][0].second;
			return *ans;		
		}
		h = (h+1)%capacity;
	}
	Value temp;
	m[h].push_back({key,temp});
	size++;
	Value* ans = &m[h][0].second;
	if(size == capacity)
		rehash();
	return *ans;
}

template <typename Key,typename Value>
void OpenMap<Key,Value>::put(const Key& key, const Value& value)
{
	int h = Hash(key);
	while(m[h].size()!=0)
		h = (h+1)%capacity;
	m[h].push_back({key,value});
	size++;
	if(size == capacity)
		rehash();
}

template <typename Key,typename Value>
bool OpenMap<Key,Value>::has(const Key& key)
{
	int orig = Hash(key);
	if (m[orig].size()==0) return false;
	else if(m[orig][0].first==key) return true;
	int h = (orig+1)%capacity;
	while(h!=orig)
	{
		if(m[h].size()!=0 && m[h][0].first==key)
			return true;
		h = (h+1)%capacity;
	}
	return false;
}

template <typename Key,typename Value>
void OpenMap<Key,Value>::remove(const Key& key)
{
	int orig = Hash(key);
	if (m[orig].size()==0 && !marker[orig]) return;
	else if(m[orig][0].first==key)
	{
		m[orig].clear();
		marker[orig]=true;
		size--;
		return;
	}
	int h = (orig+1)%capacity;
	while(h!=orig)
	{
		if(m[h].size()!=0 && m[h][0].first==key)
		{	
			m[h].clear();
			marker[h]=true;
			size--;
			return;
		}
		h = (h+1)%capacity;
	}
}

template <typename Key,typename Value>
Value OpenMap<Key,Value>::get(const Key& key)
{
	int orig = Hash(key);
	if (m[orig].size()==0) 	throw std::invalid_argument("Key doesn't exist in the map.\n");
	else if(m[orig][0].first==key) return m[orig][0].second;
	int h = (orig+1)%capacity;
	while(h!=orig)
	{
		if(m[h].size()!=0 && m[h][0].first==key)
			return m[h][0].second;
		h = (h+1)%capacity;
	}
	throw std::invalid_argument("Key doesn't exist in the map.\n");
}
#endif