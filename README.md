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

Получение аргумента из команды.	
```
char* get_argument(char* command)
```

Обработка случаев некорректного ввода: отсутствия аргумента, присутствие более чем одного аргумента. В случае ошибки возвращается пустая строка, иначе – аргумент (строка).

Поиск перевода слова в разделе.	
```
char* bin_search(char* word, DictSection* section)
```

Бинарный поиск перевода слова по разделу. В случае если слово не найдено – возвращается пустая строка, иначе – перевод слова.

Запись/чтение байта из портов.	
```
static inline void outb(unsigned short port, unsigned char data)
static inline void outw(unsigned int port, unsigned int data)
static inline unsigned char inb(unsigned short port)
```

При помощи ассемблерных вставок выполняются действия записи в порт байта/слова, чтение из порта байта.

+ Вывод символа/строки на экран.	
```
static inline void out_char(const char sym, unsigned int strnum)
static inline void out_str(const char* ptr, unsigned int strnum)
```

Поддерживают прокрутку экрана (video_up()), при выводе строки корректно обрабатываются специфические символы алфавита испанского языка.

+ Вывод информации об ОС.	
```
static inline void info()
```

+ Вывод информации о загруженном словаре.	
```
void dictinfo()
```

Используется информация о количестве слов, хранящаяся в структуре словаря.

+ Перевод слова.	
```
void translate()
```

Из команды извлекается аргумент, проверяется его корректность (в случае некорректного аргумента пользователь увидит сообщение об ошибке). При помощи бинарного поиска выводится слово-перевод или сообщение об отсутствии перевода.

+ Вывод информации о статистике раздела словаря.	
```
void wordstat()
```

Из команды извлекается аргумент, проверяется его корректность (в случае некорректного аргумента пользователь увидит сообщение об ошибке). Используется информация о количестве слов и статусе раздела, хранящаяся в структуре раздела словаря.

+ Выключение компьютера.	
```
static inline void shutdown()
```

Реализуется с помощью записи последовательности байтов в порты ввода-вывода.

+ Вывод случайного слова из загруженных/на конкретную букву.	
```
void anyword()
```

Из команды извлекается аргумент, проверяется его корректность (в случае некорректного аргумента пользователь увидит сообщение об ошибке).
Если раздел на указанную букву не загружен, пользователь увидит сообщение об ошибке. 
Если аргумент корректен обрабатывается каждый из случаев. При указании конкретной буквы, генерируется псевдослучайное число, для выбора слова из раздела используется его остаток от деления на число слов в разделе. Без указания буквы подобным образом генерируется также номер раздела.

+ Обработка ввода некорректной/неизвестной команды.	
```
static inline void wrong_command()
```

+ Обработка команды.	
```
void enter()
```

Выполнение функции в соответствие с вводом. Очистка строки-ввода после выполнения команды для записи новой.

+ Обработка кода, поступившего в клавиатуры.	
```
void read_code(unsigned char code)
```

Изменение строки-ввода, если количество уже введённых символов не превышает 40. Обработка клавиш BackSpace, Enter, Shitf.

+ Точка входа.
```
extern "C" int kmain()
```	

Загрузка словаря, подсчёт его характеристик, приветствие пользователя, инициализация обработчиков прерываний, ожидание ввода от пользователя.

+ Функции вывода, приведённые в методических материалах, были усовершенствованы: реализована прокрутка экрана при его полном заполнении, также поддерживается вывод специфических символов алфавита испанского языка. Для выполнения операций со строками и числами были реализованы служебные функции – аналоги функций для работы со строками и числами в C.

+ Словарь представляет собой структуру данных, состоящую из разделов (по одному на каждую букву), в которых хранятся пары слов, количество пар слов, флаги включения раздела. Также в словаре хранится общее количество слов и количество доступных слов, которые вычисляются в начале работы ядра ОС. 
Поиск слов в словаре происходит посредством бинарного поиска по необходимому разделу словаря: программа обращается по первой букве слова в необходимый раздел, после чего производит бинарный поиск по нему.

+ Для реализации генерации псевдослучайных чисел был выбран алгоритм из ОС Pintos: алгоритм генерации псевдослучайных чисел RC4.
