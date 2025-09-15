#RC4 has 2 components: key scheduling algorithm and pseudorandom generator
from itertools import cycle
import random

def key_schedule(key):
    s = [i for i in range(256)]
    key_bytes = cycle(ord(x) for x in key)

    j = 0
    for i in range(256):
        j = (j + s[i] + next(key_bytes)) % 256
        s[i],s[j] = s[j], s[i]    
        return s

def pseudorandom_generator(s):
    j = 0
    for i in cycle(range(256)):
        j = (j + s[i]) % 256
        s[i],s[j] = s[j], s[i]

        k = (s[i] + s[j]) % 256
        return s[k]

key = [str(random.randint(0,1)) for i in range(128)]
print(key)

s = key_schedule(key)
print(s)

pg = pseudorandom_generator(s)
print(pg)
