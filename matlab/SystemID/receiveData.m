% Goal: System ID

% **** Part 1: receive data ****
% Method: 1.serial port   2.Bluetooth
clc
clear;

objs = instrfind;
fclose(objs);
%objs
 %Method 1
% serial port COM6
% set BaudRate to 57600
% set terminator to CR
% CR: carriage return, /r
% LF: line feed, /n
%s = serial('COM6', 'BaudRate', 57600, 'Terminator', 'CR');

fopen(s);

%Method 2

instrhwinfo('Bluetooth');
s = Bluetooth('777', 1);
fopen(b);


%ªì©l¤ÆPID¼Æ­È
fprintf(s,'~0,112,3,1,-0.015#')%¥ª½üPID­È
fprintf(s,'~1,135,5,1,-0.015#')%¥k½üPID­È




% Collect Data
% imformation of raw data
dT = 0.005;        % sampling time
time = 10;        % record 10 second of data
length = time/dT; % # of raw data
t = 0 : dT : time - dT;
rawData = strings(length, 1);
%fopen(s);
for i = 1:length
    rawData(i) = fscanf(s);     % receive data from serial port
    %rawData(i) = fscanf(b);     % receive data from bluetooth
end

% **** Part 2: process raw data ****
% parse the raw data
data = zeros(length, 3);
for i = 1:length
    x = strsplit(rawData(i), ",");
    data(i, 3) = str2double( x(1) ) ;% left speed
    data(i, 1) = str2double( x(2) )  ; % right speed
    data(i, 2) = str2double( x(3) )   % psi
end

% plot left speed, right speed, phi with respect to t
% 390 cpr =  360 degree  ==>  1 cpr = 360/390 degree

figure;
plot(t, data(:, 1), t, data(:, 2));
xlabel("t");
ylabel("angle (degree)");
title("wheel position");
legend("left angle", "right angle");

figure;
plot(t, data(:, 3));
xlabel("t (s)");
ylabel("angle (degree)");
title("psi");


% save data

save_thetad_5v = data(:, 1);
save('thetad_balance.mat', 'save_thetad_5v');
save_psi = data(:, 3);
save('psi_balance.mat', 'save_psi');
% instrfind: Read serial port objects from memory to MATLAB workspace
%objs = instrfind;
fclose(s);