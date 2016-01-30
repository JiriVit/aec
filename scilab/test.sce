exec('aec.sce');

// file names
mic_fn    = '../data/01_mic_clvl12.raw';
spk_fn    = '../data/01_spk_clvl12.raw';
micwav_fn = '../data/01_mic_clvl12.wav';
spkwav_fn = '../data/01_spk_clvl12.wav';

// load data from raw files
mic = loadRawData(mic_fn);
spk = loadRawData(spk_fn);

// store data to wav files
wavwrite(mic./32768, 8000, micwav_fn);
wavwrite(spk./32768, 8000, spkwav_fn);


