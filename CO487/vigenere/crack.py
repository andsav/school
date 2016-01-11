import fileinput
import operator
from config import frequencies


def rotate(cipher_letter, key_letter):
    alphabet = list('abcdefghijklmnopqrstuvwxyz')
    position = (alphabet.index(cipher_letter) + alphabet.index(key_letter)) % alphabet.__len__()
    return alphabet[position]


def nth_sub_key(start, key_length, string):
    letters_list = []
    for i in range(start - 1, string.__len__(), key_length):
        letters_list.append(string[i])
    return ''.join(letters_list)

# Read input and remove spaces/line breaks
cipher_text = ''
for line in fileinput.input():
    cipher_text = cipher_text + line.replace(" ", "").strip()

# 1) Find Key Length

## 1.1) Find repeated sequences of 3 to 5 letters

sequences = {}
repeat_spacings = []

for sequence_length in range(3, cipher_text.__len__()):
    for sequence_start in range(cipher_text.__len__() - sequence_length):
        sequence = cipher_text[sequence_start : (sequence_start + sequence_length)]
        if sequences.has_key(sequence):
            repeat_spacings.append(sequence_start - sequences[sequence])
        else:
            sequences[sequence] = sequence_start

## 1.2) Find factors for each spacing

factors = {}

for spacing in repeat_spacings:
    for i in range(2, cipher_text.__len__()):
        if spacing % i == 0:
            if factors.has_key(i):
                factors[i] += 1
            else:
                factors[i] = 1

## 1.3) Guess the key length (most common factor, it might be something else)

key_length = max(factors.iteritems(), key = operator.itemgetter(1))[0]
print "Guessing Key Length: ", key_length

# 2) Find Encrypted Text

## 2.1)

for letter in range(1, key_length):
    print nth_sub_key(letter, key_length, cipher_text)