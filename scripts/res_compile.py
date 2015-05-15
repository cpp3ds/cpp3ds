#!/usr/bin/env python
import os, re, sys, getopt

def compile(filenames, output_source, relative_dirs):
	# Make sure dirs have trailing slash /
	relative_dirs = [os.path.join(d, '') for d in relative_dirs]
	sourcefile = open(output_source, 'w')
	sourcefile.write("#include <cpp3ds/Resources.hpp>\nnamespace cpp3ds { namespace priv {\n")
	count = 1
	varlist = []
	for filename in filenames:
		varname = "res_%s_%d" % (re.sub('[^0-9a-zA-Z_]', '_', os.path.basename(filename)), count)
		for d in relative_dirs:
			if filename.find(d) == 0:
				resname = filename.replace(d,'').replace('\\','/')
		varlist += [[varname, resname]]
		with open(filename, 'rb') as f:
			sourcefile.write("const Uint8 %s_data[] = {\n" % varname)
			byte = f.read(1)
			while byte:
				sourcefile.write("%d," % ord(byte))
				byte = f.read(1)
			sourcefile.write("\n};\n")
			sourcefile.write("const Uint32 %s_size = %d;\n" % (varname, f.tell()))
		count += 1
	sourcefile.write("std::map<std::string, ResourceInfo> resources = {\n")
	for varname in varlist:
		sourcefile.write("{\"%s\",{%s_data,%s_size}}," % (varname[1], varname[0], varname[0]))
	sourcefile.write('};}}')

def show_usage_exit():
	print('test.py -i <inputfile> -o <outputfile>')
	sys.exit(2)

def main(argv):
	try:
		opts, args = getopt.getopt(argv, "ho:d:")
	except getopt.GetoptError:
		show_usage_exit()
	rel_dirs = []
	for opt, arg in opts:
		if opt == '-h':
			print('res_compile.py -o <cpp source output> -d <relative directory> file1, file2, ...')
			sys.exit()
		elif opt in ("-d", "--dir"):
			rel_dirs += [arg]
		elif opt in ("-o", "--output"):
			outfile = arg
	# print('Opts are: ', opts)
	# print('Args are: ', args)
	if not (rel_dirs and outfile):
		show_usage_exit()
	compile(args, outfile, rel_dirs)

if __name__ == "__main__":
	main(sys.argv[1:])
