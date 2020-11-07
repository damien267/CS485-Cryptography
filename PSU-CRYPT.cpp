/*******************************************************************
 * Damien Jones
 * CS 485 Project 1
 * Block cipher system utilizing key whitening and Feistel cipher
*******************************************************************/

#include <iostream>
#include <fstream>
#include <string.h>
#include <bitset>
#include <time.h>
#include <stdlib.h>
#include <vector>

using namespace std;


string hex2char(string in);
string bin2hex(string in);
string hex2bin(string in);
string Generate_Key();
string G(string w, int round_num);
string Whiten(string ptblock, string key);
string xOR(string k, string w, int arrsize);
string import_fTable ();
string K(unsigned int x);  // K(x)
string get_ftable_hex(string hex_in);
void Encrypt();
void char2hex(const string in, string& out);
int round_num = 0;
unsigned int hex2int(char hex_in);
char key_in[64]; //So that the key can be incremented to k'
char tmp; // To hold the one element that gets written over on the shift
static unsigned int f_count = 0; // Count of '0' used to load from key.txt initially, then use updated global val


/*******************************************************************
 * F function
 * Gets the left half of the 16 byte block
*******************************************************************/

string F(string l0, string l1, int round_num)
{
  string t0(G(l0, round_num));
  string t1(G(l1, round_num));
  string f0;
  string f1;

  cout << "t0: " << t0 << endl;
  cout << "t1: " << t1 << endl;
  cout << "l0: " << l0 << endl;
  cout << "l1: " << l1 << endl;


  string result(t0 + t1);
  return result;
}

/*******************************************************************
 * G function

 * Takes 2 bytes as input
 * Loads each byte into g1 and g2 - size of 9 for /n

 * So g1 is first byte of l0 or l1 (2 bytes)
*******************************************************************/

string G(string w, int round_num)
{
  char g1[9]; // Byte 1
  char g2[9]; // Byte 2

  for(int i = 0; i < 8; i++)
  {
    g1[i] = w[i];
    g2[i] = w[i+8];
  }

/***   -->  COUTS  <-- *********************************************
  cout << "In G(): " << endl;
  cout << "w in- 2 bytes of binary, l0 or l1: " << w << endl;
  cout << "g1- Byte 1:     " << g1 << endl;
  cout << "g2 - Byte 2:     " << g2 << endl;
  cout << "g1- Byte 1 in hex:     " << bin2hex(g1) << endl;
  cout << "g2- Byte 2 in hex:     " << bin2hex(g2) << endl << endl;
*******************************************************************/


/*******************************************************************
 * K function used here
 * Assign keys depending on round number
*******************************************************************/

  string tmp_k = K(4* round_num);
  string tmp_k2 = K((4* round_num) + 1);

  cout << "K(4* rnd) in hex: " << bin2hex(tmp_k) << endl;
  cout << "K(4* rnd): " << tmp_k << endl;
  cout << "K(4* rnd+1) in hex: " << bin2hex(tmp_k2) << endl;
  cout << "K(4* rnd+1): " << tmp_k2 << endl << endl;

/*******************************************************************
 * XOR bytes with key
 * PROBLEM
 * This part I cant remember the structure for the algorithm

** SOLUTION: Generate all keys in the begining and store. Just
** run the same calls on a seperate for loop (i < 16)
*******************************************************************/

  string temp_x = xOR(g2, tmp_k, 8);
  string g3 = xOR(g1, get_ftable_hex(temp_x), 8);
  string temp_x2 = xOR(g3, tmp_k2, 8);
  string g4 = xOR(g2, get_ftable_hex(temp_x2), 8);
  string temp_x3 = xOR(g4, K((4* round_num) + 2), 8);
  string g5 = xOR(g3, get_ftable_hex(temp_x3), 8);

/******** -->  COUTS  <-- ******************************************
  cout << "xOR(g2, K(4* rndnum), 8): " << bin2hex(temp_x) << endl;
  cout << "g3: " << bin2hex(g3) << endl;
  cout << "xOR(g3, K(4* rndnum+1), 8): " << bin2hex(temp_x2) << endl;
  cout << "g4: " << bin2hex(g4) << endl;
  cout << "xOR(g4, K(4* rndnum+2), 8): " << bin2hex(temp_x3) << endl;
  cout << "g5: " << bin2hex(g5) << endl;
  cout << "ftable input in hex: " << bin2hex(temp_x) << endl;
*******************************************************************/

/*******************************************************************
 * get_ftable_hex isnt returning a value
 * PROBLEM
*******************************************************************/

  //string test_ftable_lookup = bin2hex(temp_x);
  //string test_ftable_lookup2 = "7a";
  //cout << "get_ftable_hex 7a: " << get_ftable_hex("7a") << endl;
  //cout << "get_ftable_hex (temp_x): " << get_ftable_hex(test_ftable_lookup2) << endl;

  string g5g6 = "HELLO"; // <------------JUST TO RETURN A STRING. This will be deleted
  return g5g6;
}


