#ifndef BMINHEAP_H_INCLUDED
#define BMINHEAP_H_INCLUDED

#include<vector>
#include<string>
#include<queue>
#include<fstream>
#include<stack>
using namespace std;

struct Word
{
	int frequency;
	int dec_value;
	string bin_value;
	string code;
	Word* lchild, * rchild;

	Word()
	{
		code = "";
		frequency = 0;
		dec_value = 0;
		bin_value = "";
		lchild = nullptr;
		rchild = nullptr;
	}

	Word(Word* w1, Word* w2)
	{
		code = "";
		bin_value = "";
		frequency = w1->frequency + w2->frequency;
		dec_value = 0;
		lchild = w1;
		rchild = w2;
	}

	Word(string c, string b, int f, int d, Word* l, Word* r)
	{
		code = c;
		bin_value = b;
		frequency = f;
		dec_value = d;
		lchild = l;
		rchild = r;
	}

	bool operator<(Word w)
	{
		if (frequency < w.frequency)
		{
			return true;
		}
		return false;
	}

	bool operator>(Word w)
	{
		if (frequency > w.frequency)
		{
			return true;
		}
		return false;
	}

	bool operator<=(Word w)
	{
		if (frequency <= w.frequency)
		{
			return true;
		}
		return false;
	}

	bool operator>=(Word w)
	{
		if (frequency >= w.frequency)
		{
			return true;
		}
		return false;
	}

	bool operator!=(Word w)
	{
		if (frequency != w.frequency)
		{
			return true;
		}
		return false;
	}

	Word& operator=(const Word& w)
	{
		if (this != &w)
		{
			frequency = w.frequency;
			dec_value = w.dec_value;
			code = w.code;
			bin_value = w.bin_value;
			lchild = w.lchild;
			rchild = w.rchild;
		}
		return *this;
	}

	void setToNull()
	{
		code = "";
		frequency = 0;
		dec_value = 0;
		lchild = nullptr;
		rchild = nullptr;
	}

};


class bMinHeap {

	vector<Word*> h;

	inline int parent(int i) {
		return i / 2;
	}

	inline int lchild(int i) {
		return 2 * i;
	}

	inline int rchild(int i) {
		return 2 * i + 1;
	}


	inline bool isLeaf(int i) {
		return lchild(i) > size();

	}

	void percolateUp(int i) {
		//O(lgn)
		while (parent(i) > 0 && h[i]->frequency < h[parent(i)]->frequency) {
			swap(h[i], h[parent(i)]);
			i = parent(i);
		}
	}


	void heapify(int i) {//percolate down
		//O(lgn)
		while (!isLeaf(i)) {
			int mini = i;//node itself is min

			//update if lchild has smaller key
			if (h[lchild(i)]->frequency < h[mini]->frequency)
				mini = lchild(i);

			//update if rchild has even smaller key
			if (rchild(i) < size() && h[rchild(i)]->frequency < h[mini]->frequency)
				mini = rchild(i);

			if (i != mini) {
				swap(h[i], h[mini]);//swap ith node with the minimum of i, 2*i and 2*i+1
				i = mini;//move to one of the children
			}
			else
				break;//no violation
		}



	}

	void EncodingTree(Word* root, string c)
	{
		if (root != NULL)
		{
			EncodingTree(root->lchild, c + "0");
			EncodingTree(root->rchild, c + "1");
			root->code = c;
		}
	}

	void searchintree(Word* root, string& c, int key)
	{
		if (root != NULL)
		{
			searchintree(root->lchild, c, key);
			if (root->dec_value == key)
			{
				c = root->code;
				return;
			}
			searchintree(root->rchild, c, key);
		}
	}


	void postorderforFile(Word* ptr, string& tree)
	{

		if (ptr != NULL)
		{
			postorderforFile(ptr->lchild, tree);
			postorderforFile(ptr->rchild, tree);
			if (ptr != NULL && (ptr->lchild == NULL && ptr->rchild == NULL))
			{
				tree += "1";
				tree += ptr->bin_value;
				tree += " ";
			}
			else
			{
				tree += "0";
				tree += " ";
			}

		}
	}


public:
	bMinHeap() {
		h.resize(1);
	}

	int size() {
		return h.size() - 1;
	}

	bool empty() {
		return size() == 0;
	}

	Word* getMin() {
		//O(1)
		return h[1];
	}


	void insert(Word* key) {

		h.push_back(key);//key becomes the next leaf the last level

		//now percolate up
		int i = size(); //index of the newly added node
		percolateUp(i);
	}


	void deteleMin() {
		if (size() > 0) {
			h[1] = h[size()];//overwrite root with last leaf
			h.pop_back();//remove last leaf
			heapify(1);//percolateDown(1)
		}
	}


	Word* makeTree()
	{
		heapify(1);
		Word* lc, * rc, * top, temp;
		while (!empty())
		{
			lc = getMin();
			deteleMin();
			rc = getMin();
			deteleMin();

			top = new Word(lc, rc);

			if (h.size() == 1)
			{
				insert(top);
				return top;
			}
			insert(top);
		}
	}

	void EncodingTree()
	{
		Word* root = makeTree();

		string temp = "";

		EncodingTree(root, temp);
		return;
	}

	string searchintree(int key)
	{
		Word* root = getMin();

		string c = "";

		searchintree(root, c, key);

		return c;

	}

	void treeinFile(const char* filename)
	{
		fstream file(filename, ios::out);

		Word* prev = NULL;
		Word* root = getMin();

		string tree = "";
		postorderforFile(root, tree);

		file << tree;

		file.close();

	}

	Word* regainTree(string tree)
	{
		stack<Word*> ptr;

		int i = tree.length() - 1;

		//char temp = ' ';

		Word* root = new Word;

		ptr.push(root);

		i -= 2;

		while (i >= 0)
		{
			if ((i - 1) >= 0 && tree[i] == '0' && tree[i - 1] == ' ')
			{
				Word* temp = !ptr.empty() ? ptr.top() : NULL;
				if (temp->rchild == NULL)
				{
					Word* newtemp = new Word;
					temp->rchild = newtemp;
					//ptr.pop();
					ptr.push(newtemp);
				}
				else
				{
					Word* newtemp = new Word;
					temp->lchild = newtemp;
					if (!ptr.empty())
					{
						ptr.pop();
					}
					ptr.push(newtemp);
				}
				i--;
			}
			else if (tree[i] == ' ')
			{
				i--;
			}
			else
			{
				Word* temp = ptr.top();
				if (temp->rchild == NULL)
				{
					//Word* newtemp = new Word;
					temp->rchild = new Word;
					int j = i - 7;
					while (j <= i)
					{
						temp->rchild->bin_value += tree[j++];
					}
					i -= 8;
					if (ptr.size() > 1)
					{
						ptr.pop();
					}
				}
				else
				{
					temp->lchild = new Word;

					int j = i - 7;
					while (j <= i)
					{
						temp->lchild->bin_value += tree[j++];
					}
					i -= 8;
					bool check = true;
					if (!ptr.empty())
					{
						ptr.pop();
					}
				}
				i--;
			}


		}

		string c = "";
		EncodingTree(root, c);
		return root;

	}


	~bMinHeap()
	{
	}

};


#endif // BMINHEAP_H_INCLUDED