# DictOS
OS implementation of a simple dictionary translator from English to the specified language. The implementation corresponds to additional tasks.

This repository contains materials on a practical task dedicated to studying the basics of OS development, the principles of low-level interaction with hardware, 
programming system functionality and the system boot process.

## Repository structure

+ task.md - text of the task;
+ command.txt - the necessary commands to generate binary files and run the OS;
+ start.asm - example of a boot sector;
+ kernel_start.cpp - an example of a minimal kernel;
+ bootsect.asm - the loader code in assembly language;
+ bootsect.bin - the loader;
+ kernel.cpp - kernel implementation code;
+ kernel.o - kernel object file;
+ kernel.bin - the kernel;

## Description of the implementation

**The loader** starts by initializing the addresses of memory segments.  An array of zeros is created at address 0x9000, which will be used to store information about allowed letters (from 'a' to 'z'): if the letter is entered by the user, the value of the array element becomes 1 at the appropriate position, and vice versa. Until the user presses Enter, he can enter letters to include/exclude them. If the user presses the Enter key, control is transferred to the begin label.  Each time the status changes, the current status of the active letters is displayed on the screen.

The begin label begins preparing for the transition to protected mode. The kernel boot address, information about the number of the disk, head, cylinder, sector, and the number of sectors to be loaded are recorded. Interrupts are disabled, information about the descriptor table is loaded, and the A20 address line is turned on. 

After switching to protected mode, control is transferred to the address 0x10000, where the OS kernel code will be located. 

The bootloader code ends with the installation of the last two bytes, the signature of the bootable media.

**To implement the task solution to the OS kernel, the following functions have been developed:**

+ Getting an argument from a command.
```
char* get_argument(char* command)
```

Handling cases of incorrect input: the absence of an argument, the presence of more than one argument. In case of an error, an empty string is returned, otherwise an argument is returned.

+ Search for the translation of a word in the section.
```
char* bin_search(char* word, DictSection* section)
```

Binary search for the translation of a word by section. If the word is not found, an empty string is returned, otherwise the word is translated.

+ Write/read bytes from ports.
```
static inline void outb(unsigned short port, unsigned char data)
static inline void outw(unsigned int port, unsigned int data)
static inline unsigned char inb(unsigned short port)
```

Using assembler inserts, the actions of writing to the byte/word port and reading from the byte port are performed.

+ Displaying a character/string on the screen.	
```
static inline void out_char(const char sym, unsigned int strnum)
static inline void out_str(const char* ptr, unsigned int strnum)
```

They support scrolling of the screen (video_up()), when displaying a line, specific characters of the Spanish alphabet are correctly processed.

+ Output information about the OS.	
```
static inline void info()
```

+ Displaying information about the loaded dictionary.	
```
void dictinfo()
```

The information about the number of words stored in the dictionary structure is used.

+ The translation of the word.
```
void translate()
```

An argument is extracted from the command, its correctness is checked (in case of an incorrect argument, the user will see an error message). Using binary search, a translation word or a message about the absence of a translation is displayed.

+ Output of information about the statistics of the dictionary section.	
```
void wordstat()
```

An argument is extracted from the command, its correctness is checked (in case of an incorrect argument, the user will see an error message). The information about the number of words and the status of the section stored in the structure of the dictionary section is used.

+ Turning off the computer.
```
static inline void shutdown()
```

It is implemented by writing a sequence of bytes to the I/O ports.

+ Output a random word from the uploaded/to a specific letter.
```
void anyword()
```

An argument is extracted from the command, its correctness is checked (in case of an incorrect argument, the user will see an error message).
If the section for the specified letter is not loaded, the user will see an error message. 
If the argument is correct, each of the cases is processed. When specifying a specific letter, a pseudo-random number is generated, and the remainder of the division by the number of words in the section is used to select a word from the section. Without specifying the letter, the section number is also generated in this way.

+ Processing the input of an incorrect/unknown command.
```
static inline void wrong_command()
```

+ Processing the command.	
```
void enter()
```

Performing the function according to the input. Clearing the input line after executing the command to write a new one.

+ Processing the code received by the keyboard.
```
void read_code(unsigned char code)
```

Changing the line is an input if the number of characters already entered does not exceed 40. Press BackSpace, Enter, Shift.

+ The entry point.
```
extern "C" int kmain()
```	

Loading a dictionary, counting its characteristics, greeting the user, initializing interrupt handlers, waiting for input from the user.

+ The output functions given in the methodological materials have been improved: scrolling of the screen has been implemented when it is fully filled, and the output of specific characters of the Spanish alphabet is also supported. To perform operations with strings and numbers, utility functions were implemented – analogs of functions for working with strings and numbers in C.

+ The dictionary is a data structure consisting of sections (one for each letter), which store pairs of words, the number of pairs of words, and flags for including the section. The dictionary also stores the total number of words and the number of available words, which are calculated at the beginning of the OS kernel. 
The search for words in the dictionary takes place by means of a binary search for the necessary section of the dictionary: the program accesses the first letter of the word to the necessary section, after which it performs a binary search on it.

+ To implement the generation of pseudorandom numbers, an algorithm from the Pintos OS was selected: the RC4 pseudorandom number generation algorithm.
