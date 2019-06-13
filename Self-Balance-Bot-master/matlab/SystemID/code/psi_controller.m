% PID control for psi(angle of inclination)
clear;
clc;

KP = 112;
KI = 1000;
KD = 0.01;

num = conv([0.9856 488.1], [KD KP KI]);
den = [1 8.623 -328.7 17.6 0] + [0 num];

Gp = tf(num, den);