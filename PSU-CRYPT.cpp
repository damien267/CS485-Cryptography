//Damien Jones
//CS 485 Project 1


#include <iostream>
#include <fstream>
#include <string.h>
#include <bitset>
#include <time.h>
#include <stdlib.h>
#include <vector>

using namespace std;


//This function was borrowed, but I cant remember where
void char2hex(const string in, string& out, bool capital = false)
{
  out.resize(in.size() *2);
  const size_t a = capital ? 'A' - 1 : 'a' - 1;

  for(size_t i = 0, c = in[0] & 0xFF; i < out.size(); c = in[i / 2] & 0xFF)
  {
    out[i++] = c > 0x9F ? (c/ 16-9) | a : c / 16 | '0';
    out[i++] = (c & 0xF) > 9 ? (c % 16 - 9) | a : c % 16 | '0';
  }
}

string hex2char(string in);
string bin2hex(string in);
string hex2bin(string in);
string Generate_Key();
string G(string w, int round_num);
string Whiten(string ptblock, string key);
string xOR(string k, string w, int arrsize);
string get_fTable ();
//void encrypt();
string K(unsigned int x);  // K(x)
string get_ftable_hex(string hex_in);
unsigned int hex2int(char hex_in);

char key_in[64]; //So that the key can be incremented to k'
char tmp; // To hold the one element that gets written over on the shift
static unsigned int f_count = 0; //To only load from key.txt initially, then use updated global val
int round_num = 0;


string F(string r0, string r1, int round_num)
{
  string t0(G(r0, round_num));
  string t1(G(r1, round_num));
  string f0;
  string f1;

//  t0 = G(r0, round_num);
//  t1 = G(r1, round_num);
//  cout << "t0: " << t0 << endl;
//  cout << "t1: " << t1 << endl;


  string result(t0 + t1);
  return result;
}

string G(string w, int round_num)
{
  char g1[9];
  char g2[9];

  for(int i = 0; i < 8; i++)
  {
    g1[i] = w[i];
    g2[i] = w[i+8];
  }
  cout << "In G(): " << endl;
  cout << "w in: " << w << endl;

  cout << "g1:     " << bin2hex(g1) << endl;
  cout << "g2:     " << bin2hex(g2) << endl;


  string tmp_k = K(4* round_num);
  cout << "K(4* rnd): " << bin2hex(tmp_k) << endl;

  string tmp_k2 = K((4* round_num) + 1);
  cout << "K(4* rnd+1): " << bin2hex(tmp_k2) << endl;


  string temp_x = xOR(g2, tmp_k, 8);

  cout << "xOR(g2, K(4* rndnum), 8): " << bin2hex(temp_x) << endl;
  string g3 = xOR(g1, get_ftable_hex(temp_x), 8);
  cout << "g3: " << bin2hex(g3) << endl;

  string temp_x2 = xOR(g3, tmp_k2, 8);

  cout << "xOR(g3, K(4* rndnum+1), 8): " << bin2hex(temp_x2) << endl;
  string g4 = xOR(g2, get_ftable_hex(temp_x2), 8);
  cout << "g4: " << bin2hex(g4) << endl;

  string temp_x3 = xOR(g4, K((4* round_num) + 2), 8);

  cout << "xOR(g4, K(4* rndnum+2), 8): " << bin2hex(temp_x3) << endl;
  string g5 = xOR(g3, get_ftable_hex(temp_x3), 8);
  cout << "g5: " << bin2hex(g5) << endl;


  cout << "ftable input in hex: " << bin2hex(temp_x) << endl;




  string g5g6 = "HELLO";
//  cout << "get_ftable_hex 7a: " << get_ftable_hex("7a") << endl;
  unsigned int lookup = 0;

  return g5g6;
}


