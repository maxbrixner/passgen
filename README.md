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
### Default alphabets
| No    | Alphabet                            |
| --------- | ----------------------------------------------------------------------- |
| 1      | abcdefghijklmnopqrstuvwxyz                                                 |
| 2      | ABCDEFGHIJKLMNOPQRSTUVWXYZ                                                 |
| 3      | ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz                       |
| 4      | ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789             |
| 5      | ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!$=?+#,;.:-_ |
## How it works
The Mersenne Twister is initialized using the process id and system time and creates 800,000 random numbers before starting to create passwords. The passwords themselves are created by picking a random letter from the alphabet using the random numbers created by the Mersenne Twister.