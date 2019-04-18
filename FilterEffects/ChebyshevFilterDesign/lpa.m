% lpa.m - lowpass analog filter design
%
% function [N,B,A] = lpa(Wp, Ws, Ap, As, type)
%
% Wp,Ws = passband and stopband frequencies in rad/sec
% Ap,As = passband and stopband attenuations in dB
% type  = 0,1,2,3 for Butterworth, Chebyshev-1, Chebyshev-2, Elliptic
%
% N   = filter order
% B,A = (L+1)x3 matrices of numerator and denominator coefficients, L=floor(N/2)
function [N,B,A] = lpa(Wp, Ws, Ap, As, type)

  if nargin==0, help lpa; return; end
    ep = sqrt(10^(Ap/10)-1); es = sqrt(10^(As/10)-1);
    k = Wp/Ws; k1 = ep/es;
    switch type
      case 0
      Nexact = log(1/k1) / log(1/k);
      N = ceil(Nexact);
      case 1
      Nexact = acosh(1/k1) / acosh(1/k);
      N = ceil(Nexact);
      case 2
      Nexact = acosh(1/k1) / acosh(1/k);
      N = ceil(Nexact);
      k = 1/cosh(acosh(1/k1) / N);
      case 3
      [K,Kp] = ellipk(k);
      [K1,K1p] = ellipk(k1);
      Nexact = (K1p/K1)/(Kp/K);
      N = ceil(Nexact);
      k = ellipdeg(N,k1);
    end
    % selectivity and discrimination parameters
    % determine order N
    % recalculate k to satisfy degree equation
    % recalculate k to satisfy degree equation
    r = mod(N,2); L = (N-r)/2; i = (1:L)'; u=(2*i-1)/N;
    switch type                               % determine poles and zeros
      case 0
      pa = Wp * j * exp(j*u*pi/2) / ep^(1/N);
      pa0 = -Wp / ep^(1/N);
      case 1
      v0 = asinh(1/ep) / (N*pi/2);
      pa = Wp * j * cos((u-j*v0)*pi/2);
      pa0 = -Wp * sinh(v0*pi/2);
      case 2
      v0 = asinh(es) / (N*pi/2);
      za =  Wp ./ (j*k*cos(u*pi/2));
      pa =  Wp ./ (j*k*cos((u-j*v0)*pi/2));
      pa0 = -Wp / (k*sinh(v0*pi/2));
      case 3
      v0 = -j * asne(j/ep, k1) / N;
      21
      za = Wp * j ./(k*cde(u,k));
      pa = Wp * j * cde(u-j*v0, k);
      pa0 = Wp * j * sne(j*v0, k);
    end
    B = [ones(L+1,1), zeros(L+1,2)];
    A = [ones(L,1), -2*real(1./pa), abs(1./pa).^2];
    A = [[1, -r*real(1/pa0), 0]; A];
    % determine coefficient matrices
    if type==2 | type==3,
      B(2:L+1,:) = [ones(L,1), -2*real(1./za), abs(1./za).^2];
    end
    Gp = 10^(-Ap/20);
    if type==1 | type==3,                     % adjust dc gain
      B(1,1) = Gp^(1-r);
    end
