function [x] = loadRawData(filename)
	fi = fileinfo(filename);
	fd = mopen(filename, 'rb');
	x = mget(fi(1) / 2, 's', fd);
	mclose(fd);
endfunction

function saveRawData(x, filename)
    fd = mopen(filename, 'wb');
    mput(x, 's', fd);
    mclose(fd);
endfunction

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

function plotSpectrum(x)
    Fs = 8000;
    t = 0 : 1/Fs : 1;
    n = length(t);
    f = linspace(0, Fs, n);

    X = fft(x)./(length(x)/2);
    
    plot(f(1 : n/2), abs(X(1 : n/2)));
endfunction

function [y] = doNlms(spk, mic)
    M = 80;
    len = length(spk);
    h = zeros(M, 1);
    spk0 = [zeros(1, M - 1) spk]; // spk signal preceded by zeros
    y = zeros(1, len);
    mikro = 0.7;
    
    for i = 1 : 10
        // calculate echo estimate
        x = spk0(i : (i + M - 1));
        y(i) = x * flipdim(h, 1);

        // update tap-weight
        e = mic(i) - y(i);
        stepsize = mikro * e * x / (x * x');
        h = h + stepsize';     
    end    
endfunction






