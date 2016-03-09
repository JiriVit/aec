// MULTIDELAY BLOCK FREQUENCY DOMAIN ADAPTIVE FILTER

// Interesting links:
// https://en.wikipedia.org/wiki/Multidelay_block_frequency_domain_adaptive_filter
// http://mathworld.wolfram.com/FourierMatrix.html
// http://www.mathworks.com/help/signal/ref/dftmtx.html

funcprot(0);

// create Fourier transform matrix F(n, n)
function F = dftmtx(n)
    i = complex(0, 1); // imaginary unit
    
    F = ones(n, n);
    
    for j = 1:n
        for k = 1:n
            F(j, k) = exp(2*%pi*i*(j-1)*(k-1)/n);
        end
    end
endfunction

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



