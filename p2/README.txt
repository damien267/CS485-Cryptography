Damien Jones
damien2@pdx.edu

This program is written in C++ on the PSU Linux system and uses the g++ compiler. Enter 'g++ ' plus the file name to compile. This will produce 'a.out'. a.out is an executable file that can be ran with './'.

I was unable to get this program working. I spent a little over a week on the Miller-Rabin test. I can find prime numbers from 1-103, and then it says that everything else is a composite. Ive tried rewriting the algorithm a few times, and nothing worked. 

For the random 32 bit number generator, I created it bit by bit, using rand() %2 to either use 1 or 2, then coverted the 32 bit array into an integer. This created large radom numbers but I was unable to tell if they were prime. 

The last thing I was trying to finish before it was due is the key generation. I started with the primality test which ended up being the most time consuming. Next time I will produce the "easy" stuff first, and save the hard part for last. I just figured that I would need to know if the numbers are primes before I could continue.

I really enjoyed working on these projects, I just wish I could have completed them. 

The files that are included are:

README.txt - Instructions
p2.cpp - Main implementation file
prikey.txt - File to hold private key
pubkey.txt - File to hold public key
ptext.txt - Text file to be encrypted
