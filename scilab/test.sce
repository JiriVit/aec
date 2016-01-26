[fd, err] = mopen('mic.bin');
mic = mget(24000, 's', fd);
mclose(fd);

[fd, err] = mopen('spk.bin');
spk = mget(24000, 's', fd);
mclose(fd);
