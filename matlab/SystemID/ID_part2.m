% **** Part 2 : 5v ****
% get ro1 ro3 ro5 ro6 ro7 ro10
% *****************************
load("rho_1.mat");
load("psi_5v_1.mat");
psi = save_psi/180*pi;       % psi is rad
thetad = save_thetad_5v/180*pi; % theta is rad

dT = 0.05;
time = 5;
t = 0 : dT : time-dT;
L = 100;

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

% plot psi, psid and psidd
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

% theta double dot
thetadd = zeros(L, 1);
for i = 2:L
    thetadd(i) = ( thetad(i) - thetad(i-1) )/dT;
end
thetadd(1) = thetadd(2);

% plot thetad thetadd
figure;
plot(t, thetad);
xlabel("t (s)");
ylabel("rad/s");
title("thetad");
legend("thetad");

figure;
plot(t, thetad, t, thetadd);
xlabel("t (s)");
title("thetad & thetadd");
legend("thetad", "thetadd");


first =(6.8)*20+1;
last = (7.15)*20+1;
range = 9:11;
L =3;

g1 = -thetadd(range);
g2 = -2*cos(psi(range)).*psidd(range)+sin(2*psi(range)).*sec(psi(range)).*psid(range).^2;
g3 = -thetad(range);
g4 = psid(range);
g5 = (1)*ones(L, 1);
g6 = -thetadd(range);
g7 = -cos(psi(range)).*psidd(range);
g8 = thetad(range)-psid(range);
g9 = sin(psi(range));
g10 = (1)*ones(L, 1);
e = psidd(range);

% ro1 ro3 ro5
A = [g1 g3 g5];
%b = e - rho(2)*g2 - rho(4)*g4;
b = 0 - rho(2)*g2 - rho(4)*g4;
x = (A'*A) \ ((A')*b);
rho(1) = x(1);
rho(3) = x(2);
rho(5) = x(3);

% ro6 ro7 ro10
A = [g6 g7 g10];
%b = e - rho(8)*g8 - rho(9)*g9;
b = 0 - rho(8)*g8 - rho(9)*g9;
x = (A'*A) \ ((A')*b);
rho(6) = x(1);
rho(7) = x(2);
rho(10) = x(3);
save("rho_final_10", "rho");