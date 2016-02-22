// 2nd order high-pass Butterworth filter with cut-off freq 100 Hz
function [y] = applyHighPassFilter(x)
    B = [0.9460 -1.8920 0.9460];
    A = [1 -1.8890 0.8949];
    
    y = filter(B, A, x);
endfunction

// 2nd order low-pass Butterworth filter with cut-off freq 3400 Hz
function [y] = applyLowPassFilter(x)
    B = [0.7157 1.4315 0.7157];
    A = [1 1.3490 0.5140];
    
    y = filter(B, A, x);
endfunction

N_C = 240;
L_C = 50;

function ro_OL = doubleTalkDetection(x, y)
    xlen = length(x);
    ylen = length(y);
    z = zeros(1, L_C + 1);
    
    for l = 0 : L_C
        xx = x((xlen - N_C + 1 - l) : (xlen - l));
        yy = y((ylen - N_C + 1) : $);
        z1 = abs(xx * yy');
        z2 = abs(xx) * abs(yy');
        if (z2 ~= 0)
            z(l + 1) = z1 / z2;
        end          
    end
    
    ro_OL = max(z);
endfunction

function [d_d, e] = doNlms(spk, mic)
    M = 5;
    len = length(spk);
    h = zeros(M, 1);
    spk0 = [zeros(1, M - 1) spk]; // spk signal preceded by zeros
    d_d = zeros(1, len);
    e = zeros(1, len);
    mikro = 0.7;
    
    for i = 1 : len
        // calculate echo estimate
        x = spk0(i : (i + M - 1));
        d_d(i) = x * flipdim(h, 1);

        // update tap-weight
        e(i) = mic(i) - d_d(i);
        stepsize = mikro * e(i) * x / (x * x');
        if (i < 20)
            h = h + stepsize';
        end
    end    
endfunction


// MDF ---------------------------------------------------------------

function e = doMdf(spk, mic)
    N = 4; // size of block
    K = 20; // number of blocks
    L = K * N; // adaptive filter length
    len = length(spk);
    D = zeros(N, K); // FFT for each block
    H = zeros(N, K); // weights for each block
    A = zeros(N, K); // output of each block
    
    // iterate through blocks
    for i = 0 : ((len / N) - 1)
        // get i-th block from spk and mic
        x = spk((1 + i * N) : (i + 1) * N);
        y = mic((1 + i * N) : (i + 1) * N);

        // calculate FFT
        d = (fft(x))';
        // add FFT to matrix D
        D = [d D(1:$, 1:($ - 1))];
        
        // calculate outputs for each block
        a = D * h;

        // calculate sum of all block outputs
        
        
    end
    
    e = 0;
endfunction




