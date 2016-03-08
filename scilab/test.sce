funcprot(0);

exec('utils.sce');
exec('mdf.sce');

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
    subplot(3, 1, 1);
    plot(t, x); // Time Domain representation of the sine waves

    X = fft(x)./(length(x)/2); // Creating frequency response of the signal
    subplot(3, 1, 2);
    plot(f(1:n/2),abs(X(1:n/2))); // Frequency Domain representation.
    
    // 2N FFT
    xx = [zeros(1, length(x)), x];
    XX = fft(xx)./(length(xx)/2); // Creating frequency response of the signal
    subplot(3, 1, 3);
    plot(f(1:n),abs(XX(1:n))); // Frequency Domain representation.
endfunction

spectrum_example();

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

function mdf_example()
    // create testing input signal
    Fs = 1000;
    t = 0:1/Fs:(1-1/Fs);
    x1 = sin(2*%pi*10*t);
    x2 = sin(2*%pi*100*t);
    x = x1 + x2;

    // create random FIR filter divided to 3 blocks
    blockSize = 4;
    blockCount = length(x) / blockSize;

    fftHistory = zeros(blockSize, 3);

    // three filter blocks
    h1 = [ 1.2 -0.5  1.1  0.3];
    h2 = [ 1.4  0.9 -0.2  1.0];
    h3 = [-1.1  0.8  1.0  0.5];
    H = [h1' h2' h3'];

    // calculate response
    for blockIndex = 0 : (blockCount - 1)
        blockInput = x((1 + blockIndex * blockSize) : (blockIndex + 1) * blockSize);
        blockInputFFT = (fft(blockInput))';
        fftHistory = [blockInputFFT fftHistory(1:$, 1:($ - 1))];
        blockOutputHistory = fftHistory .* H;
        CS = cumsum(blockOutputHistory, 2);
        blockOutputFFT = CS(:, $);
        blockOutput = ifft(blockOutputFFT');
        y((1 + blockIndex * blockSize) : (blockIndex + 1) * blockSize) = blockOutput;
    end

    // plot the result
    subplot(2, 1, 1);
    plot(t(1:400), x(1:400));
    xtitle('input');
    subplot(2, 1, 2);
    plot(t(1:400), y(1:400));
    xtitle('output');
endfunction
