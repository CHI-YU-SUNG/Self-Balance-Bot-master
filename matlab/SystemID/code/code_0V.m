%https://www.youtube.com/watch?v=7IDAYfAb0k4&fbclid=IwAR2CEPm9rwMM-8ST1MuPx1SiwrHwgw5pW0m-e5K0IM9h3Hh6Fnbcu_CnQE0
% The length of the crank and the slider in m
%P為輪子的中心點，P1為平衡桿在輪子的位置，P2為平衡桿的最高處
%Point P1
P1 = [4 2];
P = [0 0];
%Parameteers of the plot
axis(gca,'equal');  %The aspect ratio
axis([0 50 0 20]);   %The limits

%The angular speed of the crank in rad/s
k = 1;
%Now comes the loop
x=load('0V_5V.mat');
x=x.data;
for t=1:100    %t is the time in tenths of seconds
    P = P + [x(t,2)*0.1 0];
    P1 = P + [5 2];
    theta = x(t,1) + 1.57;
    %The point P2
    P2 = [6*cos(theta) 6*sin(theta)] + P1;
    %The crank line
    crank = line([P1(1) P2(1)],[P1(2) P2(2)]);
    %The point P2 trajectory
    P2_traj = viscircles(P1, 2);
    %The point P1 and P2
    P1_circ = viscircles(P1, 0.1);
    P2_circ = viscircles(P2, 0.1);
    %The intervsl to update the plot
    pause(0.05);
    %Delete the previous line
    delete(crank);
    delete(P1_circ);
    delete(P2_circ);
    delete(P2_traj);
end