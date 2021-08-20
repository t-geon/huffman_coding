# huffman_coding

This is the code I wrote through visual studio 2019 on windows.

-Explanation
This is a code that reads a string from a txt file and compresses it using Huffman coding.
After checking the probability of each character, it is compressed in a way that maximizes the compression ratio of the one with high probability.


- Encoding process
  1. Reading a txt file and counting the number of occurrences of each character
  2. Put the two least probable values of the characters left and right
  3. Repeat operation 2 until there is 1 remaining in the multimap -> If 1 is left, the node is root
  4. Put 0 on the left and 1 on the right and find the codeword
  5. table encoding (repeat until eof)
  6. Code encoding using table while reading txt file (repeat until eof is displayed)
    * When encoding, the byte unit is changed to the bit unit.

  
- decoding process
    * When decoding, it reads in byte unit, so it accesses bit by bit and changes each bit in byte unit.
  1. Decoding the table.hbs file
  2. Decode the code.hbs file
    2-1. Start with 1 letter, add 1 letter if no matching codeword in table
    2-2. If it is a codeword in the table, convert it to a character and start from the next position one by one
    2-3. If it is a codeword corresponding to eof, exit
  3. Output the resulting txt file


- result
  Compress the input txt file into a .hbs file using an encoder.
  Decompress the .hbs file into a txt file using a decoder.


- verification
  Check the compression rate by comparing the size of the input txt file and the compressed hbs file.
  Compare the input txt file with the decoded txt file and check if they are the same.
