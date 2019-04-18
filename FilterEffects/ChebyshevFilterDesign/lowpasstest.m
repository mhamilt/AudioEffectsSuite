%% lpa script
clear all;
close all;
SR = 44.1e3;
f0 = SR/2;
w0 = 2*pi*f0/SR;
a = [1, 1];
b = [0];
m = length(a)-1;

x = zeros(SR,1);
x(1) = 1;
N = length(x);
x=[zeros(m,1);x];


y=zeros(N+m,1);



for n = m+1:m+N
    y(n) = a*x(n:-1:n-m) + b*y(n-1:-1:n-m);
end




% FFT
XF = fft(x);
YF = fft(y);

fx = linspace(0, SR, length(XF));
fy = linspace(0, SR, length(YF));


w = linspace(0, 2*pi*SR, SR);
H = abs(1 + exp(-1j*w/SR));
fh = linspace(0,SR,length(H));

% Plot
% plot(fx,20*log10(abs(XF)),fy,20*log10(abs(YF)));
subplot(3,1,1)
plot(fx,abs(XF))
subplot(3,1,2)
plot(fy,abs(YF));
subplot(3,1,3)
plot(fh,H);
