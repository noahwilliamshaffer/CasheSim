//--------------- MAIN.CPP ---------------

//Noah Shaffer
//CDA 3100
//Instruction Cashe Simulator
//C++
//MakeFile included

#include <iostream>		// for cout
#include <string>
#include <stdio.h>      /* printf */
#include <math.h>       /* pow */
#include <fstream>

using namespace std;
void Summary();
string HexToBin(string hex);
bool HitOrMiss();
float Accesses;
int binaryToDecimal(int n);
void SetBlocksHitOrMiss();



class Block{
	public:
	
	//block
	Block(){		
	}
	void SetBlock(string B){
		cout <<"Setting Reference" << endl;
		Address = B;
		Binary = HexToBin(Address);
		Tag = CalcTag(Binary);
		Index = CalcIndex(Binary);
		Offset = CalcOffset(Binary);
		Accesses++;
		//Result = GetResult(R);
	}
	//8 block cashe line 
	string CalcTag(string B){
		string tag;
		for(int i = 0; i < 2; i++){
			tag+=B[i];
		}
		return tag;
	}
	//8 block cashe line index is 
	string CalcIndex(string B){
		string index;
		for(int i = 2; i < 5; i++){
			index += B[i];
		}
		return index;
	}
	//8 block cashe line offset is first 3 bit
	string CalcOffset(string B){
		string offset;
		for(int i = 5; i < B.length(); i++){
			offset += B[i];
		}
		return offset;
	}

	string CalcResult(string B){
		string result = "Miss";
		return result;
	}

	int GetTag(){
		int intTag = stoi(Tag);
		return binaryToDecimal(intTag); 
	}
	int GetIndex(){
		int intIndex = stoi(Index);
		return binaryToDecimal(intIndex); 
	}
	int GetOffset(){
		int intOffset = stoi(Offset);
		return binaryToDecimal(intOffset); 
	}
	string GetResult(){
		if (Result == false)
			return "miss";

		if(Result == true)
			return "hit";

		return "miss";
	}
	
	int Ref;
	string Address;
	string Tag;
	string Index;
	string Offset;
	int IndexNumber;
	int OffsetNumber;
	int valid;
	
	
	string Binary;
	bool Result;
};

class Cashe{
	public:
		Cashe(){

		}
		~Cashe(){
			delete [] Blocks;
		}
	
	int size;
	int tag;
	int index;
	int offset;
	Block *Blocks;
};


Block Blocks[8192];
float Hits;
//Valid bit and tag match its a hit
void HitOrMiss(int current);

string line;
string Sets;
string  SetSize;
string LineSize;
//int ByteSize= 8;

//int IndexCalc;
//assume the size of each reference is 4 bytes

//double OffsetCalc = log2(stoi(Sets));
//double TotalSize = 8;
//offset = log2(Set size * bytes)

int lines= 0;
int SetsInBlock;
int Index;
int Offset;
//string address[8192];


int main()
{
	ifstream config;
	config.open("trace.config");
	if(!config){
			cout <<"The config could not be opened.";
			return -1;
	}

	while(getline(config, line)){	
		
		//cout << line << endl;
		for(int i = 0; i < line.length()+1; i++){
			if(lines == 0){		
					if(isdigit(line[i]))
						Sets += line[i];
			}
			if (lines == 1){
				if(isdigit(line[i]))
					SetSize += line[i];
			}
			if(lines == 2){
				if(isdigit(line[i]))
					LineSize += line[i];

			}
			
		}
		lines++;
	}
	cout << Sets << endl;
	cout <<SetSize << endl;
	cout << LineSize << endl;
	cout <<"Done with config " << endl;

	double OffsetCalc = log2(stoi(SetSize)*stoi(LineSize));
	double IndexCalc = log2(stoi(Sets));
	cout <<"Offsett is " << OffsetCalc<< endl;
	cout <<"Index is " << IndexCalc << endl;
	cout <<"OffsetCalc is " << OffsetCalc << endl;
	cout <<"IndexCalc is " << IndexCalc << endl;
	int Set = stoi(Sets);
	Offset = OffsetCalc;
	Index = IndexCalc;
	config.close();

	//We now have Set, Offsett, and Index feilds for calculations
	lines = 0;
	SetsInBlock = Set;
	ifstream in;
	in.open("trace.dat");
	if(!in){
		cout <<"The file could not be opened." << endl;
		return -1;
	}

	while(getline(in, line)){	
		lines++;
	}
	cout <<"Lines = " << lines << endl;


	in.close();

	SetBlocksHitOrMiss();

	Summary();

	return 0;
}

