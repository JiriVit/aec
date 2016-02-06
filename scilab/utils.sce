// Global settings
Fs = 8000;
SampleLen = 24000;

// Plot more files one below another, so they can be compared in time
// domain.
// Takes filenames as input argument.
function plotOneFileBelowAnother(varargin)
    [lhs, rhs] = argn(0);

    t = linspace(0, (SampleLen / Fs) - 1, SampleLen);
    
    for i = 1:rhs
        filepath = varargin(i);
        x = loadRawData(filepath);
        subplot(rhs, 1, i);
        plot(t, x);
        xtitle(fileparts(filepath, 'fname'))
        axes = gca();
        axes.data_bounds(1, 2) = -30000;
        axes.data_bounds(2, 2) = 30000;
    end
endfunction

function plotSpectrum(x)
    t = 0 : 1/Fs : 1;
    n = length(t);
    f = linspace(0, Fs, n);

    X = fft(x)./(length(x)/2);
    
    plot(f(1 : n/2), abs(X(1 : n/2)));
endfunction

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

function play(x)
    sound(x ./ 32768, Fs);
endfunction
