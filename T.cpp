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
string Whiten(string ptblock, string key);
//void encrypt();

void encrypt()
{
  string choice = " ";
//  string tempS;
  string hex_whitenedString;
  string str;
  string whitened;
  string key_fromfile;
  string ciphertext;


/*
  cout << "Would you like to use your own key.txt file? Type Y or N" << endl;
  cin >> choice;
  if(choice != "y" && choice != "Y")
  {
    cout << "Key generated and written to key.txt file" << endl;
    Generate_Key(); // Will geenerate key and place in key.txt file
  }
  else
  {
    cout << "Make sure ket.txt file is in same directory and contains key in hex" << endl;
    cout << "Press c then ENTER to continue" << endl;
    cin >> choice;
  }
*/

  ifstream inFile; // plaintext.txt
  ifstream inFile2; // key.txt
  ofstream outFile; // cyphertext.txt

  inFile.open("plaintext.txt");
  inFile2.open("key.txt");
  outFile.open("cyphertext.txt"); //Will be cyphertext.txt, an encrypted file

  inFile >> str; //contents of plaintext.txt in ASCII
////
  int pTxtLng =  0;
  pTxtLng = str.length();
  if(pTxtLng % 16 != 0)
  {
    cout << "This will require padding" << endl;
  }
  cout << endl << "Length of input string is: " << pTxtLng << endl;
/////
  inFile2 >> key_fromfile; // contents of key.txt
  cout << endl << "Plaintext in ascii: " << str << endl;

/*
for(int i = 0; i < pTxtLng; i+=8)
{
  char w0 [2];
  char w1 [2];
  char w2 [2];
  char w3 [2];

  w0 [0] = str[i + 0];
  w0 [1] = str[i + 1];
  w1 [0] = str[i + 2];
  w1 [1] = str[i + 3];
  w2 [0] = str[i + 4];
  w2 [1] = str[i + 5];
  w3 [0] = str[i + 6];
  w3 [1] = str[i + 7];


//}


/*
  char tmpArr [8];
  char tmpArrcpy [pTxtLng] = {' '};



  for(int i = 0; i < pTxtLng; i+= 8)
  {

    for(int j = 0; j < 8; j++)
    {
      tmpArr[j] = str[i];
      tmpArrcpy[j] = str[i];
      i++;
    }

  string tempS(tmpArr);
  cout << "string tempS(tmpArr): " << tempS << endl;

  cout << "All contents of tempArr: ";
  for(int q = 0; q < pTxtLng; q++)
  {
    cout << tmpArrcpy[q];
  }
  cout << endl << endl;
*/
  string tempS(str);
  char2hex(tempS, tempS); //tempS becomes hex version of plaintext

  cout << "Plaintext in hex: " << tempS << endl;
  cout << "Key_fromfile in hex is: " << key_fromfile << endl;
//____________________________________________//
  whitened = Whiten(str, key_fromfile); // 64bits (8 words*8 bits each)XORD P+K=R in string
  cout << endl << "Whitened 64 bit string: " << endl << whitened << endl;
//____________________________________________//
  hex_whitenedString = bin2hex(whitened);
  cout << "Whitened string in hex: " << hex_whitenedString << endl;

  ciphertext = ciphertext + hex_whitenedString;

//  }


  outFile << whitened; //

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
///////////////////////////////////////////////////////////////////////////////////
//Takes plaintext in ASCII from plaintext.txt and a key in hex
//from key.txt and returns XORD [128]
string Whiten(string ptblock, string key)
{
  string pbin = hex2bin(ptblock);
  string kbin = hex2bin(key);
  char k[65];
  char w[65];
  char r[65];
  cout << endl;
//  cout << "ptblock in ascii: " << ptblock << endl;
//  cout << "pbin: " << pbin << endl;
//  cout << "Key: " << key << endl;
//  cout << "kbin: " << kbin << endl;

  strcpy(k, kbin.c_str());
  strcpy(w, pbin.c_str());

  for(int i = 0; i < 64; i++)
  {
    if(k[i] == w[i])
    {
      r[i] = '0';
    }
    else
    {
      r[i] = '1';
    }
//    cout << "k[i] " << k[i] << " XOR w[i] " << w[i] << " = " << r[i] << endl;
  }


  string result(r);
  cout << "  " << pbin << endl;
  cout << "  " << kbin << endl;
  cout << "  " << result << endl << endl;
//  cout << "RESULT string plaintext XORD with key =: " << endl << result << endl;

  return result;
}

//////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////////

//Will print the binary to screen
string hex2bin(string in)
{
  int i = 0;
  int h = 0;
  char binArray[65];

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