/*******************************************************************
 * Encrypt()
 * PROBLEM
*******************************************************************/

void Encrypt()
{
  string choice = " ";
//  string tempS;
  string hex_whitenedString;
  string str;
  string whitened;
  string key_fromfile;
  string ciphertext;
  int pTxt_Length =  0;


  ifstream plaintext_txt; // plaintext.txt
  ifstream inFile2; // key.txt
  ofstream outFile; // cyphertext.txt
  plaintext_txt.open("plaintext.txt");
  inFile2.open("key.txt");
  outFile.open("cyphertext.txt"); //Will be cyphertext.txt, an encrypted file

  plaintext_txt >> str; //contents of plaintext.txt in ASCII
  inFile2 >> key_fromfile; // contents of key.txt
//  char2hex(str,str); // Needed when plaintext.txt is in ascii
  string pBin = hex2bin(str);
  string kBin = hex2bin(key_fromfile);

  pTxt_Length = str.length();
  if(pTxt_Length % 64 != 0)
  {
    cout << "*******This will require padding*******" << endl;
  }

  string r = str;
  char w0[17] = { };
  char w1[17] = { };
  char w2[17] = { };
  char w3[17] = { };
  char l0[17] = { };
  char l1[17] = { };
  char r0[17] = { };
  char r1[17] = { };
  char k0[17] = { };
  char k1[17] = { };
  char k2[17] = { };
  char k3[17] = { };
  unsigned int n = 0;
  n = round_num;


  for(int i = 0; i < 1; i++) // START 16 rounds <-- ********SET ROUNDS HERE ********************************
  {

    if(round_num == 0)
    {
      str = Whiten(pBin, kBin); // 64bits (8 words*8 bits each)XORD P+K=R in string

/*******************************************************************
 * Loads and splits first 64 bits of plaintext binary into 4 arrays
 * l0-r1 each contain 2 bytes (16 bits)
*******************************************************************/
      for(int j = 0; j < 16; j++)
      {
        l0[j] = str[j];
        l1[j] = str[j+16];
        r0[j] = str[j+32];
        r1[j] = str[j+48];
      }
    }

  F(l0, l1, round_num);

  round_num++;
  }
  outFile << whitened; //
  plaintext_txt.close();
  inFile2.close();
  outFile.close();
}


/*******************************************************************
 * Main()
*******************************************************************/

int main()
{
  Generate_Key();
  Encrypt();

cout << endl;
return 0;
}

/*******************************************************************
 * K function:
 * Returns 1 byte subkey
*******************************************************************/

