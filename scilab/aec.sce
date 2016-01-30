function x = loadRawData(filename)
	fi = fileinfo(filename);
	fd = mopen(filename);
	x = mget(fi(1) / 2, 's', fd);
	mclose(fd);
endfunction

