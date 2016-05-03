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

function test_dftmtx()    
    // 128 samples, containing 4 sine periods
    t = 0:127;
    x = sin(2*%pi*t/32);
    
    X_FFT = fft(x);  // use scilab function
    F = dftmtx(128);
    X_F = x * F;     // use Fourier matrix
    
    subplot(2, 1, 1);
    plot(abs(X_FFT));
    xtitle('scilab fft()');
    subplot(2, 1, 2);
    plot(abs(X_F));
    xtitle('Fourier transform matrix');
endfunction

// creates block diagonal matrix from k square matrices
function [y] = mdiag(m, k)
    s = size(m);
    s = s(1);
    
    y = zeros(s*k, s*k);
    for i=0:(k-1)
        y((s*i+1):(s*(i+1)), (s*i+1):(s*(i+1))) = m;
    end   
endfunction

function test_mdiag()
    m = ones(2, 2);
    y = mdiag(m, 3);
    disp(y);        
endfunction

// Calculates response of a system in frequency domain.
// Input variables:
//     x ... input signal (row vector)
//     h ... filter coefficients in freq domain (column vector 2Nx1)
//     N ... filter block size (scalar value)
function y = mdf_response(x, h, N)
    L = length(h) / 2; // length of filter
    K = L / N; // number of blocks

    y = zeros(1, length(x)); // prepare output vector
    // Fourier transform matrices
    F = dftmtx(2*N);
    Finv = inv(F);
    // normalization matrix 2Nx2N
    G1 = abs(F * [zeros(N, 2*N); zeros(N, N) eye(N, N)] * Finv);
    // how many blocks in input signal
    B = length(x) / N;

    // X(l) composed of diag(fft(xf_k))
    X = zeros(2*N, 2*N*K);

    // input with zero-padding
    x0 = [zeros(1, (K-1)*N) x zeros(1, N)];

    for l = 1:B
        // calculate X(l)
        for k = 0:(K-1)
            // subset of x which is to be FFT'd
            xf_k = x0(((l+K-2-k)*N+1):((l+K-k)*N));
            // x_k as defined in Wiki
            x_k = abs(diag(F*(flipdim(xf_k, 2))'));
            // place x_k to X(l)
            X(1:$, (k*2*N+1):((k+1)*2*N)) = x_k;
        end

        // output of filter, needs to IFFT
        ye_l = G1 * X * h; // 2Nx1
        // IFFT        
        y_l = real(Finv * ye_l); // 2Nx1
        // store last N values to output vector
        y(((l-1)*N+1):(l*N)) = (y_l((N+1):(2*N)))';        
    end
endfunction

function test_mdf_response()
    Fs = 1000;
    t = 0:1/Fs:(1-1/Fs);
    x1 = sin(2*%pi*10*t);
    x2 = sin(2*%pi*100*t);
    x = x1 + x2;
    
    h = 0.9 * ones(24, 1);
    y = mdf_response(x, h, 4);
    
    subplot(2, 1, 1);
    plot(t, x);
    subplot(2, 1, 2);
    plot(t, y);
endfunction

// MDF algorithm, as described on Wikipedia
// Input variables:
//     x ... input to unknown system + adaptive filter
//     y ... output from unknown system + interference
//     N ... length of one adaptive filter block
//     K ... number of adaptive filter blocks
// Output variables:
//     e ... error output, estimate of interference
//     y ... adaptive filter output
//     H ... history of adaptive filter weights
function [e, y, H] = mdf(x, y, N, K)
    // filter parameters
    L = N*K; // length of filter
    h = zeros(2*N*K, 1); // filter weights

    mu = 0.7;

    // Fourier transform matrices
    F = dftmtx(2*N);
    Finv = inv(F);
    // normalization matrices
    G1 = F * [zeros(N, 2*N); zeros(N, N) eye(N, N)] * Finv;
    G2t = F * [eye(N, N) zeros(N, N); zeros(N, 2*N)] * Finv;
    G2 = mdiag(G2t, K);

    B = length(x) / N;  // how many blocks in input signal

    H = zeros(B, 2*N*K); // history of h vector

    // X(l) composed of diag(fft(xf_k))
    X = zeros(2*N, 2*N*K);

    // input with zero-padding
    x0 = [zeros(1, (K-1)*N) x zeros(1, N)];

    for l = 1:B
        // calculate X(l)
        for k = 0:(K-1)
            // subset of x which is to be FFT'd
            xf_k = x0(((l+K-2-k)*N+1):((l+K-k)*N));
            // x_k as defined in Wiki
            x_k = diag(F*(flipdim(xf_k, 2))');
            // place x_k to X(l)
            X(1:$, (k*2*N+1):((k+1)*2*N)) = x_k;
        end

        // real y(l)
        y_l = y(((l-1)*N+1):(l*N));  // 1xN
        // 2N-FFT'd y(l)
        df_l = F * [zeros(1, N) y_l]'; // 2Nx1
        // estimate y(l), output of adaptive filter        
        ye_l = G1 * X * h; // 2Nx1
        // FFT'd error signal
        ef_l = df_l - ye_l;
        // normalisation matrix
        Phi = X' * X;
        H(l, 1:$) = h';
        // update filter weights
        h = h + mu * G2 * Phi * X' * ef_l;        

        // IFFT of error signal
        e_l = real(Finv * ef_l);
        y_l = real(Finv * ye_l);
        // store last N values to outputs vector
        e(((l-1)*N+1):(l*N)) = (e_l((N+1):(2*N)))';                
        y(((l-1)*N+1):(l*N)) = (y_l((N+1):(2*N)))';                
    end
endfunction



// SCRIPT SECTION ----------------------------------------------------

// input signal
t = 0:1023;
x1 = sin(2*%pi*t/10);
x2 = sin(2*%pi*t/100);
x = x1 + x2;

// MDF parameters
N = 8; // length of one block
K = 3; // number of blocks

// real system
F = dftmtx(2*N);
h = 0.9 * ones(N, 1);  // one block in time domain
h0 = [h; zeros(N, 1)]; // padded with zeros
hfk = abs(fft(h0));    // one block in frequency domain
hf = [hfk; hfk; hfk];  // all blocks in frequency domain

printf("calculating response...\n");
y = mdf_response(x, hf, N);

printf("calculating MDF...\n");
[e, ye] = mdf(x, y, N, K);
   
//deletefile('mdf_e.txt');
//write('mdf_e.txt', e, '(1(f7.2,3x))');   
//deletefile('mdf_y.txt');
//write('mdf_y.txt', y, '(1(f7.2,3x))');   
//deletefile('mdf_h.txt');
//write('mdf_h.txt', H, '(24(f7.2,3x))');   

subplot(211);
plot(t, x);
subplot(212);
plot(t, y);
