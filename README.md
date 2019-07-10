# Self-Balance-Bot-master(雙輪平衡車)

## Introduction
This projects purpose was to design and build a two wheeled upright robot.A two-wheeled
differential drive mobile robot based on the inverted pendulum model is built as a platform
to investigate the use of a Kalman filter to estimate the tilt angle. As the robot is
mechanically unstable, it becomes necessary to explore the possibilities of
implementing a control system to keep the system in equilibrium.

  This project examines the suitability and evaluates the performance of a Linear Quadratic
Regulator (LQR) and a Pole-placement controller in balancing the system. The LQR controller
uses several weighting matrix to obtain the appropriate control force to be applied to the
system while the Pole placement requires the poles of the system to be placed to guarantee
stability. As the robot will be moving about on a surface, a PID controller is implemented to
control the trajectory of the robot.
## Hardware

1. Accelerometer: 
2. Main Controller:Arduino nano
3. Motor Driver:
4. DC Motors: 
5. Wheels:
6. Rotary Encodrs: 

### System Mechanical Structure
![](https://i.imgur.com/g31t4Qn.jpg)
### System Structure
![](https://i.imgur.com/oQ0a6CK.png)

## Function
* Position contol
* Speed control
* Image recognition
## Simulation
### matlab Modeling and Model Verification
* ID :
First of all, I collect some system data including "**Angle of car inclination**" and "**Wheel angle**". Base on LST(method of least-squares) method to derive the formula of dynamics equation, then compete the system's transfer function by matlab.
* Simulink : Cascade form and Parralled form to simulate which condition of PID would keep satble status.  
## Product
 Your support will let me be better
## video
You will see in my youtube channel later. 

