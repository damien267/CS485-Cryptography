//Damien Jones
//CS485 P2

#include<iostream>
#include<cstdlib>
#include<ctime>
#include<math.h>
#include<fstream>
#include<cstring>
#include<string>

using namespace std;
const double num_rounds = 10000;
bool check_primality(unsigned long num_in);
unsigned long gen_large_num();
string get_Ptext();
int power(int x, unsigned int y, int p); //https://www.geeksforgeeks.org/modular-exponentiation-power-in-modular-arithmetic/
unsigned long newseed = 0;
void Generate_Key(int seed);


void Generate_Key(int seed)
{


  unsigned long g = 2;
  unsigned long p, d;


  cout << "Enter a random integer to seed RNG: " << endl;
  cin >> seed;
  srand(seed);

//  cout << "Enter first prime integer" << endl;
//  cin >> p;
p = 1117; //<----------------------------------
//p = 11; //<----------------------------------
//  while(check_primality(p) == false)
//  {
//    cout << "That number is not prime, try again" << endl;
//    cin >> p;
//  }

//  cout << "Enter private key q (must be prime)" << endl;
//  cin >> d;
d = 3; //<------------------------------------
//  while(check_primality(d) == false)
//  {
//    cout << "That number is not prime, try again" << endl;
//    cin >> d;
//  }


  int public_key [5];
  int private_key [5];
  int private_key_int = d;

  //(p,g,e2)
  private_key[1] = ' ';
  private_key[3] = ' ';
  private_key[0] = p;
  private_key[2] = g;
  private_key[5] = d;


  //(p,g,d)
  public_key[0] = p;
  public_key[1] = g;
  public_key[2] = power(g,d,p);

  ifstream pubkey;
  ifstream prikey;
  ofstream pubkeyo;
  ofstream prikeyo;
  pubkey.open("pubkey.txt");
  prikey.open("prikey.txt");

  pubkeyo.open("pubkey.txt");
  prikeyo.open("prikey.txt");

  pubkeyo << public_key;
  prikeyo << private_key;

  pubkey.close();
  prikey.close();

  pubkeyo.close();
  prikeyo.close();


    cout << "start pub" << endl;



}

int main()
{

//  int i = 1019;
  unsigned long seed = 3;
  srand(time(NULL));
  long int message_ascii [10000] = {'\0'};

  string message_in = get_Ptext();
  int string_length = message_in.length();
  cout << "Input message: " << message_in << endl;

  for(int i = 0; message_in[i] != '\0'; i++)
  {
	message_ascii[i] = message_in[i] - 48;
        cout << "message in: " << message_in[i] << "  ascii value: " << message_ascii[i] << endl;
  }

  Generate_Key(seed);

  ifstream prikey_in;
  ifstream pubkey_in;
  prikey_in.open("prikey.txt");
  pubkey_in.open("pubkey.txt");

  string temp_pri;
  string temp_pub;

  prikey_in >> temp_pri; //[p, ' ', g , ' ' , d]
  pubkey_in >> temp_pub;

  int public_key [3];
  unsigned long g,d,p,e2 = 0;

  //Public key (p, g, e2) = (p, 2, power(g,d,p))
  p = temp_pub[0];
  g = temp_pub[2];
  e2= temp_pub[4];

  //Private key (p, g, d) where d is private key
  p = temp_pri[0];
  g = temp_pri[2];
  d = temp_pri[4];

  prikey_in.close();
  pubkey_in.close();

  //Encrypt

  //Alice chooses k = 4
  int k = 4;

  for(int i = 0; i < string_length; i++)
{

  unsigned long c1, c2 = 0;
  c1 = power(2,k,p);
  c2 = power((pow(power(2,d,p), k) * message_ascii[i]),1,p);
//  c2 = (4096 * 7) % p;
  cout << "message ascii in: " << message_ascii[i] <<  " c1: " << c1 << "  c2: " << c2 << endl;

  int numnum = (power(c1,(p-1-d), p) * c2) % p;
//  numnum = (numnum * c2) % p;
  cout << "Dcecrypted with c1 and c2: " << numnum  << endl;
//  char temp_numnum = (numnum + 48);
//  cout << "temp numnum " << temp_numnum << endl;
//k++;
}


//  cout << "prikey: " << public_key[0] << "  " << public_key[1] << "  " << public_key[2] << endl;

//Generate safe prime p so that 2 is a generator for mod p
  unsigned long temp = 0;
  for(int i = 0; i < 2000; i++)
  {

    srand(seed+ i);
    temp = gen_large_num();
    if(temp % 12 == 5)
    {
	unsigned long p = 0;
	p = (2 * temp) + 1;

//	cout << "checking " << p << endl;
	if(check_primality(p))
	{
          cout << "Prime number found is: " << p << endl;
	  return true;
	}
//  cout << "No prime found" << endl;
    }
  }


//for(int i = 0; i < 30; i++)
//{

//  cout << "Probably prime " << ": ";
//  if(check_primality(gen_large_num())) cout << "True" << endl;
//  else cout << "False" << endl;
//}
  return 0;

}


string get_Ptext()
{

  string m = "";
  ifstream inFile;
  inFile.open("ptext.txt"); //pubkey.txt prikey.txt
  inFile >> m;
  inFile.close();

  return m;
}

unsigned long gen_large_num()
{


  unsigned long result = 0;
  unsigned long exponent = 1;
  int rand_num_arr [32];

  for(int i = 0; i < 32; i++)
  {
    rand_num_arr[i] = rand() % 2;
//    cout << "assigning random nums: " << rand_num_arr[i] << endl;
  }

  rand_num_arr[0] = 1;
  for(int i = 0; i < 32; i++)
  {
    if(rand_num_arr[i] == 1)
    {
      result = result + exponent;
    }
    exponent = 2 * exponent;
  }

  return result;
}

//-------------------------------------------------------------------------------//
//https://www.geeksforgeeks.org/modular-exponentiation-power-in-modular-arithmetic/
/* Iterative Function to calculate (x^y)%p in O(log y) */
int power(int x, unsigned int y, int p)
{
    int res = 1;     // Initialize result

    x = x % p; // Update x if it is more than or
                // equal to p

    while (y > 0)
    {
        // If y is odd, multiply x with result
        if (y & 1)
            res = (res*x) % p;

        // y must be even now
        y = y>>1; // y = y/2
        x = (x*x) % p;
    }
    return res;
}
//-------------------------------------------------------------------------------//


bool check_primality(unsigned long num_in)
{
  unsigned long n = num_in;
  unsigned long s;
  unsigned long a, d = 0;
  unsigned long x = 0;

        if(n == 2 || n == 3 || x == n-1)
        {
          return true;
        }
        if(n < 2 || n % 2 == 0)
        {
          return false;
        }

        d = n - 1;
	s = 0;
        while(d % 2 == 0)
        {
          d = d/2;
          s++;
        }

        for(int i = 0; i < num_rounds; i++)
        {


	    a = 2 + rand() % (n-4);
//          a = rand() % (n-2) + 2;


//cout << "A= " << a;
//          x = pow(a, d);
//	    x = x % n;
	  x = power(a,d,n);

//cout << "a: " << a << " d: " << d << " a^d: " << pow(a, d) << " n: " << n << " x: " << x << endl;

	  if(x != n-1 && x != 1)
	  {
		for(int r = 0; r < (s-1); r++)
		{
			x = power(x,2,n);
			if( x == 1)
			{
			  return false;
			}
		}
                if(x != (n - 1))
                {
                  return false;
                }
            }
          }
	  return true;
}

