s = tf('s');
kp = 113;
ki = 3;
kd=0.8;
num_GH=  108.3* s^2 - 41.76*s - 8.345e-14;
den_GH=s^4 - 7.087 *s^3 + 2.769 *s^2 - 0.05792*s;
GH=num_GH/den_GH*(kp+ki/s+kd*s);
CL = GH/(1+GH);
rlocus(GH)
figure(1)
stepinfo(CL)
figure(2)
step(CL)
figure(3)
margin(GH)
grid on
%s^4 - 7.087 s^3 + 2.769 s^2 - 0.05792 s