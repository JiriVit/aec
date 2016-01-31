// Acoustic Echo Canceller

funcprot(0);
exec('aec.sce');

// file names
mic_fn    = '../data/01_mic_clvl12.raw';
spk_fn    = '../data/01_spk_clvl12.raw';

// load data from raw files
mic = loadRawData(mic_fn);
spk = loadRawData(spk_fn);

y = doNlms(mic(1:1000), spk(1:1000));