string K(unsigned int x)
{
  unsigned int new_x = x % 8;
  char out[9]= { };


  if(f_count == 0)
  {
    // Get key from key.txt
    ifstream inFile;
    inFile.open("key.txt");
    string kHex = "";
    inFile >> kHex;
    string kbin = hex2bin(kHex); // kbin is key from key.txt in binary

    for(int i = 0; i < 64; i++)  //transfer kbin to key_in
    {
      key_in[i] = kbin[i];
    }
    inFile.close();
  }

  //Rotate by one bit, produce K'
  tmp = key_in[0];
  for(int j = 0; j < 63; j++)
  {
   key_in[j] = key_in[j+1];
  }
  key_in[63] = tmp;


  //Select the correct bytes depending on the x mod 8 input
  if(new_x == 0)
  {
   for(int a = 0; a < 8; a++)
    {
      out[a] = key_in[a];
    }
  }
  if(new_x == 1)
  {
   for(int a = 0; a < 8; a++)
    {
      out[a] = key_in[a+8];
    }
  }
  if(new_x == 2)
  {
   for(int a = 0; a < 8; a++)
    {
      out[a] = key_in[a+16];
    }
  }
  if(new_x == 3)
  {
   for(int a = 0; a < 8; a++)
    {
      out[a] = key_in[a+24];
    }
  }
  if(new_x == 4)
  {
   for(int a = 0; a < 8; a++)
    {
      out[a] = key_in[a+32];
    }
  }
  if(new_x == 5)
  {
   for(int a = 0; a < 8; a++)
    {
      out[a] = key_in[a+40];
    }
  }
  if(new_x == 6)
  {
   for(int a = 0; a < 8; a++)
    {
      out[a] = key_in[a+48];
    }
  }
  if(new_x == 7)
  {
   for(int a = 0; a < 8; a++)
    {
      out[a] = key_in[a+56];
    }
  }
//  cout << "key byte chosen: " << new_x << ": " << out << endl; // new_x is 4*round_num % 8
  f_count++; // Used to make sure only the intial key is loaded from file
  string result(out);
  return result; // Returns 8 bit subkey
}


/*******************************************************************
 * Generate key:
 *
*******************************************************************/

string Generate_Key()
{
  srand(time(NULL));
  char key_stream[9] = {'\0'};
  char char_set[36] = {'Q','W','E','R','T','Y','1','U','I','O','P','2','A','S','D','0','F','G','3','H','J','K','L','4','5','6','Z','X','C','7','V','B','N','8','M','9'};

  //Each of the 8 chars for the key are chosen individually and randomly
  for(int i = 0; i < 8; i++)
  {
    int r_select = rand() % 36;
    key_stream[i] = char_set[r_select];
  }

  //Convert the char array to a string so it can be returned
  string result(key_stream);

  cout << endl;
  cout << "****KEY in ASCII is: " << result << endl;

  ifstream key_txt_in;
  ofstream key_txt_out;
  key_txt_in.open("key.txt");
//  key_txt_out.open("key.txt");
  key_txt_out.open("Gen_Key.txt");

  char2hex(result, result);
  cout << "****KEY in HEX is: " << result << '\n';
  key_txt_out << result;

  key_txt_in.close();
  key_txt_out.close();

  return result;
}


/*******************************************************************
 * XOR
*******************************************************************/

string xOR(string k, string w, int arrsize)
{
  char r[arrsize+1] = {" "};
  for(int i = 0; i < arrsize; i++)
  {
    if(k[i] == w[i])
    {
      r[i] = '0';
    }
    else
    {
      r[i] = '1';
    }
  }
  string result(r);
  return result;
}

/*******************************************************************
 * Takes plaintext in ASCII from plaintext.txt and a key in hex
 * from key.txt and returns XORD [128]
*******************************************************************/

string Whiten(string ptblock, string key)
{
  char k[64] = {" "};
  char w[64] = {" "};
  strcpy(k, key.c_str());
  strcpy(w, ptblock.c_str());
  string result(xOR(key, ptblock, 65));
  return result;
}

/*******************************************************************
 * f_table functions:
 * import_fTable removes spaces, commas, and '0x' in "f_table.txt"
*******************************************************************/

string import_fTable()
{
  string f_table;
  char new_FT[10000];
  ifstream inFile;
  inFile.open("f_table.txt");
  inFile >> f_table;
  int flen = f_table.length();
  int t = 0;

  for(int i = 0; i < flen; i++)
  {
    if(f_table[i] == ' ' || f_table[i] == ',')
    {
      i++;
    }
    if(f_table[i] == '0' && f_table[i+1] == 'x')
    {
      i += 2;
    }
    new_FT[t] = f_table[i];
    t++;
  }
  inFile.close();
  return new_FT;
}

string get_ftable_hex(string hex_in)
{

  unsigned int irow = hex2int(hex_in[0]);
  unsigned int icolumn = hex2int(hex_in[1]);
  char temp[2];
  string fTable = import_fTable();
  temp[0] = fTable[(irow*32) + (2* icolumn)];
  temp[1] = fTable[((irow*32) + (2* icolumn)) + 1];
  string result(temp);

/*****     -->  COUTS  <---   ***********
  cout << "irow: " << irow << endl;
  cout << "icolumn: " << icolumn << endl;
  //cout << "fTable" << fTable << endl;
  cout << "temp[0]: " << temp[0] << endl;
  cout << "temp[1]: " << temp[1] << endl;
  cout << "result: " << result << endl;
*****************************************/

  return result;
}


