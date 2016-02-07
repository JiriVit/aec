// Show output of Visual C++ AEC implementation

funcprot(0); // avoid warning when redefining a function
exec('utils.sce');

// file names
mic_fn    = '../data/03_mic.raw';
spk_fn    = '../data/03_spk.raw';
out1_fn    = '../data/out/03_out.raw';

mic = loadRawData(mic_fn);
spk = loadRawData(spk_fn);
out1 = loadRawData(out1_fn);

// plot the data
plotOneFileBelowAnother(spk_fn, mic_fn, out1_fn);
