% Goal System ID

rho = zeros(10, 1);

% **** Part 1 : zero input ****
% get ro2 ro4 ro8 ro9
% *****************************
% load data wiht zero input
%psi = load("psi_0v.mat");
load("psi_1.mat");
psi = save_psi/180*pi;  % psi is rad
dT = 0.05;
time = 5;
t = 0 : dT : time-dT;
L =100;

% psi dot
psid = zeros(L, 1);
for i = 2:L
    psid(i) = ( psi(i) - psi(i-1) )/dT;
end
psid(1) = psid(2);

% psi double dot
psidd = zeros(L, 1);
for i = 2 : L
    psidd(i) = ( psid(i) - psid(i-1) )/dT;
end
psidd(1) = psidd(2);

% plot psi, psid, psidd
figure;
plot(t, psi);
xlabel("t (s)");
ylabel("rad");
title("psi");
legend("psi");

figure;
plot(t, psi, t, psid, t, psidd);
xlabel("t (s)");
title("psi & psid & psidd");
legend("psi", "psid", "psidd");

% ro2 ro4
first =(4.3)*20+1;
last = (5.65)*20+1;
range = 7:11;%choose linear data

%g1 = -thetadd(range);
g2 = -2*cos(psi(range)).*psidd(range)+sin(2*psi(range)).*sec(psi(range)).*psid(range).^2;
%g3 = -thetad(range);
g4 = psid(range);
%g5 = 0;
%g6 = -thetadd(range);
%g7 = -cos(psi(range)).*psidd(range);
g8 = 0-psid(range);
g9 = sin(psi(range));
%g10 = 0;
e = psidd(range);

%L = length(range);
A = [g2 g4];
b = e;
x = (A'*A) \ ((A')*b);
rho(2) = x(1);
rho(4) = x(2);

% ro8 ro9
A = [g8 g9];
b = e;
x = (A'*A) \ ((A')*b);
rho(8) = x(1);
rho(9) = x(2);

save("rho_1", "rho");
%clear;