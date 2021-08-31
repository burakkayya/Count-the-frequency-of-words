
#include "WordFreq.h"
#include <vector>
#include <fstream>
#include <string.h>
#include <string>
#include <iostream>
#include <map>
#include <stack>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
using namespace std;

///-----------------------------------------------------------------
/// Process the given text file and return the most frequent "k" words
struct AVLNode {
	int tekrar;
	string kelime;
	int height;
	AVLNode* left, * right;
	AVLNode(string kelime, int tekrar) { this->tekrar = tekrar; left = right = NULL; height = 0; this->kelime = kelime; }
};

void add(string, int);
int stringcompare(string, string);
void balance(stack<AVLNode*>&);

vector<WordFreq> GetMostFrequentKWords(string filename, int k) {
	vector<WordFreq> result;
	map <string, int> word;
	fstream file;
	string satir="";
	int i;
	file.open(filename, ios::in);
	
	while (getline(file, satir)) {
		
		i = 0;
		string kelime = "";
		while (satir[i]!='\0')
		{
			
			if (isalpha(satir[i])) {
				satir[i] = tolower(satir[i]);
				kelime += satir[i];
				i++;
			}
			else if (isspace(satir[i]) || !isalpha(satir[i])) {

				auto search = word.find(kelime);
				if (search != word.end()) {
					search->second++;
					kelime = "";
					i++;
				}
				else {
					if (kelime.size() > 2)
						word.insert({ kelime,1 });
					kelime = "";
					i++;
				}

			}
			else
				i++;

		}
	}
	for (auto j = word.begin(); j != word.end(); j++) {
		add(j->first ,j->second);
	}
	auto j = word.begin();
	int t=j->second;
	for ( ; j != word.end(); j++) {
		
		if (t < j->second)
			t = j->second;
	}

	vector<vector<string>> words(t + 1);
	vector<string>temp;
	for (i = 0; i < t + 1; i++) {

		for (auto j = word.begin(); j != word.end(); j++) {
			if (i == j->second) {
				temp.push_back(j->first);
			}
		}
		
		words.insert(words.begin() + i, temp);
		temp.clear();
	}

	for (int i = t; i >0; i--){
	
		for (int j = 0; j < words[i].size(); j++) {
			WordFreq* tmp = new WordFreq(words[i][j]);
			tmp->freq = i;
			result.push_back(*tmp);

			k--;
			if (k == 0)
				break;
			}
		if (k == 0)
			break;
	}
	
	return result;
} //end-GetMostFrequentKWords

AVLNode* root = NULL;
stack<AVLNode*> S;
void add(string kelime, int tekrar) {
	
	AVLNode* q = NULL;
	AVLNode* p = root;
	while (p) {
		S.push(p);

		q = p;
		if (p->kelime == kelime) {
			return;
		}
		else if (stringcompare(kelime, p->kelime)==1) {
			p = p->left;
		}
		else {
			p = p->right;
		} 
	} 
	
	AVLNode* node = new AVLNode(kelime, tekrar);
	if (root == NULL) root = node;
	else if (stringcompare(kelime , q->kelime)==1) q->left = node;
	else q->right = node;
	
	
	balance(S);
} 

#define Height(p) ((p) == NULL? -1:(p)->height)

void balance(stack<AVLNode*>& S) {
	
	while (!S.empty()) {
		AVLNode* p = S.top(); S.pop();

		int hl = Height(p->left);
		int hr = Height(p->right);
		p->height = 1 + std::max(hl, hr);

		int bf = hl - hr;
		if (bf >= -1 && bf <= 1) continue;

		AVLNode* pp = NULL;
		if (!S.empty()) pp = S.top();

		if (bf == 2) {
			
			AVLNode* l = p->left;

			hl = Height(l->left);
			hr = Height(l->right);
			bf = hl - hr;

			if (bf == 0 || bf == 1) {
				
				p->left = l->right;
				l->right = p;

				p->height = 1 + std::max(Height(p->left), Height(p->right));
				l->height = 1 + std::max(Height(l->left), Height(l->right));

				if (pp == NULL) root = l;
				else {
					if (p == pp->left) pp->left = l;
					else               pp->right = l;
				} 

			}
			else {
				
				AVLNode* x = l->right;

				l->right = x->left;
				x->left = l;

				p->left = x->right;
				x->right = p;

				p->height = 1 + std::max(Height(p->left), Height(p->right));
				l->height = 1 + std::max(Height(l->left), Height(l->right));
				x->height = 1 + std::max(Height(x->left), Height(x->right));

				if (pp == NULL) root = x;
				else {
					if (p == pp->left) pp->left = x;
					else               pp->right = x;
				} 

			}

		}
		else {
			
			AVLNode* r = p->right;

			hl = Height(r->left);
			hr = Height(r->right);
			bf = hl - hr;

			if (bf == 0 || bf == -1) {
				
				p->right = r->left;
				r->left = p;

				p->height = 1 + std::max(Height(p->left), Height(p->right));
				r->height = 1 + std::max(Height(r->left), Height(r->right));

				if (pp == NULL) root = r;
				else {
					if (p == pp->left) pp->left = r;
					else               pp->right = r;
				} 

			}
			else {
				
				AVLNode* x = r->left;

				r->left = x->right;
				x->right = r;

				p->right = x->left;
				x->left = p;

				p->height = 1 + std::max(Height(p->left), Height(p->right));
				r->height = 1 + std::max(Height(r->left), Height(r->right));
				x->height = 1 + std::max(Height(x->left), Height(x->right));

				if (pp == NULL) root = x;
				else {
					if (p == pp->left) pp->left = x;
					else               pp->right = x;
				} 
			} 
		} 
	} 
} 


int stringcompare(string st1, string st2) {
	int i = 0;
	while (1) {

		if ((int)st1[i] < (int)st2[i]) {
			return 1;
		}
		else if ((int)st1[i] > (int)st2[i]) {
			return -1;
		}
		else if (st1[i] == NULL) {
			return 1;
		}
		else if (st2[i] == NULL) {
			return -1;
		}
		i++;
		
	}


}