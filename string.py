import random
import string

def generate_random_string(length):
    return ''.join(random.choices(string.ascii_lowercase, k=length))

string1 = generate_random_string(1048)
string2 = generate_random_string(1048)

with open('inputdp.txt', 'w') as file:
    file.write(string1 + '\n')
    file.write(string2 + '\n')