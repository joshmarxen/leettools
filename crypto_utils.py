#crypto utils

def egcd(a, b):
    x,y, u,v = 0,1, 1,0
    while a != 0:
        q, r = b//a, b%a
        m, n = x-u*q, y-v*q
        b,a, x,y, u,v = a,r, u,v, m,n
    gcd = b
    return gcd, x, y
    
def mod_inv(a, m):
    gcd, x, y = egcd(a, m)
    if gcd != 1:
        return None  # modular inverse does not exist
    else:
        return x % m

def mod_add(x, y, N):
    z = x+y
    if z > N:
        return z % N
    if z < 0:
        t = -z/N
        z+=t*N
        while z < 0:
            z+=N
    return z

def rotate_alphabet(s, o):
    return ''.join([chr((ord(x)-ord('A')+o)%26 + ord('A')) for x in s.upper()])
    
def vigenere_encrypt(s, k):
    s=s.upper()
    k=k.upper()
    ret_s = ""
    for i in range(len(s)):
        nc = ord(s[i])-ord('A')+ord(k[i%len(k)])
        if nc > ord('Z'):
            nc -= 26
        ret_s += chr(nc)
    return ret_s

def vigenere_decrypt(s, k):
    s = s.upper()
    k = k.upper()
    ret_s = ""
    for i in range(len(s)):
        nc = ord(s[i])-ord(k[i%len(k)]) + ord('A')
        if nc < ord('A'):
            nc += 26
        ret_s += chr(nc)
    return ret_s

def mod_exp(base, power, modulus):
	pbits = bin(power)[2:]
	running_val = 1
	for b in pbits:
		running_val = running_val*running_val
		running_val = running_val % modulus
		if b == '1':
			running_val = running_val * base
			running_val = running_val % modulus
	return running_val