//Block Blocks[SetsInBlock];

void Summary(){
	cout << endl;
	cout << endl;
	cout <<"Cashe Configuration" << endl;
	cout <<endl;
	cout <<" 8 1-way set asociative entries" << endl;
	cout <<"of line size 8 bytes" << endl;
	cout << endl;
	cout << endl;
	cout <<"Results for Each Reference" << endl;
	cout << endl;
	cout <<"Ref   Address   Tag   Index Offset Result" << endl;
	cout <<"---- -------- ------- ----- ------ ------" << endl;
	int Total = 0;
	for(int i = 0; i < lines; i++){
		Total++;
		cout <<i << "         " << Blocks[i].Address << "     " << Blocks[i].GetTag() 
		<<  "     " << Blocks[i].GetIndex() << "        " << Blocks[i].GetOffset() <<
		"     " << Blocks[i].GetResult() <<  "     " << Blocks[i].Binary << endl;
	}
	cout << endl;
	cout <<"Simulation Summary Statistics" << endl;
	cout <<"-------------------------------" << endl;
	cout <<"Total Hits        :" << Hits << endl;
	cout <<"Total misses      :" << Accesses -Hits <<endl;
	cout <<"Total Accesses    :" << Accesses << endl;
	cout <<"Hit Ratio         :" << Hits / Accesses << endl;
	cout <<"Miss Ratio        :"<< ((Accesses- Hits) / Accesses) << endl;
	cout << endl;
}
string HexToBin(string hex)
{
    string Binary;
 
    for(int i =0; i< hex.length(); i++) {
 
        switch (hex[i]) {
        case '0':
            Binary+= "0000";
            break;
        case '1':
            Binary+= "0001";
            break;
        case '2':
            Binary+= "0010";
            break;
        case '3':
            Binary+= "0011";
            break;
        case '4':
            Binary+= "0100";
            break;
        case '5':
            Binary+= "0101";
            break;
        case '6':
            Binary+= "0110";
            break;
        case '7':
            Binary+= "0111";
            break;
        case '8':
            Binary+= "1000";
            break;
        case '9':
            Binary+= "1001";
            break;
        case 'A':
        case 'a':
            Binary+= "1010";
            break;
        case 'B':
        case 'b':
            Binary+= "1011";
            break;
        case 'C':
        case 'c':
            Binary+= "1100";
            break;
        case 'D':
        case 'd':
            Binary+= "1101";
            break;
        case 'E':
        case 'e':
            Binary+= "1110";
            break;
        case 'F':
        case 'f':
            Binary+= "1111";
            break;
        default:
            cout << "\nInvalid hexadecimal digit " << endl;
        }
    }
	return Binary;
}

int binaryToDecimal(int n)
{
    int num = n;
    int dec_value = 0;
 
    // Initializing base value to 1, i.e 2^0
    int base = 1;
 
    int temp = num;
    while (temp) {
        int last_digit = temp % 10;
        temp = temp / 10;
 
        dec_value += last_digit * base;
 
        base = base * 2;
    }
 
    return dec_value;
}
void HitOrMiss(int current){
		for(int i = 0; i < Accesses-1; i++){
			if(Blocks[current].Index == Blocks[i].Index && Blocks[current].Tag == Blocks[i].Tag){
				Hits++;
				Blocks[current].Result = 1;
				return;
			}
		}
		Blocks[current].Result = 0;
}

void SetBlocksHitOrMiss(){
		ifstream in;
		in.open("trace.dat");
		int current = 0;

		while(getline(in, line)){
		Blocks[current].SetBlock(line);
		HitOrMiss(current);
		current++;
		}

		in.close();
}
