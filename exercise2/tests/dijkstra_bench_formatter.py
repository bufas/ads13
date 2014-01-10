
#
# Step 1
#

elements = 0
density  = 0

running_sum = 0

files = [['fib_dijkstra_bench.txt', 'fib_dijkstra_bench_processed.txt'], \
		 ['bin_dijkstra_bench.txt', 'bin_dijkstra_bench_processed.txt'], \
		 ['veb_dijkstra_bench.txt', 'veb_dijkstra_bench_processed.txt']]

for filename in files:
	with open(filename[0]) as in_file:
		with open(filename[1], 'w') as out_file:
			counter = 0;
			for line in in_file:
				if counter % 11 == 0:
					words = line.split();
					elements = int(words[0])
					density  = int(words[1])
					running_sum = 0;
				else:
					running_sum += float(line[:-1])
				
				counter += 1

				if counter % 11 == 0:
					out_file.write('%d\t%d\t%f\n' % (elements, density, running_sum/10))

#
# Step 2
#

output = ['','','','','','']
file_contents = []

# Read in all the files
for filename in files:
	with open(filename[1]) as in_file:
		file_contents.append(in_file.readlines());

# Merge the contents
for i in range(1, len(file_contents[0])):
	output[i % 6] += '%s%s%s' % (file_contents[0][i], file_contents[1][i], file_contents[2][i])

# Write the output to files
output_filenames = ['dens0.data', 'dens10.data', 'dens25.data', 'dens50.data', 'dens75.data', 'dens100.data']
i = 0
for filename in output_filenames:
	with open(filename, 'w') as out_file:
		out_file.write(output[i])
	i += 1