void encrypt()
{
  string choice = " ";
//  string tempS;
  string hex_whitenedString;
  string str;
  string whitened;
  string key_fromfile;
  string ciphertext;


  ifstream inFile; // plaintext.txt
  ifstream inFile2; // key.txt
  ofstream outFile; // cyphertext.txt
  inFile.open("plaintext.txt");
  inFile2.open("key.txt");
  outFile.open("cyphertext.txt"); //Will be cyphertext.txt, an encrypted file

  inFile >> str; //contents of plaintext.txt in ASCII
  inFile2 >> key_fromfile; // contents of key.txt
////
  int pTxtLng =  0;
  pTxtLng = str.length();
  if(pTxtLng % 8 != 0)
  {
    cout << "*******This will require padding*******" << endl;
  }
//  cout << endl << "Length of input string is: " << pTxtLng << endl;
//  cout << endl << "Plaintext in ascii (str) in : " << str << endl;

//  char2hex(str,str); // Needed when plaintext.txt is in ascii
  string pBin = hex2bin(str);

//  cout << "key_fromfile = " << key_fromfile << endl;
//  cout << "hex2bin(str)      = " << pBin << endl;

  string kBin = hex2bin(key_fromfile);
//  cout << "kBin              = " << kBin << endl;

  string r = str;
  char w0[17] = { };
  char w1[17] = { };
  char w2[17] = { };
  char w3[17] = { };
//  char r[64];
  char r0[17] = { };
  char r1[17] = { };
  char r2[17] = { };
  char r3[17] = { };
  char k0[17] = { };
  char k1[17] = { };
  char k2[17] = { };
  char k3[17] = { };


//str is input string in hex
//key is input in hex

  unsigned int n = 0;
//-------------------------------------------------------------
  n = round_num;
//  string key_byte = K(n);
//  cout << endl << "K2 0: " <<  key_byte << endl;
//  string key_byte1 = K(n+1);
//  cout << "K2 1: " <<  key_byte1 << endl;
//---------------------------------------------------------------
  for(int i = 0; i < 16; i++) // START 16 rounds
  {

    if(round_num == 0)
    {
      str = Whiten(pBin, kBin); // 64bits (8 words*8 bits each)XORD P+K=R in string
//      cout << endl << "Whitened 64 bit string: " << endl << str << endl;
//      hex_whitenedString = bin2hex(str);
//      cout << "Whitened string in hex: " << hex_whitenedString << endl;

      for(int j = 0; j < 16; j++)
      {
        r0[j] = str[j];
        r1[j] = str[j+16];
        r2[j] = str[j+32];
        r3[j] = str[j+48];
//        cout << r0[j] << " " << r1[j] << " " << r2[j] << " " << r3[j] << endl;
      }
    }

    cout << "Key out put: " << K(i) << endl;


//  cout << "going into F " << endl;
//  cout << "r0: " << r0 << endl;
//  cout << "r1: " << r1 << endl;
//  cout << "rnd:" << round_num << endl;
  cout << endl << "Spacer----------------------" << endl;

  F(r0, r1, round_num);








//  cout << endl << "K" << i << " " << round_num << ": " <<  key_byte << endl;

//*&^*&^*^*^*^*^(^%&^%*%%*%%(%%(%^%$^#$%^&^%#&$%^&(*(^*(%$^&*^%#@$@$^&%*&%&*$^&$*

//    cout << "str after round " << i << ": " << str << endl;
//    str = xOR(str, key_fromfile);
//    cout << "str after round 0: " << str << endl;
    round_num++;
  }
//  outFile << whitened; //
  inFile.close();
  inFile2.close();
  outFile.close();

}

////////////////////////////////-MAIN-////////////////////////////////////////
int main()
{

  encrypt();

cout << endl;
return 0;
}
//////////////////////////////////////////////////////////////////////////


string K(unsigned int x)
{
  unsigned int new_x = x % 8;
  char out[9]= { };

  if(f_count == 0)
  {
    ifstream inFile;
    inFile.open("key.txt");
    string kHex = "";
    inFile >> kHex;
    string kbin = hex2bin(kHex);

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


  //Select the correct bytes depending dependin on the x mod 8 input
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

//Generates key and saves to file key.txt
string Generate_Key()
{
  srand(time(NULL));
  char key_stream[9] = {'\0'};
  char char_set[36] = {'Q','W','E','R','T','Y','1','U','I','O','P','2','A','S','D','0','F','G','3','H','J','K','L','4','5','6','Z','X','C','7','V','B','N','8','M','9'};

  //Each of the 16 chars for the key are chosen randomly, individually
  for(int i = 0; i < 8; i++)
  {
    int r_select = rand() % 36;
    key_stream[i] = char_set[r_select];

// For testing: shows keystream and charset match, and the random number used
//    cout << i << ". " << "key stream = " << key_stream[i] << "  charset = " << char_set[r_select] << "  r-selct = " << r_select << '\n';
  }

  //Convert the char array to a string so it can be returned
  string result(key_stream);

  cout << endl;
  cout << "****KEY in ASCII is: " << result << endl;

  ifstream inFile;
  ofstream outFile;
  inFile.open("key.txt");
  outFile.open("key.txt");

  char2hex(result, result);
  cout << "****KEY in HEX is: " << result << '\n';
  outFile << result;
//  cout << "KEY BACK INTO ascii: " << hex2char(result) << endl;

  string ddd = hex2char(result);
  inFile.close();
  outFile.close();

  return result;
}



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
//    cout << "r[" << i << "] = " << r[i] << endl;
//    cout << "r[64] = " << r[64] << endl;
  }
  string result(r);
//  cout << "###############" << result << endl;
  return result;
}

///////////////////////////////////////////////////////////////////////////////////
//Takes plaintext in ASCII from plaintext.txt and a key in hex
//from key.txt and returns XORD [128]
string Whiten(string ptblock, string key)
{
  string pbin = ptblock;
  string kbin = key;
  char k[64] = {" "};
  char w[64] = {" "};
  strcpy(k, key.c_str());
  strcpy(w, ptblock.c_str());
  string result(xOR(key, ptblock, 65));
  return result;
}

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
  char binArray[6500]; // <--- CHANGE THIS BACK -----------------------

