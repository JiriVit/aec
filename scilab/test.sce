funcprot(0);

exec('utils.sce');

// create 64 samples with 4 sine periods
fft_in_fn = "..\sam4s\source\fft_in_64.txt";
t = linspace(0, 4, 64);
x = sin(2 * %pi * t);
saveTextData(x, fft_in_fn);
plot(t, x);

//fft_out_fn = "d:\Projekty\jcom\gdp06i\sam4s-sim\fft_out.txt";
//
//x = loadTextData(fft_out_fn);
//plot(x);
//

//
//
// Creating signals with sampling frequency of 1000 Hz
//Fs = 1000;
//t = 0:1/Fs:1;
//n = length(t);
//f = linspace(0,Fs,length(t)); // Create frequency vectors
//x1 = sin(2*%pi*10*t); // 10 Hz Sine Wave
//x2 = sin(2*%pi*100*t); // 100 Hz Sine Wave
//x = x1 + x2; // Combination of 10 Hz and 100 Hz Sine Wave
//plot(t,x); // Time Domain representation of the sine waves

//X = fft(x)./(length(x)/2); // Creating frequency response of the signal
//X = fft(x(1:64)); // Creating frequency response of the signal
//plot(f(1:n/2),abs(X(1:n/2))); // Frequency Domain representation.
//plot(f,X); // Frequency Domain representation.

