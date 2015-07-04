%%%%%%%%%%%%%%%%%%%%
%% Original whalesong
%%%%%%%%%%%%%%%%%%%%
%Read original wav file
[y,fs]=wavread('F:\\Git\\whale\\resources\\whalesong.wav');
sound(y,fs)
time=(1:length(y))/fs;
%Plot original waveform
figure(1);
plot(time,y);
axis([0,0.38,-1,1]);
title('whalesong完整波形');
xlabel('time/s');
figure(2);
x=1.05e4:1.1e4;
plot(x,y(1.05e4:1.1e4))
title('whalesong局部波形');
xlabel('采样点');

%%%%%%%%%%%%%%%%%%%%
%% Fixed frequency
%%%%%%%%%%%%%%%%%%%%
%Generate 2744Hz
y1=sin(2*pi*2744*time);
sound(y1,fs)
%Plot waveform
figure(3);
plot(time,y1);
axis([0,0.38,-1,1]);
title('synfixed完整波形');
xlabel('time/s');
figure(4);
x=1.05e4:1.1e4;
plot(x,y1(1.05e4:1.1e4))
title('synfixed局部波形');
xlabel('采样点');
wavwrite(y1,fs,'F:\\Git\\whale\\resources\\synfixed.wav');

%%%%%%%%%%%%%%%%%%%%
%% Single variable frequency
%%%%%%%%%%%%%%%%%%%%
%original STFT
h=window('hamming',256);
NOverlap=192;
NFFT=256;
figure(5);
spectrogram(y,h,NOverlap,NFFT,fs,'yaxis');
%Generate base frequency
y2(4001:11999)=sin(2*pi*2800*time(4001:11999));
%Generate swept frequency
y2(1:4000)=chirp(time(1:4000),2250,4000/fs,2800,'linear',-110);
y2(12000:16572)=chirp(time(12000:16572),5057,16572/fs,1940,'linear',245);
%Generate variable amplitude
a2(1:4000)=linspace(0.2,1,4000);
a2(4001:11999)=ones(7999,1);
a2(12000:16572)=linspace(1,0,4573);
y2=a2.*y2;
sound(y2,fs)
%Plot waveform
figure(6);
plot(time,y2);
axis([0,0.38,-1,1]);
title('synsingle完整波形');
xlabel('time/s');
figure(7);
x=3500:4500;
plot(x,y2(3500:4500))
title('synsingle局部波形');
xlabel('采样点');
%Plot STFT
figure(8);
spectrogram(y2,h,NOverlap,NFFT,fs,'yaxis');
wavwrite(y2,fs,'F:\\Git\\whale\\resources\\synsingle.wav');

%%%%%%%%%%%%%%%%%%%%
%% Mutiple variable frequency
%%%%%%%%%%%%%%%%%%%%
%Generate 10.875kHz
y3_1(1:11999)=sin(2*pi*10875*time(1:11999));
y3_1(12000:16572)=chirp(time(12000:16572),16609,16572/fs,8690,'linear',245);
%Generate 5.5kHz
y3_2(4001:11999)=sin(2*pi*5500*time(4001:11999));
y3_2(1:4000)=chirp(time(1:4000),5800,4000/fs,5500,'linear',53);
y3_2(12000:16572)=chirp(time(12000:16572),7127,16572/fs,4880,'linear',130);
%Generate 8.19kHz
y3_3(4001:11999)=sin(2*pi*8190*time(4001:11999));
y3_3(1:4000)=chirp(time(1:4000),6250,4000/fs,8190,'quadratic',-45,'convex');
y3_3(12000:16572)=chirp(time(12000:16572),14094,16572/fs,5940,'linear',175);
%Compose
y3=0.8*y2+0.1*a2.*y3_1+0.05*a2.*y3_2+0.05*a2.*y3_3;
%Plot waveform
figure(9);
plot(time,y3);
axis([0,0.38,-1,1]);
title('synmulti完整波形');
xlabel('time/s');
figure(10);
x=3500:4500;
plot(x,y3(3500:4500))
title('synmulti局部波形');
xlabel('采样点');
%Plot STFT
figure(11);
spectrogram(y3,h,NOverlap,NFFT,fs,'yaxis');
wavwrite(y3,fs,'F:\\Git\\whale\\resources\\synmulti.wav');
