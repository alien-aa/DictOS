# Task
**Loader:** interacts with the user, allowing the user to specify the words for which letters of the English language he will be interested in after launching the OS. 
The loader prompts the user to allow the use of words with the specified letter or prohibit the use of them by pressing the corresponding key once.  

At the same time, the screen indicates which letters are marked and which are not:

abcde_____kl_nop____uv_x_z

When you press a key, for example 'w', the screen is updated:

abcde_____kl_nop____uvwx_z

To start the OS, the user presses Enter.

The dictionary can be stored in the data section of the kernel (you can declare it directly in the code as static data). The dictionary must be sorted. Dictionary storage is allowed in an unsorted form, but the sorting is performed at OS startup.
The dictionary must contain at least 50 common words. It is not necessary to specify articles in words.

_**Additional task:_ The dictionary must contain at least 500 commonly used word pairs. Binary search must be used to search the dictionary.

### Supported OS commands:
**info**

Displays information about the author and OS development tools (assembler translator, compiler), the parameters specified in the loader — a list of letters, words to which should be processed when requesting translation.

**dictinfo**

Displays information about the loaded dictionary (language, total number of words, number of words available for translation — calculated based on the data set in the loader).
Example:
```
# dictinfo
Dictionary: en -> es
Number of words: 1121
Number of loaded words: 780
```

**translate _word_**

Translates a word from English into another language. If the word is not found or has not been uploaded — displays an error. When searching for a word, a binary search is performed in an array of dictionary words.
_**Additional task:_ When translating, some specific letters of the language must be displayed correctly (for Spanish, the letter n; for Finnish, the letters a, ö). There must be words with such letters in the dictionary.
Example:
```
# translate cat
gato
# translate airport
aeropuerto
# translate moonlight
Error: the word "moonlight" is unknown
```

**wordstat _letter_**

Displays the number of loaded words in the dictionary by the specified letter.
Example:
```
# wordstat z
Letter 'z': 57 words loaded.
# wordstat y
Letter 'y': 1 word loaded.
# wordstat d
Letter 'd': 0 words loaded.
```

**shutdown**

Turning off the computer.

_**Additional task:_ **any word _letter_**

Outputs a random word to the specified letter and its translation. The letter may not be specified – then a random word is output for any letter. The algorithm for generating random or pseudorandom numbers is determined by the student and should be described in the report.
Examples:
```
# anyword n
night: noche
# anyword z
Error: no words
# anyword n
nice: bonito
# anyword
boy: muchacho
```

## Task options:
The work is performed on Linux OS.

Assembler translator for loader: YASM, syntax: Intel.

Kernel Compiler: gcc.

Task for implementation: DictOS (language: English -> Spanish).
