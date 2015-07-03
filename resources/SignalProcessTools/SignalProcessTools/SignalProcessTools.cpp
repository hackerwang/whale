#include "SignalProcessTools.h"
#include "matrix.h"
#include <sstream>
SignalProcessTools::SignalProcessTools()
{
	if (!(ep = engOpen("\0")))
	{
		fprintf(stderr, "\nCan't start MATLAB engine\n");
	}
}

void SignalProcessTools::plot(vector dataX)
{
	putVector(dataX, "X");
	engEvalString(ep, "plot(X);");
}

void SignalProcessTools::plot(vector dataX, vector dataY, \
							  string title, string xLabel, string yLabel)
{
	putVector(dataX, "X");
	putVector(dataY, "Y");
	engEvalString(ep, "plot(X,Y);");
	if (title != "")  engEvalString(ep, ("title('"+title+"');").c_str());
	if (xLabel != "")  engEvalString(ep, ("xlabel('"+xLabel+"');").c_str());
	if (yLabel != "")  engEvalString(ep, ("ylabel('"+yLabel+"');").c_str());
}

WaveData SignalProcessTools::waveRead(string filename)
{
	engEvalString(ep, ("[R,Fs]=audioread('"+filename+"');").c_str());
	engEvalString(ep, "R=R(:,1);");
	WaveData data;
	getVector(data.wave, "R");
	mxArray *Fs = engGetVariable(ep, "Fs");
	data.sampleRate = (int)mxGetPr(Fs)[0];
	return data;
}

void SignalProcessTools::waveWrite(string filename, WaveData &data)
{
	putWaveData(data);
	string cmd = "audiowrite('"+filename+"',W,Fs);";
	engEvalString(ep, cmd.c_str());
}

string intToString(int value)
{
	stringstream ss;
	ss<<value;
	return ss.str();
}

void SignalProcessTools::putWaveData(WaveData data)
{
	putVector(data.wave, "W");
	mxArray *Fs;
	Fs = mxCreateDoubleScalar(data.sampleRate);
	engPutVariable(ep, "Fs", Fs);
}

void SignalProcessTools::putVector(vector data, string name)
{
	mxArray *W;
	W = mxCreateDoubleMatrix(1, data.length, mxREAL);
	memcpy((void *)mxGetPr(W), (void *)(data.data), data.length*sizeof(double));
	engPutVariable(ep, name.c_str(), W);
}

void SignalProcessTools::getVector(vector &data, string name)
{
	mxArray *result = engGetVariable(ep, name.c_str());
	const mwSize size = mxGetNumberOfElements(result);
	int l = (int)size;
	double *d = new double[l];
	memcpy( (void *)d, (void *)mxGetPr(result), l*sizeof(double));
	data.data = d;
	data.length = l;
}

FreqData SignalProcessTools::stft(WaveData data, string windowName /* = "hamming" */, 
					int windowN /* = 256 */, int NOverlap /* = 192 */, int NFFT /* = 256 */)
{
	putWaveData(data);
	string cmd = "h=window('"+windowName+"',"+intToString(windowN)+")";
	engEvalString(ep, cmd.c_str());
	cmd = "[B,freq,time]=spectrogram(W,h,"+intToString(NOverlap)+","+intToString(NFFT)+",Fs);";
	engEvalString(ep, cmd.c_str());
	cmd = "BReal=real(B);BImag=imag(B);";
	engEvalString(ep, cmd.c_str());
	FreqData freq;
	getVector(freq.freq, "freq");
	getVector(freq.time, "time");
	getVector(freq.real, "BReal");
	getVector(freq.imag, "BImag");
	return freq;
}

void SignalProcessTools::put2DData(double *data, int n1, int n2, string name)
{
	mxArray *W;
	W = mxCreateDoubleMatrix(n1, n2, mxREAL);
	memcpy((void *)mxGetPr(W), (void *)(data), n1*n2*sizeof(double));
	engPutVariable(ep, name.c_str(), W);
}

void SignalProcessTools::plotSpec(FreqData data)
{
	int n1 = data.freq.length;
	int n2 = data.time.length;
	put2DData(data.real.data, n1, n2, "BReal");
	put2DData(data.imag.data, n1, n2, "BImag");
	string cmd = "B=BReal+i*BImag;imagesc(time,freq,abs(B));";
	engEvalString(ep, cmd.c_str());
	cmd = "axis('xy');xlabel('Ê±¼ä (Ãë)'); ylabel('ÆµÂÊ (Hz)');";
	engEvalString(ep, cmd.c_str());
}