/******************************************************
 * Conversion tables below:
 * hex2char
 * hex2bin
 * bin2hex
 * hex2int
 * char2hex
*******************************************************/

string hex2char(string in)
{
  string result;
  int length = in.length();
  for(int i = 0; i < length; i+=2)
  {
    string temp = in.substr(i,2);
    char chr = (char)(int)strtol(temp.c_str(), NULL, 16);
    result.push_back(chr);
  }
  return result;
}

string hex2bin(string in)
{
  int i = 0;
  int h = 0;
  char binArray[6500];

  while(in[h])
  {
    if(in[h] == '0')
    {
      binArray[i] = '0';
      binArray[i+1] = '0';
      binArray[i+2] = '0';
      binArray[i+3] = '0';
    }
    else if(in[h] == '1')
    {
      binArray[i] = '0';
      binArray[i+1] = '0';
      binArray[i+2] = '0';
      binArray[i+3] = '1';
    }
    else if(in[h] == '2')
    {
      binArray[i] = '0';
      binArray[i+1] = '0';
      binArray[i+2] = '1';
      binArray[i+3] = '0';
    }
    else if(in[h] == '3')
    {
      binArray[i] = '0';
      binArray[i+1] = '0';
      binArray[i+2] = '1';
      binArray[i+3] = '1';
    }
    else if(in[h] == '4')
    {
      binArray[i] = '0';
      binArray[i+1] = '1';
      binArray[i+2] = '0';
      binArray[i+3] = '0';
    }
    else if(in[h] == '5')
    {
      binArray[i] = '0';
      binArray[i+1] = '1';
      binArray[i+2] = '0';
      binArray[i+3] = '1';
    }
    else if(in[h] == '6')
    {
      binArray[i] = '0';
      binArray[i+1] = '1';
      binArray[i+2] = '1';
      binArray[i+3] = '0';
    }
    else if(in[h] == '7')
    {
      binArray[i] = '0';
      binArray[i+1] = '1';
      binArray[i+2] = '1';
      binArray[i+3] = '1';
    }
    else if(in[h] == '8')
    {
      binArray[i] = '1';
      binArray[i+1] = '0';
      binArray[i+2] = '0';
      binArray[i+3] = '0';
    }
    else if(in[h] == '9')
    {
      binArray[i] = '1';
      binArray[i+1] = '0';
      binArray[i+2] = '0';
      binArray[i+3] = '1';
    }
    else if(in[h] == 'a' || in[h] == 'A') //10
    {
      binArray[i] = '1';
      binArray[i+1] = '0';
      binArray[i+2] = '1';
      binArray[i+3] = '0';
    }
    else if(in[h] == 'b' || in[h] == 'B') //11
    {
      binArray[i] = '1';
      binArray[i+1] = '0';
      binArray[i+2] = '1';
      binArray[i+3] = '1';
    }
    else if(in[h] == 'c' || in[h] == 'C') //12
    {
      binArray[i] = '1';
      binArray[i+1] = '1';
      binArray[i+2] = '0';
      binArray[i+3] = '0';
    }
    else if(in[h] == 'd' || in[h] == 'D') //13
    {
      binArray[i] = '1';
      binArray[i+1] = '1';
      binArray[i+2] = '0';
      binArray[i+3] = '1';
    }
    else if(in[h] == 'e' || in[h] == 'E') //14
    {
      binArray[i] = '1';
      binArray[i+1] = '1';
      binArray[i+2] = '1';
      binArray[i+3] = '0';
    }
    else if(in[h] == 'f' || in[h] == 'F') //15
    {
      binArray[i] = '1';
      binArray[i+1] = '1';
      binArray[i+2] = '1';
      binArray[i+3] = '1';
    }
    h++;
    i += 4;
  }
  string binresult(binArray);
  return binresult;
}

