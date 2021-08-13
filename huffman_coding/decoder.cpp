#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <bitset>

using namespace std;

int binary_int(string b) {// Receive a string written in binary and convert it to a decimal value.
	int bi = stoi(b);
	int result = 0, mul = 1;
	while (bi > 0) {//repeat if bi is greater than 0
		if ((bi % 2) == 1) { result += mul; }//If the remainder is 1, add it.
		mul *= 2;
		bi /= 10;
	}
	return result;
}
int main() {
	map<string, char>huffman;//Decoded table

	ifstream readfile("Huffman_table.hbs", ifstream::binary);
	unsigned char* table = NULL;
	int len = 0;
	if (readfile) {//Reading binary files
		readfile.seekg(0, readfile.end);
		len = (int)readfile.tellg();
		readfile.seekg(0, readfile.beg);

		table = (unsigned char*)malloc(len);

		readfile.read((char*)table, len);
		readfile.close();
	}

	//Receives in 1 byte unit, reads it in 8 bits, and stores it in str string
	bitset<8> b;
	string str = "\0";
	for (int i = 0; i < len; i++) {
		b = table[i];
		str += b.template to_string<char, char_traits<char>, allocator<char> >();
	}

	//Create table using str
	char ascii;
	int index = 0, bitnum = 0;
	string cw;
	while (index + 8 < str.length()) {//Repeat if next letter is found
		ascii = (char)binary_int(str.substr(index, 8));	//Receiving text
		index += 8;
		bitnum = binary_int(str.substr(index, 8));	//Get how many bits a codeword is
		index += 8;
		cw = str.substr(index, bitnum);		//save codeword
		index += bitnum;
		huffman.insert(pair<string, char>(cw, ascii));	//insert into huffman table
	}

	//Receive codeword and create original
	ifstream codefile("huffman_code.hbs", ifstream::binary);
	unsigned char* code = NULL;
	int codelen = 0;
	if (codefile) {//Reading code binary files
		codefile.seekg(0, codefile.end);
		codelen = (int)codefile.tellg();
		codefile.seekg(0, codefile.beg);

		code = (unsigned char*)malloc(codelen);

		codefile.read((char*)code, codelen);
		codefile.close();
	}

	str = "\0";
	for (int i = 0; i < codelen; i++) {//Read 1 byte and convert to bit type string
		b = code[i];
		str += b.template to_string<char, char_traits<char>, allocator<char> >();
	}

	index = 0;
	int i = 1;
	string sub, result = "\0";
	map<string, char>::iterator it;

	while (1) {//loop
		sub = str.substr(index, i);
		it = huffman.find(sub);
		if (it == huffman.end()) { i++; }//If it is a codeword that is not in the table
		else {
			if (it->second == 26) { break; }//End iteration if eof
			else {//If it is a letter
				result += it->second;
				index += i;
				i = 1;
			}
		}
	}

	ofstream writefile;
	writefile.open("output.txt");
	if (writefile.is_open()) {//Output as txt file
		writefile.write(result.c_str(), result.length());//write
	}
	writefile.close();
	return 0;

}