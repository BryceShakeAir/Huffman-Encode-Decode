#include <bits/stdc++.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <cstdio>
using namespace std; 

double encode_time;
double decode_time;

/*
	Specify proper command line arguments to run the program
	The arguments expected are:
        1) Cache size m (Minimum 32)
        2) Path to input file
        3) Path to encoded file
        4) Path to output file
*/
ifstream input_stream;
FILE * encoded_stream_in;
ofstream encoded_stream;
ofstream output_stream;
int m;
int encoded_symbol_bit_length;
int original_size;
int compressed_size;

vector<int> encoding(string s1) 
{
    queue<string> fifokeys;
	cout << "Encoding\n"; 
	unordered_map<string, int> table; 
	for (int i = 0; i < 26; i++) { 
		string ch = ""; 
		ch += char(i + 'a'); 
		table[ch] = i; 
	} 

	string p = "", c = ""; 
	p += s1[0]; 
	int code = 26; 
	vector<int> output_code; 
	// cout << "String\tOutput_Code\tAddition\n"; 
	for (int i = 0; i < s1.length(); i++) { 
		if (i != s1.length() - 1) 
			c += s1[i + 1];
        unordered_map<string, int>::iterator f = table.find(p+c);
        unordered_map<string, int>::iterator except = table.size() == m ? table.find(fifokeys.front()) : table.end();
		if (f != table.end() && f != except) { 
			p = p + c; 
		} 
		else { 
			// cout << p << "\t" << table[p] << "\t\t"
				// << p + c << "\t" << code << endl; 
			output_code.push_back(table[p]);
            if(table.size() != m){
			    table[p + c] = code;
                fifokeys.push(p+c);
			    code++;
            } else {
                code = table[fifokeys.front()];
                table.erase(fifokeys.front());
                fifokeys.pop();
                table[p + c] = code;
				fifokeys.push(p+c);
            }
			p = c; 
		} 
		c = ""; 
	} 
	// cout << p << "\t" << table[p] << endl; 
	output_code.push_back(table[p]);
    for(int i = 0; i < output_code.size(); i++){
		string str = bitset< 25 >(output_code[i]).to_string();
        string binary_code = str.substr(str.size()-encoded_symbol_bit_length, encoded_symbol_bit_length);
		encoded_stream << binary_code;
    }
	compressed_size = output_code.size() * encoded_symbol_bit_length;
	return output_code; 
} 

void decoding() 
{ 
	vector<int> op;
	char * code_binary = (char *)calloc(sizeof(char), encoded_symbol_bit_length + 1);
	while(fgets(code_binary, encoded_symbol_bit_length + 1, encoded_stream_in) != NULL){
		if(code_binary[0] == '\n') break;
		op.push_back((int) stoi(std::string(code_binary), nullptr, 2));
	}
	cout << "Decoding\n"; 
	unordered_map<int, string> table;
	queue<int> fifokeys;
	for (int i = 0; i < 26; i++) { 
		string ch = ""; 
		ch += char(i + 'a'); 
		table[i] = ch;
	}
	int old = op[0], n; 
	string s = table[old]; 
	string c = ""; 
	c += s[0];
	// cout << s;
    output_stream << s;
	int count = 26; 
	for (int i = 0; i < op.size() - 1; i++) { 
		n = op[i + 1]; 
		unordered_map<int, string>::iterator f = table.find(n);
        unordered_map<int, string>::iterator except = table.size() == m ? table.find(fifokeys.front()) : table.end();
		if (f == table.end() || f == except) { 
			s = table[old];
			s = s + c; 
		} 
		else { 
			s = table[n]; 
		} 
		// cout << s; 
        output_stream << s;
		c = ""; 
		c += s[0];
		if(table.size() != m){
			table[count] = table[old] + c;
			fifokeys.push(count); 
			count++; 
		} else {
			table[fifokeys.front()] = table[old] + c;
			fifokeys.push(fifokeys.front());
			fifokeys.pop();
		}
		old = n; 
	} 
} 
int main(int argc, char * argv[]) 
{
    if(argc != 5) {
        cout << "4 arguments expected!\n";
        cout << "The arguments expected are:\n";
        cout << "\t 1) Cache size m (Minimum 32)\n";
        cout << "\t 2) Path to input file\n";
        cout << "\t 3) Path to encoded file\n";
        cout << "\t 4) Path to output file\n";
        return 1;
    }

    m = atoi(argv[1]);
	if(m<32){
		cout << "Cache size should be atleast 32. Given size is " << m << endl;
		return 1;
	}

	encoded_symbol_bit_length = (int) floor(log2(m));

    input_stream.open(argv[2], ios::in);
    encoded_stream.open(argv[3], ios_base::out);
    output_stream.open(argv[4], ios::out);
	string s;
    input_stream >> s;
	original_size = 8 * s.size();

	clock_t t = clock();
	vector<int> output_code = encoding(s);
	encoded_stream.close();
	t = clock() - t;
	encode_time = double(t)/CLOCKS_PER_SEC;

	t = clock();
	encoded_stream_in = fopen(argv[3], "r");
	decoding();
	t = clock() - t;
	decode_time = double(t)/CLOCKS_PER_SEC;

	if (original_size == 0) compressed_size = 0;
	cout << endl;
	cout << "Compression ratio: " << original_size << "/" << compressed_size << " = ";
	if(original_size)
		cout << ((float) original_size) / compressed_size << endl;
	else 
		cout << "Indeterminate" << endl;
	cout<<"Time taken to encode:\t"<<encode_time << " seconds\n";
	cout<<"Time taken to decode:\t"<<decode_time << " seconds\n";
    return 0;
} 
