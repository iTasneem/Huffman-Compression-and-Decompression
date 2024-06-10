#include <stdio.h>
#include <stdlib.h>

struct content {
	char c;
	int freq;
};
struct tNode {
	content data;
	tNode* left;
	tNode* right;
	tNode()
	{
		left = right = NULL;
	}
};
struct Node {
	tNode data;
	Node* next;
	Node() {
		next = NULL;
	}
};

//Global variables used in compress and decompress functions
int paddedZeros, sizeArr;	//paddedZeros: Stores number of leading zeros of the leftmost byte
int* arr;	//arr: Array to store the binary equivalent of the input file


Node* createNode(Node* h, char letter)		//Creates a new linked list node
{
	if (h == NULL)	//Linked List is not yet created
	{
		h = (Node*)malloc(sizeof(Node));
		h->data.data.c = letter;
		h->data.data.freq = 1;
		h->data.left = h->data.right = NULL;
		h->next = NULL;
	}
	else // linked list already exists
	{
		Node* it = h;
		while (it->next != NULL)
			it = it->next;

		Node* tmp = (Node*)malloc(sizeof(Node));
		tmp->data.data.c = letter;
		tmp->data.data.freq = 1;
		tmp->data.left = tmp->data.right = NULL;
		tmp->next = NULL;

		it->next = tmp;
	}
	return h;
}

Node* createParent(Node* h, int freq, tNode* L, tNode* R)	//Function to create a new abstract parent tree node
{
	if (h == NULL)	//Linked List is not yet created
	{
		h = (Node*)malloc(sizeof(Node));
		h->data.data.c = '*';
		h->data.data.freq = freq;
		h->data.left = L;
		h->data.right = R;
		h->next = NULL;
	}
	else // linked list already exists
	{
		Node* it = h;
		while (it->next != NULL)
			it = it->next;

		Node* tmp = (Node*)malloc(sizeof(Node));
		tmp->data.data.c = '*';
		tmp->data.data.freq = freq;
		tmp->data.left = L;
		tmp->data.right = R;

		tmp->next = NULL;

		it->next = tmp;
	}
	return h;
}

void sort(Node* head)	//Sorting in ascending order
{
	Node* i, * j;
	tNode tempData;
	i = head;
	while (i != NULL)
	{
		j = i->next;
		while (j != NULL)
		{
			if (i->data.data.freq > j->data.data.freq)	//Logic of bubble sort
			{
				tempData = i->data;
				i->data = j->data;
				j->data = tempData;
			}
			j = j->next;
		}
		i = i->next;
	}
}

Node* generateFrequencyTable(Node* h, char c)		//Creating the frequency table
{
	int flag = 0;
	Node* tmp = h;	//Using tmp to traverse through the linked list without losing the head node
	while (tmp != NULL)	//Checking the existence of the first node
	{
		if (c == tmp->data.data.c)	//Comparing the character from the input file with characters in linked list
		{
			tmp->data.data.freq++;	//Incrementing the frequency of the already existing character
			flag = 1;	//Flag to prevent creating a new node with the same character
		}
		tmp = tmp->next;
	}
	if (flag == 0 && c != EOF)	//Create a new node if the character does not exist
		h = createNode(h, c);
	return h;
}

Node* removeFirst(Node* h, tNode* item)		//Removes the first node of the priority queue and returns it
{
	if (h == NULL)
		return NULL;

	if (h->next == NULL)
	{
		*item = h->data;
		h = NULL;
	}
	else
	{
		Node* tmp = h->next;
		*item = h->data;
		h = tmp;
	}
	return h;
}

Node* createHuffmanTree(Node* h)	//Creating the huffman tree (by assigning two tree nodes into a parent node)
{
	while (h->next != NULL)
	{
		int freq1, freq2;
		freq1 = freq2 = 0;
		tNode* x1 = (tNode*)malloc(sizeof(tNode)), * x2 = (tNode*)malloc(sizeof(tNode));
		h = removeFirst(h, x1);
		freq1 = x1->data.freq;
		h = removeFirst(h, x2);
		freq2 = x2->data.freq;
		int freq = freq1 + freq2;
		h = createParent(h, freq, x1, x2);
		sort(h);
	}
	return h;
}

void findHeight(tNode* h, int index, int* max)		//Function to find the maximum depth of the huffman tree 
{
	if (h->left != NULL)
		findHeight(h->left, index + 1, max);

	if (h->right != NULL)
		findHeight(h->right, index + 1, max);

	if (h->left == NULL && h->right == NULL)
	{
		if (index > *max)
			*max = index;
	}
}

void generateVLCs(FILE* output, tNode* h, int code[], int index)		//Function to generate the variable length codes of each character in input file
{
	if (h->left != NULL)
	{
		code[index] = 0;
		generateVLCs(output, h->left, code, index + 1);
	}

	if (h->right != NULL)
	{
		code[index] = 1;
		generateVLCs(output, h->right, code, index + 1);
	}

	if (h->left == NULL && h->right == NULL)
	{
		printf("\n%c: ", h->data.c);
		fprintf(output, "%c", h->data.c);
		for (int depth = 0; depth < index; depth++)
		{
			printf("%d", code[depth]);
			fprintf(output, "%d", code[depth]);
		}
	}
}

