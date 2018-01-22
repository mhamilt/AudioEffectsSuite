% Chebyshev Coefficient calculator

%% It seems the maximum number of chebyshev coeffcients is around 20
%% the following script is to test the generation of coeffients for the algorithm
%% given in "the scientist and engineers guide to DSP"
clear all
close all
%% vectors to store coefficients
A = zeros(22,1);
B = A; TA = A; TB = A;

% A(2) = 1; %%INDEX ISSUE
% B(2) = 1; %%INDEX ISSUE
A(3) = 1;
B(3) = 1;

%% Main variables
SR = 44.1e3;
Fc = 2/SR; %% normalised cutoff frequency
LH = 0;  %% filter shelf type, 0 = low pass, 1 = high pass
Pr = 0; %% percentage ripple < .2929
Np = 4;  %% number of poles

%% Some internal coefficients
if(Pr~=0)
  Es = sqrt((1/(1-Pr))^2 - 1);
  Vx = (1/Np)*log(1/Es + sqrt(1/(Es^2)+1));
  Kx = (1/Np)*log(1/Es + sqrt(1/(Es^2)-1));
  Kx = cosh(Kx);
else
  Vx = 1;
  Kx = 1;
end

T = 2*tan(.5);
W = 2*pi*Fc;

%% if low pass
if(LH==0)
  K = sin(.5 - W/2)/sin(.5 + W/2);
else
  %% if high pass
  K = -cos(.5 + W/2)/cos(W/2 - .5);
end
%%% main algorithm
j_ = 3:22; % indexing variable
for i = 1:Np*.5  %%INDEX ISSUE
  %%% Sub routine
  alpha = pi/(2*Np) + (i-1-1)*(pi/Np);

  if(Pr~=0)
    Rp = -cos(alpha)*sinh(Vx)/Kx;
    Ip = sin(alpha)*cosh(Vx)/Kx;
  else
    Rp = -cos(alpha);
    Ip = sin(alpha);
  end

  M = Rp^2 + Ip^2;
  D = 4 - 4*Rp*T + M*T;

  X0 = (T^2)/D;
  X1 = (2*T^2)/D;
  X2 = X0;

  Y1 = (8-(2*M*T^2))/D;
  Y2 = (-4 - 4*Rp*T - M*T)/D;

  D = 1 + Y1*K - Y2*K^2;

  A0 =  (X0 - X1*K + X2*K^2)/D;
  A1 =  (-2*X0*K + X1 + X1*K^2 - 2*X2*K)/D;
  A2 =  (X0*K^2 - X1*K + X2)/D;

  B1 = (2*K + Y1 +Y1*K^2 - 2*Y2*K)/D;
  B2 = (-(K^2) - Y1*K + Y2)/D;

  if (LH == 1)
    A1 = -A1; B1 = -B1;
  end

  TA = A;
  TB = B;

  A(j_) = A0*TA(j_) + A1*TA(j_-1) + A2*TA(j_-2);
  B(j_) =  (TB(j_) - B1*TB(j_-1) - B2*TB(j_-2));

end %% for i


B(3) = 0; %% B is the numerator %%INDEX ISSUE
A = A(3:(3+Np))';
B = -B(3:(3+Np))';

%%%% Normalising
SA = 0; SB = 0;
if (LH==0)
  SA = sum(A); SB = sum(B);
else
  for i = 1:Np
    SA = SA + A(i)*(-1)^(i-1);
    SB = SB + B(i)*(-1)^(i-1);
  end
end

gain = SA/(1-SB);

A = A/gain;
%%% end

% R = 0.00; % the number of dB of peak to peak ripple in the pass band
% Wp = 0.01; % Normalised frequency.
m = Np; % Filter order
%
% % % FilterType = 'low';
% [bmat,amat] = cheby1(Np,12,Fc,'low');
% a=bmat;
% b=-amat(2:m+1); %note the change in sign

% %
b = B;
a = A;

SR = 44.1e3;
% x = zeros(SR,1);
% x(1) = 1;
x = (rand(SR*10,1));
% x = ones(2000 ,1);
% x = sin(2*pi*100*[0:44100]/SR)';
% [file, path] =  uigetfile({'*.wav';'*.flac';'*.ogg';...
%                           '*.mp3'; '*.m4a'; '*.mp4'},... % unsupported in OS X
%                           'Choose a Sound File'); %% No Message in OS X
%
% if file == 0
%   error('Load file cancelled')
% end
% [x, SR] = audioread('/Users/admin/Documents/Masters/PBMMI/Audio_Examples/TestGuitarPhraseMono.wav');


N = length(x);
m = Np;
% x=[zeros(m,1);x];

% y=[zeros(N+m,1)];
% y=[zeros(N,1)];
y=[ones(N,1)*.5];

xrms= zeros(N,1);
win = 128;
for n = win+1:N
  xrms(n) = sqrt(sum(x(n:-1:n-(win-1)).^2)/(win));
  % xrms(n) = sum(x(n:-1:n-(win-1)).^2)/(win);
end
% xrms = 2*x.*x;
% xrms = abs(x);
for n = m+1:N
  y(n) = a*x(n:-1:n-m) + b*y(n:-1:n-m);
end

% y = sqrt(y);
figure(1)
% plot(x);
hold on
% plot(xrms);
plot(y,'k','LineWidth',2)
hold off

XF = fft(x);
YF = fft(y);

fx = linspace(0, SR, length(XF));
fy = linspace(0, SR, length(YF));

% Plot
% plot(fx,20*log10(abs(XF)),fy,20*log10(abs(YF)));
figure(2)
subplot(2,1,1)
plot(fx,XF)
subplot(2,1,2)
% plot(fy,abs(YF));

ymin = 0.493;
ymax = 0.507;
ynorm = 1/(ymax-ymin);

swing = SR*0.015;
base = SR*0.01;

delaySig = (((y-ymin)*ynorm)*swing) + base;
plot(delaySig);


% (y - (.4 + depth(1 ~ 10)*.08)) * depth*intensity*constant

% Joining LFOs
ly = length(y);
fadeSize = 880000;
i = 1:fadeSize;
t = (i/(.5*fadeSize)) - 1;

fadeIn = sqrt(.5*(1+t));
fadeOut = sqrt(.5*(1-t));
w = zeros((2*ly)-(fadeSize*.5),1);
w1 = w;
w2 = w;
y1 = y;
y2 = y(end:-1:1);


% y1 = ones(size(y));
% y2 = y1;

y1(end-fadeSize+1:end) = y1(end-fadeSize+1:end).*fadeOut';
y2(i) = y2(i).*fadeIn';

figure(4)
w1(1:ly) = y1;
plot(w1)
hold on
w2(ly-(fadeSize)+1:end-(fadeSize*.5)) = y2;
plot(w2);
plot(w1+w2);
