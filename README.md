# Huffman Compression and Decompression

This project implements a simple Huffman coding algorithm to compress and decompress text files. The program reads an input text file, compresses it using Huffman coding, and then decompresses it back to verify the correctness of the compression.

## Features

1. **Create a Frequency Table:** Reads characters from the input file and creates a frequency table.
2. **Build Huffman Tree:** Uses the frequency table to build a Huffman tree.
3. **Generate Variable Length Codes (VLC):** Generates binary codes for each character based on the Huffman tree.
4. **Compress the File:** Uses the generated codes to compress the input file.
5. **Decompress the File:** Decompresses the compressed file back to its original form.

## Files

- `input.txt`: The input text file to be compressed.
- `VLCodes.cod`: Stores the variable length codes for each character.
- `input.com`: The compressed file.
- `decompressedFile.txt`: The file after decompression, should match the original input file.

## Detailed Explanation of Functions

### Data Structures

- `struct content`: Stores a character and its frequency.
- `struct tNode`: Represents a tree node in the Huffman tree.
- `struct Node`: Represents a linked list node containing a tree node and a pointer to the next node.

### Functions

- `Node* createNode(Node* h, char letter)`: Creates a new linked list node.
- `Node* createParent(Node* h, int freq, tNode* L, tNode* R)`: Creates a new parent node in the Huffman tree.
- `void sort(Node* head)`: Sorts the linked list nodes in ascending order based on frequency.
- `Node* generateFrequencyTable(Node* h, char c)`: Generates the frequency table by counting occurrences of each character.
- `Node* removeFirst(Node* h, tNode* item)`: Removes the first node from the linked list.
- `Node* createHuffmanTree(Node* h)`: Builds the Huffman tree from the frequency table.
- `void findHeight(tNode* h, int index, int* max)`: Finds the height of the Huffman tree.
- `void generateVLCs(FILE* output, tNode* h, int code[], int index)`: Generates variable length codes for each character.
- `void compress(FILE* input, FILE* cod, FILE* com)`: Compresses the input file using the generated codes.
- `void decompress(Node* h, FILE* decompressedFile)`: Decompresses the compressed file back to its original form.

### Main Function

The `main` function orchestrates the process of reading the input file, generating the frequency table, building the Huffman tree, generating the codes, compressing the file, and finally decompressing it to verify correctness.

## Notes

- The program checks for file size constraints and handles cases where the file is too small or too large to be compressed.
- Proper error handling is implemented to ensure that files are opened successfully.


## Conclusion

This implementation demonstrates the Huffman coding algorithm's effectiveness in compressing and decompressing text files. By following the steps and understanding the functions, you can grasp the fundamentals of file compression using Huffman coding.
 
