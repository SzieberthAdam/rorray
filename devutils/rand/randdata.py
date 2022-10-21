import os
desired_size = 1024*1024*100 # 100 MB
desired_number_of_files = 1
for file_number in range(desired_number_of_files):
    filename = 'output_file{0:04d}.dat'.format(file_number)
    print('filename: {0}'.format(filename))
    with open(filename, 'wb') as fout: fout.write(os.urandom(desired_size))
print('Done.')
