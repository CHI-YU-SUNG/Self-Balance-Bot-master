% Goal: System ID

% **** Part 1: receive data ****
% Method: 1.serial port   2.Bluetooth
clear;

% Method 1
% serial port COM8
% set BaudRate to 9600
% set terminator to CR
% CR: carriage return, /r
% LF: line feed, /n
s = serial('COM3', 'BaudRate', 9600, 'Terminator', 'CR');
fopen(s);

% Method 2
%{
instrhwinfo('Bluetooth');
b = Bluetooth('Slave', 1);
fopen(b);
%}

% Collect Data
% imformation of raw data
dT = 0.05;        % sampling time
time = 30;        % record 30 second of data
length = time/dT; % # of raw data
t = 0 : dT : time - dT;
rawData = strings(length, 1);
fopen(s);
for i = 1:length
    rawData(i) = fscanf(s);     % receive data from serial port
%    rawData(i) = fscanf(b);     % receive data from bluetooth
end

% **** Part 2: process raw data ****
% parse the raw data
data = zeros(length, 3);
for i = 1:length
    x = strsplit(rawData(i), ",");
    data(i, 1) = str2double( x(1) );   % left speed
    data(i, 2) = str2double( x(2) );   % right speed
    data(i, 3) = str2double( x(3) );   % psi
end

% plot left speed, right speed, phi with respect to t
% 390 cpr =  360 degree  ==>  1 cpr = 360/390 degree

figure;
plot(t, data(:, 1), t, data(:, 2));
xlabel("t");
ylabel("speed (cpr/s)");
title("wheel speed");
legend("left speed", "right speed");

figure;
plot(t, data(:, 3));
xlabel("t (s)");
ylabel("angle (degree)");
title("psi");


% save data

save_thetad_5v = data(:, 1);
save('thetad_5v_10.mat', 'save_thetad_5v');
save_psi = data(:, 3);
save('psi_35.mat', 'save_psi');
% instrfind: Read serial port objects from memory to MATLAB workspace
objs = instrfind;
fclose(objs);