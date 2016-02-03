// Acoustic Echo Canceller

funcprot(0);
exec('aec.sce');

// file names
mic_fn    = '../data/01_mic_clvl12.raw';
spk_fn    = '../data/01_spk_clvl12.raw';

// load data from raw files
mic = loadRawData(mic_fn);
spk = loadRawData(spk_fn);

// [y, e] = doNlms(mic(1000:$), spk(1000:$));

t = linspace(0, 3000-1, 24000);

subplot(211);
plot(t, spk);
xtitle('loudspeaker');

subplot(212);
plot(t, mic);
xtitle('microphone');

