#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <algorithm>
#include <thread>
#include <list>
using namespace std;

// A Tree node
struct Node
{
	char ch;
	int freq;
	Node *left, *right;
};


bool FindCode(string s, unordered_map<char, string> code)
{
	for(auto pair: code)
	{
		if(pair.second == s)
		{
			return true;
		}
	}
	return false;
}
char FindLatter(string s, unordered_map<char, string> code)
{
	for(auto pair: code)
	{
		if(pair.second == s)
		{
			return pair.first;
		}
	}
	return ' ';
}

// Function to allocate a new tree node
Node* getNode(char ch, int freq, Node* left, Node* right)
{
	Node* node = new Node();

	node->ch = ch;
	node->freq = freq;
	node->left = left;
	node->right = right;

	return node;
}

// Comparison object to be used to order the heap
struct comp
{
	bool operator()(Node* l, Node* r)
	{
		// highest priority item has lowest frequency
		return l->freq > r->freq;
	}
};

struct compRoot
{
	bool operator()(pair<int, Node*> l, pair<int, Node*> r)
	{
		// highest priority item has lowest frequency
		return l.first > r.first;
	}
};

struct compFreq
{
	bool operator()(pair<char, int> l, pair<char, int> r)
	{
		// highest priority item has lowest frequency
		return l.first > r.first;
	}
};

// traverse the Huffman Tree and store Huffman Codes
// in a map.
void encode(Node* root, string str,
			unordered_map<char, string> &huffmanCode)
{
	//if (root == nullptr)
	//	return;

	// found a leaf node
	if (!root->left && !root->right) {
		huffmanCode[root->ch] = str;
		return;
	}

	encode(root->left, str + "0", huffmanCode);
	encode(root->right, str + "1", huffmanCode);
}

// traverse the Huffman Tree and decode the encoded string
void decode(Node* root, int &index, string str, ofstream &u, string text, int flag)
{
	if (root == nullptr) {
		return;
	}
	
 	if(flag > (int)text.size() * 8 - 1)
	{
		u << root->ch;
		cout << root->ch << "******************";
		index = (int)str.size();
		return;
	}
	
	
	// found a leaf node
	if (!root->left && !root->right)
	{
		u << root->ch;
		cout << root->ch;
		
		return;
	}

	index++;
	
	if (str[index] =='0')
		decode(root->left, index, str, u, text, flag);
	else
		decode(root->right, index, str, u, text, flag);
}

void ThreadEncode(unordered_map<char, int> freq, string text, int key, priority_queue<pair<int, Node*>, vector<pair<int, Node*> >, compRoot> &root, unordered_map<int, unordered_map<char, string> > &huffmanCode)
{
	priority_queue<Node*, vector<Node*>, comp> pq;

	// Create a leaf node for each character and add it
	// to the priority queue.
	for (auto pair: freq) {
		
		pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
	}

	// do till there is more than one node in the queue
	while (pq.size() != 1)
	{
		cout << pq.top()->ch << endl;
		// Remove the two nodes of highest priority
		// (lowest frequency) from the queue
		Node *left = pq.top(); pq.pop();
		cout << pq.top()->ch << endl;
		Node *right = pq.top();	pq.pop();
		cout << "-------" << endl;
		// Create a new internal node with these two nodes
		// as children and with frequency equal to the sum
		// of the two nodes' frequencies. Add the new node
		// to the priority queue.
		int sum = left->freq + right->freq;
		pq.push(getNode('\0', sum, left, right));
	}

	// root stores pointer to root of Huffman Tree
	Node* root1 = pq.top();
	cout << root1->freq << endl;
	// traverse the Huffman Tree and store Huffman Codes
	// in a map. Also prints them
	
	encode(root1, "", huffmanCode[key]);
	pair<int, Node*> p (key, root1);
	root.push(p);
}


pair<int, unordered_map<char, int> > p(int key, unordered_map<char, int> map)
{
	//pair<int, unordered_map<char, int>> pai;
	return  make_pair(key, map);
}

