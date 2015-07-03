#include "SignalProcessTools.h"

string wavName = "e:/6010.wav";
void testIO()
{
	SignalProcessTools tools;
	// make sure the file name here is the absolute path
	// or matlab will try to find the .wav file at the root path of MATLAB
	WaveData wave = tools.waveRead(wavName);
	tools.waveWrite("e:/haha.wav", wave);
}
/*
*	if you need to observe part of the wave:
*		WaveData wave =  tools.waveRead("6010.wav");
*		vector partWave = { wave.wave.data+1000, 100 };
*		tools.plot(partWave);
*/
void testPlot()
{
	SignalProcessTools tools;
	WaveData wave = tools.waveRead(wavName);

	// set time vector...
	vector time;
	time.length = wave.wave.length;
	time.data = new double[wave.wave.length];
	double delta = 1.0 / wave.sampleRate;
	double acc = 0.0;
	for (int i = 0; i < wave.wave.length; i++, acc+=delta)
		time.data[i] = acc;

	// call plot
	// or just use tools.plot(wave.wave);
	tools.plot(time, wave.wave, "Wave Diagram", "time", "wave");
}

void testSpec()
{
	SignalProcessTools tools;
	WaveData wave = tools.waveRead(wavName);
	FreqData freq = tools.stft(wave);
	tools.plotSpec(freq);
}

int main()
{
	//testIO();
	//testPlot();
	testSpec();
}