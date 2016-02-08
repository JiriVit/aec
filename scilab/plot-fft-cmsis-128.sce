funcprot(0);
exec('utils.sce');

fft_out_fn = "..\data\fft\01_cmsis_dsp_128\fft_out.txt";

t = linspace(0, 8, 128);
x = sin(2 * %pi * t);
X = loadTextData(fft_out_fn);

X2 = fft(x);

subplot(3, 1, 1);
plot(t, x);
xtitle('time domain');
subplot(3, 1, 2);
plot(abs(X));
xtitle('FFT done by Cortex-M4 CMSIS-DSP');
subplot(3, 1, 3);
plot(abs(X2(1:(length(t) / 2))));
xtitle('FFT done by Scilab');

