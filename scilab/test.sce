funcprot(0);

exec('utils.sce');

function spectrum_example()
    // Example from this website:
    // http://www.equalis.com/blogpost/731635/129429/Simple-Signal-Processing-Example-with-Scilab

    // Creating signals with sampling frequency of 1000 Hz
    Fs = 1000;
    t = 0:1/Fs:1;
    n = length(t);
    f = linspace(0,Fs,length(t)); // Create frequency vectors
    x1 = sin(2*%pi*10*t); // 10 Hz Sine Wave
    x2 = sin(2*%pi*100*t); // 100 Hz Sine Wave
    x = x1 + x2; // Combination of 10 Hz and 100 Hz Sine Wave
    subplot(2, 1, 1);
    plot(t, x); // Time Domain representation of the sine waves

    X = fft(x)./(length(x)/2); // Creating frequency response of the signal
    subplot(2, 1, 2);
    plot(f(1:n/2),abs(X(1:n/2))); // Frequency Domain representation.
endfunction

function ifft_example()
    t = linspace(0, 4, 64);
    x = sin(2 * %pi * t);
    X = fft(x);
    xx = ifft(X);

    subplot(3, 1, 1);
    plot(x);
    subplot(3, 1, 2);
    plot(X);
    subplot(3, 1, 3);
    plot(xx);

endfunction

ifft_example();
