// Global settings
Fs = 8000;
SampleLen = 24000;

// Plot more files one below another, so they can be compared in time
// domain.
// Takes filenames as input argument.
function plotOneFileBelowAnother(varargin)
    [lhs, rhs] = argn(0);

    t = linspace(0, SampleLen / Fs, SampleLen);

    f = gcf();
    
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

    // 1st screen
    f.figure_position = [0 0];
    f.figure_size = [1366 768];    

//    // 2nd screen
//    f.figure_position = [1366 0];
//    f.figure_size = [1280 1024];    
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

function [x] = loadTextData(filename)
    x = 0;
    fd = mopen(filename, 'rt');
    while ~meof(fd)
        [n, a] = mfscanf(fd, "%f");
        x = [x a];
    end
    mclose(fd);    
endfunction

function saveTextData(x, filename)
    fd = mopen(filename, 'wt');
    for i = 1:length(x)
        mfprintf(fd, "%f,\n", x(i));
    end
    mclose(fd);    
endfunction

function play(x)
    sound(x ./ 32768, Fs);
endfunction

function playnorm(x)
    sound(x ./ max(x), Fs);
endfunction
