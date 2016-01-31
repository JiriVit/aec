M = 10;
h = zeros(1, M);
mic0 = [zeros(1, M - 1) mic]; // mic with zeros before

i = 0;
x = mic0((1+i) : (1+i+M-1));
d = x * flipdim(h', 1);
