rho1 = 0.01545;
rho2 = -0.47775;
rho3 = -0.00703;
rho4 = 0.4115;
rho5 = 7.824517;
rho6 = 0.066283;
rho7 = -0.94603;
rho8 = -0.34739;
rho9 = -0.45349;
rho10 = 59.71147;

q = 1/((1+2*rho2)*(rho6+rho7)-rho1);
c1 = q*(rho6+rho7);
c2 = q*(-rho1);
c3 = -q;
c4 = q*(1+2*rho2);

A = [0 1 0 0; c2*rho9 c1*rho4-c2*rho8 0 -c1*rho3+c2*rho8; 0 0 0 1; c4*rho9 c3*rho4-c4*rho8 0 -c3*rho3+c4*rho8]
B = [0; c1*rho5-c2*rho10; 0; c3*rho5-c4*rho10]
C = [1 0 0 0; 0 0 1 0];     %psi, theta
D = zeros(2,1);
[n,d] = ss2tf(A,B,C,D);
h = tf(n(1,:), d);
%bode(h);
rlocus(h);
grid;

