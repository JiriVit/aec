// Show output of Visual C++ AEC implementation

funcprot(0);
exec('aec.sce');

// file names
mic_fn    = '../data/01_mic_clvl12.raw';
spk_fn    = '../data/01_spk_clvl12.raw';
out_fn    = '../data/out/01_out.raw';

// load data from raw files
mic = loadRawData(mic_fn);
spk = loadRawData(spk_fn);
out = loadRawData(out_fn);

// plot the data
t = linspace(0, 3000-1, 24000);

subplot(3, 1, 1);
plot(t, spk);
xtitle('speaker');
axes = gca();
axes.data_bounds(1, 2) = -30000;
axes.data_bounds(2, 2) = 30000;

subplot(3, 1, 2);
plot(t, mic);
xtitle('microphone');
axes = gca();
axes.data_bounds(1, 2) = -30000;
axes.data_bounds(2, 2) = 30000;

subplot(3, 1, 3);
plot(t, out);
xtitle('echo cancelled');
axes = gca();
axes.data_bounds(1, 2) = -30000;
axes.data_bounds(2, 2) = 30000;