void compress(FILE* input, FILE* cod, FILE* com)		//Function to compress the input file
{
	char temp = fgetc(input);	//Gets the first character from the input file
	while (temp != EOF)
	{
		rewind(cod);		//Moves the file pointer to the beginning of the file (.cod file)
		char temp2 = fgetc(cod);	//Gets the first character of the (.cod file)
		char oneUse = temp2;	//Usage of oneUse: To store the previous character of temp2
		while (temp2 != temp || oneUse != '')	//Loop to find the character in (.cod file) to get its code
		{
			oneUse = temp2;
			temp2 = fgetc(cod);
		}
		temp2 = fgetc(cod);

		do				//Loop to print the binary equivalent of the input file
		{
			if (temp2 == '0')
				fprintf(com, "%d", 0);
			else
				fprintf(com, "%d", 1);
			temp2 = fgetc(cod);
		} while (temp2 != '');
		temp = fgetc(input);
	}

	int sizeCom;	//Stores the length of the binary code
	fseek(com, 0, SEEK_END);
	sizeCom = ftell(com);
	paddedZeros = sizeCom % 8;	//paddedZeros: Stores number of leading zeros of the leftmost byte

	if (paddedZeros != 0)
		paddedZeros = 8 - paddedZeros;

	fclose(com);

	com = fopen("input.com", "r");
	if (!com)	//Checking if the file opened successfully
	{
		printf("Cannot open file!");
		return;
	}

	sizeArr = sizeCom + paddedZeros;		//sizeArr: Stores total size after padding zeros
	arr = (int*)malloc(sizeof(int) * sizeArr);

	for (int i = 0; i < paddedZeros; i++)	//Loop to add padded zeros to the beginning of the array
		arr[i] = 0;

	for (int i = paddedZeros; i < sizeArr; i++)		//Loop to add the binary code to the array after padding zeros
	{
		char arrElement = fgetc(com);
		if (arrElement == '0')
			arr[i] = 0;
		else
			arr[i] = 1;
	}
	fclose(com);

	com = fopen("input.com", "w");
	int iterations = sizeArr / 8;
	for (int i = 0; i < iterations; i++)
	{
		int dec = 0;
		int res = 0;

		for (int k = 0 + 8 * i; k < 8 + 8 * i; k++)	//Converting every consecutive 8 bits into one variable
			res = res * 10 + arr[k];

		//Converting the variable into its decimal equivalent
		int rem, baseMultiplier = 1;
		while (res > 0)
		{
			rem = res % 10;
			dec = dec + rem * baseMultiplier;
			baseMultiplier *= 2;
			res /= 10;
		}
		fprintf(com, "%c", dec);	//Printing the ASCII equivalent character of each byte in the (.com file)
	}
	fclose(com);
}

void decompress(Node* h, FILE* decompressedFile)		//Function to decompress the input file
{
	tNode* it = &h->data;	//it: Stores the root of the huffman tree

	//Traversing the huffman tree according to the code found in the array (0 == left , 1 == right)
	for (int i = paddedZeros; i < sizeArr; i++)		//Loop to reach the leaf node of the huffman tree
	{
		if (arr[i] == 0)
			it = it->left;
		else
			it = it->right;

		if (it->left == NULL && it->right == NULL)	//Leaf node found!
		{
			fprintf(decompressedFile, "%c", it->data.c);
			it = &h->data;	//Moving the pointer to the root of the tree
		}
	}
}

int main()
{
	FILE* inputFile;		//File to be compressed read by the user
	inputFile = fopen("input.txt", "r");

	if (!inputFile)	//Checking if the file opened successfully
	{
		printf("Cannot open file!");
		return -1;
	}

	int size;
	fseek(inputFile, 0, SEEK_END);	//Moving the file pointer to the end of the file
	size = ftell(inputFile);		//Returning the number of characters before the file pointer (EOF)
	rewind(inputFile);		//Returns the file pointer to the beginning of the file
	if (size < 2147483648 && size > 1)	//Maximum file length: 2.00GB
	{
		Node* head = NULL;
		char c,p;
		c = fgetc(inputFile);	//Gets a character from the input file (First character)
		head = createNode(head, c);

		p = c;
		int flag = 1;
		while (c != EOF)		//Reading all characters of the input file till its end
		{
			c = fgetc(inputFile);	//Gets character by character from the input file
			generateFrequencyTable(head, c);
			if (p == c)
				flag++;
		}

		if (flag == size)
		{
			printf("CANNOT COMPRESS A SINGLE REPEATED CHARACTER!");
			return -2;
		}
			

		sort(head);

		head = createHuffmanTree(head);

		FILE* outputCOD;		//File to store variable length code of each character in input file
		outputCOD = fopen("VLCodes.cod", "w");
		int max = 0;
		findHeight(&head->data, 0, &max);

		int* codeArr = (int*)malloc(sizeof(int) * max);	//Array to store the VLC of the character temporarily
		printf("Generated Codes:");
		generateVLCs(outputCOD, &head->data, codeArr, 0);
		fprintf(outputCOD, "");		//Adding the non-printable character to the end of the .cod file (Stopping Condition)
		free(codeArr);
		fclose(outputCOD);

		outputCOD = fopen("VLCodes.cod", "r");
		if (!outputCOD)	//Checking if the file opened successfully
		{
			printf("Cannot open file!");
			return -1;
		}

		FILE* com;
		com = fopen("input.com", "w");
		rewind(inputFile);
		compress(inputFile, outputCOD, com);

		FILE* decompressedFile;
		decompressedFile = fopen("decompressedFile.txt", "w");

		decompress(head, decompressedFile);

		free(arr);
		fclose(decompressedFile);
		fclose(outputCOD);
		fclose(inputFile);
		fclose(com);
	}

	else if (size == 1 || size == 0)
		printf("FILE CANNOT BE COMPRESSED! (MINIMUM SIZE)");
	else
		printf("FILE SIZE IS GREATER THAN 2.00GB");

	return 0;
}