string bin2hex(string in)
{
  char temp[33] = {' '};
  int counter = 0;

  for(int i = 0; i <= 128; i+=4)
  {
    if(in[i] == '0' && in[i+1] == '0' && in[i+2] == '0' && in[i+3] == '0')
    {
      temp[counter] = '0';
    }
    else if(in[i] == '0' && in[i+1] == '0' && in[i+2] == '0' && in[i+3] == '1')
    {
      temp[counter] = '1';
    }
    else if(in[i] == '0' && in[i+1] == '0' && in[i+2] == '1' && in[i+3] == '0')
    {
      temp[counter] = '2';
    }
    else if(in[i] == '0' && in[i+1] == '0' && in[i+2] == '1' && in[i+3] == '1')
    {
      temp[counter] = '3';
    }
    else if(in[i] == '0' && in[i+1] == '1' && in[i+2] == '0' && in[i+3] == '0')
    {
      temp[counter] = '4';
    }
    else if(in[i] == '0' && in[i+1] == '1' && in[i+2] == '0' && in[i+3] == '1')
    {
      temp[counter] = '5';
    }
    else if(in[i] == '0' && in[i+1] == '1' && in[i+2] == '1' && in[i+3] == '0')
    {
      temp[counter] = '6';
    }
    else if(in[i] == '0' && in[i+1] == '1' && in[i+2] == '1' && in[i+3] == '1')
    {
      temp[counter] = '7';
    }
    else if(in[i] == '1' && in[i+1] == '0' && in[i+2] == '0' && in[i+3] == '0')
    {
      temp[counter] = '8';
    }
    else if(in[i] == '1' && in[i+1] == '0' && in[i+2] == '0' && in[i+3] == '1')
    {
      temp[counter] = '9';
    }
    else if(in[i] == '1' && in[i+1] == '0' && in[i+2] == '1' && in[i+3] == '0')
    {
      temp[counter] = 'a'; //10
    }
    else if(in[i] == '1' && in[i+1] == '0' && in[i+2] == '1' && in[i+3] == '1')
    {
      temp[counter] = 'b'; //11
    }
    else if(in[i] == '1' && in[i+1] == '1' && in[i+2] == '0' && in[i+3] == '0')
    {
      temp[counter] = 'c'; //12
    }
    else if(in[i] == '1' && in[i+1] == '1' && in[i+2] == '0' && in[i+3] == '1')
    {
      temp[counter] = 'd'; //13
    }
    else if(in[i] == '1' && in[i+1] == '1' && in[i+2] == '1' && in[i+3] == '0')
    {
      temp[counter] = 'e'; //14
    }
    else if(in[i] == '1' && in[i+1] == '1' && in[i+2] == '1' && in[i+3] == '1')
    {
      temp[counter] = 'f'; //15
    }
    counter++;
  }
  string result(temp);
  return result;
}

unsigned int hex2int(char hex_in)
{
  unsigned int num_out = 0;
  if(hex_in == '0') num_out = 0;
  if(hex_in == '1') num_out = 1;
  if(hex_in == '2') num_out = 2;
  if(hex_in == '3') num_out = 3;
  if(hex_in == '4') num_out = 4;
  if(hex_in == '5') num_out = 5;
  if(hex_in == '6') num_out = 6;
  if(hex_in == '7') num_out = 7;
  if(hex_in == '8') num_out = 8;
  if(hex_in == '9') num_out = 9;
  if(hex_in == 'a' || hex_in == 'A') num_out = 10;
  if(hex_in == 'b' || hex_in == 'B') num_out = 11;
  if(hex_in == 'c' || hex_in == 'C') num_out = 12;
  if(hex_in == 'd' || hex_in == 'D') num_out = 13;
  if(hex_in == 'e' || hex_in == 'E') num_out = 14;
  if(hex_in == 'f' || hex_in == 'F') num_out = 15;

  return num_out;
}

//This function was borrowed
void char2hex(const string in, string& out)
{
  bool capital = false;
  out.resize(in.size() *2);
  const size_t a = capital ? 'A' - 1 : 'a' - 1;

  for(size_t i = 0, c = in[0] & 0xFF; i < out.size(); c = in[i / 2] & 0xFF)
  {
    out[i++] = c > 0x9F ? (c/ 16-9) | a : c / 16 | '0';
    out[i++] = (c & 0xF) > 9 ? (c % 16 - 9) | a : c % 16 | '0';
  }
}
