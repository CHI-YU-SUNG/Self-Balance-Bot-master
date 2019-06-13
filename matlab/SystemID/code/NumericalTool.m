b = [7.839 -7.839];
a = [1 -1.562 0.6413];
h = tf(b, a);
bode(h);
[h,w] = freqz(b, a, 100);

%plot(w/pi, 20*log10(abs(h)));
% a oscillator and why??