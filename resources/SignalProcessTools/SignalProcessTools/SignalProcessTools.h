#ifndef SIGNALPROCESSTOOLS_H
#define SIGNALPROCESSTOOLS_H
#include <stdlib.h>
#include <stdio.h>
#include "engine.h"
#include <string>
#pragma comment( lib, "libeng.lib" )
#pragma comment( lib, "libmx.lib" )
#pragma comment( lib, "libmat.lib" )
using namespace std;

struct vector
{
	double *data;
	int length;
};

struct WaveData
{
	vector wave;
	int sampleRate;
};

/*
*	if you need to access the data at (f,t)
*	try real.data[t*freq.length+f] and
*		imag.data[t*freq.length+f]
*/
struct FreqData
{
	vector real;
	vector imag;
	vector time;
	vector freq;
};

/*
*	This tool uses MATLAB engine to provide a C interface to
*		1. read and write .wav
*		2. plot wave
*		3. calculate stft
*		4. draw stft spec
*	if you need to more advanced feature of MATLAB
*	please refer to 
*	http://jp.mathworks.com/help/matlab/calling-matlab-engine-from-c-c-and-fortran-programs.html?lang=en
*/
class SignalProcessTools
{
public:
	SignalProcessTools();
	
	/*
	e.g. 
		waveData wave = waveRead("e:/xxx.wav");
		                          ^^^^^^^^^^
		waveWrite("e:/xxx.wav", wave);
		           ^^^^^^^^^^
		make sure file name here is an absolute path.
	*/
	WaveData waveRead(string filename);
	void waveWrite(string filename, WaveData &data);

	/*
	e.g.
		plot(X);
		plot(X, Y);
		plot(X, Y, "Diagram", "x-axis", "y-axis");
	*/
	void plot(vector dataX);
	void plot(vector dataX, vector dataY, \
		string title = "", string xLabel = "", string yLabel = "");

	/*
	e.g.
		FreqData freq = stft(wave);
		FreqData freq = stft(wave, "rectwin", 256, 192, 256);
	*/
	FreqData stft(WaveData data, string windowName = "hamming", \
		int windowN = 256, int NOverlap = 192, int NFFT = 256);

	void plotSpec(FreqData data);


private:
	Engine *ep;
	void putVector(vector data, string name);
	void getVector(vector &data, string name);
	void putWaveData(WaveData data);
	void put2DData(double *data, int n1, int n2, string name);
};
#endif