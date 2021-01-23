#include<fstream>
#include<iostream>
#include<string>
#include"bMinHeap.h"

int main()
{
	bMinHeap bmin;
	fstream file("abc2.cmp", ios::in);
	fstream orgfile("sample.8b", ios::out);

	string treeLine = "";

	getline(file, treeLine, '\n');

	Word* root = bmin.regainTree(treeLine);

	Word* prevRef = root;

	string c = "";

	file >> c;

	int i = 0;
	while (c[i] != '\0')
	{
		if (c[i] == '1')
		{
			root = root->rchild;
			if (root->rchild == nullptr && root->lchild == nullptr)
			{
				orgfile << root->bin_value;
				root = prevRef;
			}
		}
		else
		{
			root = root->lchild;
			if (root->rchild == nullptr && root->lchild == nullptr)
			{
				orgfile << root->bin_value;
				root = prevRef;
			}
		}
		i++;
	}

		
	file.close();

	orgfile.close();

	cout << "Decompression Complete" << endl;

	return 0;

}