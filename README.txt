Damien Jones
damien2@pdx.edu

This program is written in C++ on the PSU Linux system and uses the g++ compiler. 
- Enter 'g++ ' plus the file name to compile. 
- This will produce 'a.out'. a.out is an executable file 
- To run program, enter './a.out'

I was unable to get the key generation/f_table-lookup to work properly. The right values are being 
entered into the Ftable lookup, but the incorrect values are being output. I think 
that the rest of the program works properly, I just cant figure out the key problem. 

This program can generate a key and write it to a file named key.txt, or you can comment 
out the Generate_key() function and save your own key to the key.txt file. 

The program whitens correctly, performs XOR operations correctly, converts between 
char/ascii/hex/binary, and generates keys and increments them correctly.But unfortunately 
without the Ftable lookup working properly, it wont encrypt or decrypt   :(


Files that are included:

README.txt - Instructions
cyphertext.txt - File to write cyphertext to.
plaintext.txt - File that contains plaintext.
key.txt - File to hold the key valuess. All keys are generted at start.
f_table.txt - File with Ftable in hex
ui.txt - Contains the generate key code in .txt format
