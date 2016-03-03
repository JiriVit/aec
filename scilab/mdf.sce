// MULTIDELAY BLOCK FREQUENCY DOMAIN ADAPTIVE FILTER

// Interesting links:
// https://en.wikipedia.org/wiki/Multidelay_block_frequency_domain_adaptive_filter
// http://mathworld.wolfram.com/FourierMatrix.html


function e = mdf(spk, mic)
    N = 4; // size of block
    K = 20; // number of blocks
    L = K * N; // adaptive filter length
    len = length(spk);
    blocksInInput = len / N;
    progressOld = 0;
    e = zeros(1, len);
    D = zeros(N, K); // one column = FFT for one block
    H = zeros(N, K); // one column = weights for one block
    A = zeros(N, K); // one column = output of one block

    cnt = 0;
    
    // iterate through blocks
    for i = 0 : (blocksInInput - 1)
        
        // show progress x/10
        progress = round(10 * i / (blocksInInput - 1));
        if (progress > progressOld) then
            printf("%i/10\n", progress);
            progressOld = progress;
        end
        
        // get i-th block from spk and mic
        x = spk((1 + i * N) : (i + 1) * N);
        y = mic((1 + i * N) : (i + 1) * N);
        // calculate FFT
        d = (fft(x))';
        // add FFT to matrix D
        D = [d D(1:$, 1:($ - 1))];      
        // calculate outputs for each block
        A = H.*D;
        // cumulate sum of A columns
        A_CS = cumsum(A, 2);
        // most left column contains sum of A across its columns
        e_fft = A_CS(:, $);
        // IFFT and save to output
        e((1 + i * N) : (i + 1) * N) = ifft(e_fft');
        
        // TODO update weights
    end    
endfunction

function mdf_example()
    // create testing input signal
    Fs = 1000;
    t = 0:1/Fs:1;
    x1 = sin(2*%pi*10*t);
    x2 = sin(2*%pi*100*t);
    x = x1 + x2;

    // real filter - this is what we are trying to find
    H = [0.9 0.1 1.2 -1.3 1.1];
    
    // filter the signal with real filter
    y = filter(H, 1, x);

    // now the job is: take x and y and find H
    
endfunction

mdf_example();
