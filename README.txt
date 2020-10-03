Damien Jones
damien2@pdx.edu

This program is written in C++ on the PSU Linux system and uses the g++ compiler. Enter 'g++ ' plus the file name to compile. This will produce 'a.out'. a.out is an executable file that can be ran with './'.

I was unable to get the key generation to work properly. The right values are being entered into the Ftable lookup, but the incorrect values are being output. I think that the rest of the program works properly, I just cant figure out the key problem. 

You mention that youd give me a 75% without the key part working and that sounds fair. Ive beat my head against a wall for too long, lol  ;0) 

This program can generate a key and write it to the key file, or you can not call the generate key function and use your own key file. 

The program whitens correctly, performs XOR operations correctly, converts between char/ascii/hex/binary, and generates keys and increments them correctly.But unfortunately without the Ftable lookup working properly, it wont encrypt or decrypt   :(

I definitely think this program was super close to working, im sure its just a logic error somewhere.

Files that are included:

README.txt - Instructions
cyphertext.txt - File to write cyphertext to.
plaintext.txt - File that contains plaintext.
key.txt - File to hold the key valuess. All keys are generted at start.
f_table.txt - File with Ftable in hex
