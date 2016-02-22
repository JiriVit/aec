// Acoustic Echo Canceller

funcprot(0);
exec('aec.sce');
exec('utils.sce');

// file names
mic_fn    = '../data/01_mic_clvl12.raw';
spk_fn    = '../data/01_spk_clvl12.raw';

// load data from raw files
mic = loadRawData(mic_fn);
spk = loadRawData(spk_fn);

// time domain
t = linspace(0, 3000-1, 24000);

// echo cancellation
e = doMdf(spk(1000:1012), mic(1000:1012));

//// plot results
//subplot(211);
//plot(t, spk);
//xtitle('loudspeaker');
//
//subplot(212);
//plot(t, mic);
//xtitle('microphone');
//
