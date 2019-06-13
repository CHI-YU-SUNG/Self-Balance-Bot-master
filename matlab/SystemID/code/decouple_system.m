% decoupled system
clear;

% original system
%{
A =
         0    1.0000         0         0
    3.8988   21.3271         0   14.9660
         0         0         0    1.0000
   -8.9779  -20.9776         0  -29.9497
%}
% original pole
% 0, -22.9671, 14.2909, 0.0536

% psi
A = [    0    1.0000         0         0
    3.8988   21.3271         0         0
         0         0         0    1.0000
         0         0         0  -29.9497];
B = [0;
    -0.9856;
    0;
    -30.6397];
C = [1 0 0 0;  %psi
     0 0 1 0]; %theta
D = [0;
     0];
[n,d] = ss2tf(A,B,C,D);


h_decouple_psi = tf(n(1,:), d)
[p1,z1] = pzmap(h_decouple_psi)
% after pole-zero cancellation: 21.5084, -0.1813

h_decouple_theta = tf(n(2,:), d)
[p2,z2] = pzmap(h_decouple_theta)
% after pole-zero cancellation 0, -29.9497