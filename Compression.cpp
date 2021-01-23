#include"bMinHeap.h"
#include<iostream>
#include<string>
#include<cstring>
#include<fstream>
#include<queue>

using namespace std;

int bintodec(string key)
{
	int i = 0;
	int num = 0;
	int n = 0;
	int p = 7;
	while (key[i] != '\0')
	{
		n = key[i] - 48;
		num += (n * pow(2, p--));
		i++;
	}
	return num;
}



class HashTable
{
	int* htable;
	string* bin;
	int size;

public:
	HashTable()
	{
		htable = new int[256];
		bin = new string[256];
		for (int i = 0; i < 256; i++)
		{
			htable[i] = 0;
			bin[i] = "";
		}
		size = 0;
	}

	void insert(string key)
	{
		int num = bintodec(key);
		if (htable[num] == 0)
		{
			bin[num] = key;
			size++;
		}
		htable[num]++;
	}


	int getSize()
	{
		return size;
	}

	void arrToHeap(Word** fwords)
	{
		int j = 1;
		for (int i = 0; i < 256; i++)
		{
			if (htable[i] != 0)
			{
				fwords[j] = new Word;
				fwords[j]->frequency = htable[i];
				fwords[j]->bin_value = bin[i];
				fwords[j++]->dec_value = i;
			}
		}
	}



	~HashTable()
	{
		delete[] htable;
		delete[] bin;
	}


};





int main()
{
	HashTable ht;
	string words;
	fstream file;
	file.open("sample3.8b", ios::in);

	file >> words;

	file.close();

	int len = 0;

	string temp;

	for (int i = 0; len < words.length(); i++)
	{
		temp.assign(words, len, 8);
		len += 8;
		ht.insert(temp);
	}


	int size = ht.getSize() + 1;

	Word** allwords = new Word*[size];

	ht.arrToHeap(allwords);

	bMinHeap bmin;

	for (int i = 1; i < size; i++)
	{
		bmin.insert(allwords[i]);
	}

	bmin.EncodingTree();

	bmin.treeinFile("abc2.cmp");

	fstream filecompress;
	filecompress.open("abc2.cmp", ios::app);

	len = 0;

	filecompress << "\n";


	double count = 0;

	for (int i = 0; len < words.length(); i++)
	{
		temp.assign(words, len, 8);
		len += 8;
		int key = bintodec(temp);
		string c = bmin.searchintree(key);
		count += c.length();
		filecompress << c;
	}

	temp = "";


	filecompress.close();

	double olen = words.size();
	double clen = count;


	cout <<"Compression Complete:" <<(double)(1 - (clen / olen)) * 100 << "%" << endl;

	return 0;
}