import os, socket, sys, struct, getopt

def sendfile(filename, ip):
	statinfo = os.stat(filename)
	fbiinfo = struct.pack('!q', statinfo.st_size)
	with open(filename, 'rb') as f:
		sock = socket.socket()
		sock.connect((ip, 5000))
		sock.send(fbiinfo)
		while True:
			chunk = f.read(16384)
			if not chunk:
				break  # EOF
			sock.sendall(chunk)
		sock.close()

def show_usage_exit():
	print('sendfile.py -f <inputfile> -i <ip address>')
	sys.exit(2)

def main(argv):
	filename = None
	ip = None
	try:
		opts, args = getopt.getopt(argv, "hf:i:")
	except getopt.GetoptError:
		show_usage_exit()
	for opt, arg in opts:
		if opt == '-h':
			show_usage_exit()
		elif opt in ("-f", "--file"):
			filename = arg
		elif opt in ("-i", "--ip"):
			ip = arg
	if not (filename and ip):
		show_usage_exit()
	sendfile(filename, ip)

if __name__ == "__main__":
	main(sys.argv[1:])
