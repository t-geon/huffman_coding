#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>		
#include <map>
#include <stdio.h>
#include <bitset>
using namespace std;

typedef struct node {//Node with information about the character
	char symbol; //character
	double probability;
	string codeword;

	node* left;
	node* right;
	node* par;

	node(char s, double p) {//Default constructor
		symbol = s;
		probability = p;
		codeword = "\0";
		left = NULL;
		right = NULL;
		par = NULL;
	}

	node(char s, double p, node* l, node* r) {
		symbol = s;
		probability = p;
		codeword = "\0";
		if (l->probability == r->probability && r->symbol == 1) { this->left = r; this->right = l; }
		else { this->left = l; this->right = r; }
		par = NULL;
		l->par = this;
		r->par = this;
	}
	~node() { delete left, right, par; };
}node;

typedef struct tree {
	node* root;

	tree() { root = NULL; }
	~tree() { delete root; }
}tree;

void preorder(node* c, map<char, pair<string, double>>* count) {//Codeword generation through front-end traversal
	if (c == NULL) return;
	if (c->left != NULL && c->left->codeword == "\0") { c->left->codeword = c->codeword + "0"; }//Put 0 on the left
	if (c->right != NULL && c->right->codeword == "\0") { c->right->codeword = c->codeword + "1"; }//Put 1 on the right
	if (c->left == NULL && c->right == NULL) { count->find(c->symbol)->second.first = c->codeword; }//If a node has no children, save the codeword in the map.
	preorder(c->left, count);
	preorder(c->right, count);
}

unsigned char fit(const char* arr) {
	unsigned char pa = 0;
	int j = 7;
	for (int i = 0; i <= 7; i++) {//8bit fit
		if (arr[i] == '1') {//If the value is 1 at the position of the array passed as an argument
			pa |= 1 << j; //Align 1 position
		}
		j--;//size to shift
	}
	return pa;
}

int main() {
	map<char, pair<string, double>> arrh[128];
	map<char, pair<string, double>> count;
	map<char, pair<string, double>>::iterator it;	//iterator to receive the position of count
	multimap<double, node*> prob;	//Multimap sorted in ascending order of probability
	multimap<double, node*>::iterator iter;	//iterator to receive the position of the multimap

	char sym;		//Characters read
	double st_num = 0;	//Total number of characters

	tree* huffman = new tree;
	ofstream output;

	FILE* file = NULL;
	file = fopen("input_data.txt", "r"); //Open in reading mode


	if (file == NULL) { return 0; }//error
	else {
		while ((sym = fgetc(file)) != EOF) {//Eof doesn't store the last data because ASCII code 26 needs to be known.
			it = count.find(sym);//Finding if a character already exists
			if (it == count.end()) { count.insert(pair<char, pair<string, double>>(sym, make_pair<string, double>("\0", 1))); }	//It is 1 because it is the first letter.
			else { it->second.second++; }//count
			st_num++;	//Add 1 character
		}
		fclose(file);
	}

	count.insert(pair<char, pair<string, double>>(26, make_pair<string, double>("\0", 1)));	//EOF added at the end
	st_num++;

	for (it = count.begin(); it != count.end(); it++) {
		double p = it->second.second / st_num;	//Save probabilities for each character
		it->second.second = p;
		node* pr = new node(it->first, p);	//Create a new node
		prob.insert(pair<double, node*>(p, pr));
	}

	//Building a Huffman Tree
	node* parent, * l, * r;	//Put the value to the left and right of parent
	while (prob.size() != 1) {	//Until there is 1 left in the prob
		l = prob.begin()->second;
		prob.erase(prob.begin());	//Delete minimum value
		r = prob.begin()->second;
		prob.erase(prob.begin());	//Delete minimum value
		parent = new node(1, l->probability + r->probability, l, r);	//Initialization of new parent node (character is ASCII code 1 value)
		prob.insert(pair<double, node*>(parent->probability, parent));	//insert new node into prob
	}

	huffman->root = prob.begin()->second;//Initialize root
	preorder(huffman->root, &count);	//Number mapping

	//table encoding
	bitset<8> b;//8bit bitstream
	string table = "\0";
	for (it = count.begin(); it != count.end(); it++) {
		b = it->first;	//Change char to 8bit bitstream
		table += b.template to_string<char, char_traits<char>, allocator<char> >();
		b = it->second.first.length();
		table += b.template to_string<char, char_traits<char>, allocator<char> >();
		table += it->second.first;
	}

	output.open("Huffman_table.hbs", ios::binary);

	const char* ta = table.c_str();	//convert string to const char*
	for (int i = 0; i * 8 < table.length(); i++) {//Divide the size by 8 and write it in 1 byte size.
		char re = fit(&table[i * 8]);
		output.write(&re, sizeof(re));
	}
	output.close();

	//Attaching codewords to strings while reading the file again
	string result = "\0";
	file = fopen("input_data.txt", "r");
	if (file == NULL) { return 0; }//error
	else {
		while ((sym = fgetc(file)) != EOF) {//Eof doesn't store the last data because ASCII code 26 needs to be known.
			it = count.find(sym);//Finding if a character already exists
			result += it->second.first;//Paste string
		}
		it = count.find(26);	//check the last eof
		result += it->second.first;	//Put the last eof
		fclose(file);
	}

	int padding = (8 - (result.length() % 8)) % 8;//Calculate stuffing bit according to 8bit
	for (int i = 0; i < padding; i++) { result += "0"; }   //Fill with zeros

	output.open("huffman_code.hbs", ios::binary);

	const char* result_str = result.c_str();	// convert string to const char*
	for (int i = 0; i * 8 < result.length(); i++) {//Divide the size by 8 and write it in 1 byte size.
		char re = fit(&result_str[i * 8]);
		output.write(&re, sizeof(re));
	}

	output.close();
	return 0;
}