//  cout << '\n' << "string in: " << in << endl;

  while(in[h])
  {
    if(in[h] == '0')
    {
      binArray[i] = '0';
      binArray[i+1] = '0';
      binArray[i+2] = '0';
      binArray[i+3] = '0';
//      cout << binArray[i] << binArray[i+1] << binArray[i+2] << binArray[i+3];
    }
    else if(in[h] == '1')
    {
      binArray[i] = '0';
      binArray[i+1] = '0';
      binArray[i+2] = '0';
      binArray[i+3] = '1';
//      cout << binArray[i] << binArray[i+1] << binArray[i+2] << binArray[i+3];
    }
    else if(in[h] == '2')
    {
      binArray[i] = '0';
      binArray[i+1] = '0';
      binArray[i+2] = '1';
      binArray[i+3] = '0';
//      cout << binArray[i] << binArray[i+1] << binArray[i+2] << binArray[i+3];
    }
    else if(in[h] == '3')
    {
      binArray[i] = '0';
      binArray[i+1] = '0';
      binArray[i+2] = '1';
      binArray[i+3] = '1';
//      cout << binArray[i] << binArray[i+1] << binArray[i+2] << binArray[i+3];
    }
    else if(in[h] == '4')
    {
      binArray[i] = '0';
      binArray[i+1] = '1';
      binArray[i+2] = '0';
      binArray[i+3] = '0';
//      cout << binArray[i] << binArray[i+1] << binArray[i+2] << binArray[i+3];
    }
    else if(in[h] == '5')
    {
      binArray[i] = '0';
      binArray[i+1] = '1';
      binArray[i+2] = '0';
      binArray[i+3] = '1';
//      cout << binArray[i] << binArray[i+1] << binArray[i+2] << binArray[i+3];
    }
    else if(in[h] == '6')
    {
      binArray[i] = '0';
      binArray[i+1] = '1';
      binArray[i+2] = '1';
      binArray[i+3] = '0';
//      cout << binArray[i] << binArray[i+1] << binArray[i+2] << binArray[i+3];
    }
    else if(in[h] == '7')
    {
      binArray[i] = '0';
      binArray[i+1] = '1';
      binArray[i+2] = '1';
      binArray[i+3] = '1';
//      cout << binArray[i] << binArray[i+1] << binArray[i+2] << binArray[i+3];
    }
    else if(in[h] == '8')
    {
      binArray[i] = '1';
      binArray[i+1] = '0';
      binArray[i+2] = '0';
      binArray[i+3] = '0';
//      cout << binArray[i] << binArray[i+1] << binArray[i+2] << binArray[i+3];
    }
    else if(in[h] == '9')
    {
      binArray[i] = '1';
      binArray[i+1] = '0';
      binArray[i+2] = '0';
      binArray[i+3] = '1';
//      cout << binArray[i] << binArray[i+1] << binArray[i+2] << binArray[i+3];
    }
    else if(in[h] == 'a' || in[h] == 'A') //10
    {
      binArray[i] = '1';
      binArray[i+1] = '0';
      binArray[i+2] = '1';
      binArray[i+3] = '0';
//      cout << binArray[i] << binArray[i+1] << binArray[i+2] << binArray[i+3];
    }
    else if(in[h] == 'b' || in[h] == 'B') //11
    {
      binArray[i] = '1';
      binArray[i+1] = '0';
      binArray[i+2] = '1';
      binArray[i+3] = '1';
//      cout << binArray[i] << binArray[i+1] << binArray[i+2] << binArray[i+3];
    }
    else if(in[h] == 'c' || in[h] == 'C') //12
    {
      binArray[i] = '1';
      binArray[i+1] = '1';
      binArray[i+2] = '0';
      binArray[i+3] = '0';
//      cout << binArray[i] << binArray[i+1] << binArray[i+2] << binArray[i+3];
    }
    else if(in[h] == 'd' || in[h] == 'D') //13
    {
      binArray[i] = '1';
      binArray[i+1] = '1';
      binArray[i+2] = '0';
      binArray[i+3] = '1';
//      cout << binArray[i] << binArray[i+1] << binArray[i+2] << binArray[i+3];
    }
    else if(in[h] == 'e' || in[h] == 'E') //14
    {
      binArray[i] = '1';
      binArray[i+1] = '1';
      binArray[i+2] = '1';
      binArray[i+3] = '0';
//      cout << binArray[i] << binArray[i+1] << binArray[i+2] << binArray[i+3];
    }
    else if(in[h] == 'f' || in[h] == 'F') //15
    {
      binArray[i] = '1';
      binArray[i+1] = '1';
      binArray[i+2] = '1';
      binArray[i+3] = '1';
//      cout << binArray[i] << binArray[i+1] << binArray[i+2] << binArray[i+3];
    }
    h++;
    i += 4;
  }
/*
  cout << '\n' << "Contents of binArray in Function: " << '\n';
  for(int j = 0; j < 128; j++)
  {
    cout << binArray[j];
  }
*/
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


string get_fTable()
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

string get_ftable_hex(string hex_in)
{

  unsigned int irow = hex2int(hex_in[0]);
  unsigned int icolumn = hex2int(hex_in[1]);
  char temp[3];
  string fTable = get_fTable();
  temp[0] = fTable[(irow*32) + (2* icolumn)];
  temp[1] = fTable[((irow*32) + (2* icolumn)) + 1];
  string result(temp);
  return result;
}