// Builds Huffman Tree and decode given input text
void buildHuffmanTree(string text, int numThreads)
{
	list<int> keys;
	ofstream f;
	ofstream table("table.txt", std::ios::binary | ios::trunc);
	f.open("binary.zipper", std::ios::binary | ios::out);
	// count frequency of appearance of each character
	// and store it in a map
	unordered_map<int, unordered_map<char, int> > freq;
	int numThread = 0;
	int c = 0;
	unordered_map<char, int> tmpMap;
	for (char ch: text) {
		tmpMap[ch]++;
		if(tmpMap.size() == 64){
			numThread = c;
			freq[numThread] = tmpMap;
			keys.push_back(numThread);
			tmpMap.clear();
		}
		c++;
	}
	freq[c] = tmpMap;
	keys.push_back(c);
	priority_queue<pair<int, Node*>, vector<pair<int, Node*> >, compRoot> root;
	unordered_map<int, unordered_map<char, string> > huffmanCode;

	int tmp1 = 0;
	
	for(auto pair: keys)
	{
		ThreadEncode(freq[pair], text.substr(tmp1, pair), pair, root, huffmanCode);
		tmp1 = pair;
	}

/*
	// Create a priority queue to store live nodes of
	// Huffman tree;
	priority_queue<Node*, vector<Node*>, comp> pq;

	// Create a leaf node for each character and add it
	// to the priority queue.
	for (auto pair: freq) {
		
		pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
	}

	// do till there is more than one node in the queue
	while (pq.size() != 1)
	{
		cout << pq.top()->ch << endl;
		// Remove the two nodes of highest priority
		// (lowest frequency) from the queue
		Node *left = pq.top(); pq.pop();
		cout << pq.top()->ch << endl;
		Node *right = pq.top();	pq.pop();
		cout << "-------" << endl;
		// Create a new internal node with these two nodes
		// as children and with frequency equal to the sum
		// of the two nodes' frequencies. Add the new node
		// to the priority queue.
		int sum = left->freq + right->freq;
		pq.push(getNode('\0', sum, left, right));
	}

	// root stores pointer to root of Huffman Tree
	//Node* root = pq.top();
	cout << root->freq << endl;
	// traverse the Huffman Tree and store Huffman Codes
	// in a map. Also prints them
	unordered_map<int, unordered_map<char, string>> huffmanCode;
	encode(root, "", huffmanCode);
/*
	cout << "Huffman Codes are :\n" << '\n';
	for(auto pair: huffmanCode) {
		cout << pair.first << " " << pair.second << '\n';
		table << pair.first;
		table << pair.second;
		table << '*';
	}
	table << '@';
	table << text.length();
	cout << "\nOriginal string was :\n" << text << '\n';
*/
	// print encoded string
	string str = "";
	//string t = "";
	unsigned char tmp = '\0';
	int flag = 0;
	int j = 0;
	cout << keys.size() << endl;
	
	for(auto keyk: keys)
	{

		cout << keyk << endl;
	for(char ch: text.substr(j, keyk - j + 1)){
		str += huffmanCode[keyk][ch];
		//cout << huffmanCode[ch] << endl;
		for(char c: huffmanCode[keyk][ch])
		{
		//t += c;
		
		if(c == '1')
		{
			//tmp = tmp + 2^(7 - flag);
			tmp |= (1<< (7 - flag));
		}
		else
		{
			//tmp = tmp & static_cast<unsigned char>(255 - (1<<(7 - flag)));
		}
		if(flag == 7)
		{
			for(int i = 0; i < 8; i++)
			{
			//	cout << hex <<(tmp << i) << endl;
			}
		//	cout << hex << (tmp << 0) << endl;
			f << tmp;
			tmp = '\0';
			flag = 0;
			continue;
		}
		//t = "";
		
		flag++;
		}
		
	}
	//tmp = '\0';
	//flag = 0;
	j = keyk + 1;
	}
	
	cout << tmp << endl;
	for(int i = flag; i < 8; i++)
	{
	//	tmp = tmp & static_cast<unsigned char>(255 -(1<<(7 - i)));
	}
	//if(flag != 0)
	f << tmp;

	cout << "\nEncoded string is :\n" << str << '\n';

	// traverse the Huffman Tree again and this time
	// decode the encoded string
	int index = -1;
	//cout << "\nDecoded string is: \n";
	table.close();
	f.close();
	/*ifstream table1("table.txt", ios::binary);
	ofstream u("./unpack/unpack.txt", ios::binary);
	
	unordered_map<char, string> code;
	char c, bit;
	string s = "";
	while(table1.get(c))
	{
		table1.get(bit);
        if(bit == ' ') 
        {
            break;
        }
		while(bit != '\n')
		{
			s.push_back(bit);
			table1.get(bit);
		}
		code[c] = s;
		cout << c << " " << code[c] << endl;
		s = "";
	}
	string tmpstr = "";
	index = 0;
	//cout << str;
	
	fclose(f);
	*/
	cout << "asd" << endl;
	str = "";
	ifstream z("binary.zipper", ios::binary);
	char c1;
	
	while(z.get(c1))
	{

		for(int i = 0; i < 8; i++)
		{
            if(((c1 >> (7 - i)) & 1) == 1)
			str += "1";
            else str += "0";
            
		}
		c1 = '\0';
	}
	
	cout << str << endl;
	int iterator = 0;
	ofstream u("./unpack/unpack.svg", ios::trunc);
	int r2 = 0;
	int key1 = 0;
	while (index < (int)str.size()) {

	/*	tmpstr.push_back(str[index]);
        cout << str.at(index);
		if(FindCode(tmpstr, code)) {
			u << FindLatter(tmpstr, code);
			
			tmpstr = "";
		}
		index++;*/
		
		
		Node* r = root.top().second;
		key1 = root.top().first;
		root.pop();
		cout << key1 << "   " << r->freq << iterator << "   " << text.substr(r2, r->freq - r2) << endl;
		while(iterator < (key1 * 8 + 1) && index < (int)str.size()) {
		
		decode(r, index, str, u, text, iterator);
		iterator += 8;
		}
		r2 = r->freq;
		cout << index << "   " << (int)str.size() << endl;
		

	}
}




// Huffman coding algorithm
int main()
{
	string text = "So this is testing the Huffman algotithm - 1234567890!@#$%^&*()_+QWERTYUIOP{}йцукенгшщзхъфывапролджэячсмитьбюQERTYUIOPASDFGHJKLZXCVBNM<>QWERTYUIOP{ASDFGHJLЙЦУКЕНГШЩЗХФЫВАПРОЛДЖЭ//ЯЧСМИТЬБЮ";

	ifstream file("i.jpeg");

	string str = "";
	char s;
	int i = 0;
	while (file.get(s))
        str += s;
	file.close();
//	cout << str << endl;
	
	unsigned int numThreads = std::thread::hardware_concurrency();
	cout << numThreads << endl;
	buildHuffmanTree(str, numThreads);

	return 0;
}
