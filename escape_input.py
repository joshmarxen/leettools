import sys
from sys import argv, exit
from subprocess import PIPE, Popen

def format_chars(s):
	
	sindex = 0
	ret_str = ""
	while sindex < len(s):
		cur_char = s[sindex]
		if cur_char != '\\':
			ret_str+=cur_char
		else:
			sindex+=1
			next_char = s[sindex]
			if next_char == 'n':
				ret_str+="\n"
			elif next_char == '\\':
				ret_str+="\\"
			elif next_char == 'x':
				sindex+=1
				hex1 = s[sindex]
				sindex+=1
				hex1+=s[sindex]
				hv = int(hex1, 16)
				ret_str+=chr(hv)
		sindex+=1

	return ret_str

# check that they have supplied a program to pipe to
if len(argv) < 2:
	print "Usage: python escape_input.py <executable> <arg1> <arg2> ..."
	exit(0)
else:
	print str(argv[1:])

p = Popen(argv[1:],stdin = PIPE)

while p.poll() is None:
	try:
		if p.poll() is None:
			nextline = format_chars(sys.stdin.readline())
			p.stdin.write(nextline)
		else:
			break
	except:
		print "CONNECTION WITH PROGRAM TERMINATED"
		break

p.kill()
