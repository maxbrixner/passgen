# passgen
A simple and lightweight password generator for Linux.
## Disclaimer
This password generator is using a Mersenne Twister for random number generation. This might not be secure enough for cryptological use.
## Usage
Use the generator from your command line like this.
```
$ passgen [<options>]
```
If no options are given, 30 passwords with 25 characters each are created from the default alphabet 5.
### Options
| Option    | Description                            |
| --------- | ------------------------------------------------------------------------------------------------------------ |
| `-a`      | Sets the alphabet. Can be any string of characters or a number between 1-6 for one of the default alphabets. |
| `-l`      | Set the lengths of the created passwords.                                                                    |
| `-c`      | Set the number of passwords that are created.                                                                |
| `-s`      | Mix in a custom seed (unsigned long number).                                                              |
### Default alphabets
| No    | Alphabet                            |
| --------- | ----------------------------------------------------------------------- |
| 1      | abcdefghijklmnopqrstuvwxyz                                                 |
| 2      | ABCDEFGHIJKLMNOPQRSTUVWXYZ                                                 |
| 3      | ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz                       |
| 4      | ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789             |
| 5      | ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!$=?+#,;.:-_ |
### Example
```
$ passgen -l=25 -c=9 -s=823876319 -a="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"
alphabet length:           62
password length:           25
possible combinations:     6.453454e+44
password entropy:          148.85
time needed to guess:      1.023188e+25 millenia

mXia5kZAMexNLaNQAlWzZ1T6g     RcXuTnsLwRiQJouG6UjoFny5Q     4pyGuuSEQPR7n3vSfaCTiXj6Y
xGgBzGg91cT4i9WIPGSOzCSE0     2KWo7gwMs8hwlYG4pw1hMSjWI     z6vhzE9njQeOugmtuuquDPcyD
RrVXsbe6FLp4aNOWjYR8HL0Xv     2yj9ITbNMELStD01Yki0EyZsi     UeoGKlWaQvoa6vFrSWbQLTZZt
```
## How it works
The Mersenne Twister is initialized using the process id and system time and creates 800,000 random numbers before starting to create passwords. The passwords themselves are created by picking a random letter from the alphabet using the random numbers created by the Mersenne Twister.