import serial
import cv2 
import sys
import Tkinter
if(sys.version_info[0]<3):
    from Tkinter import *
else:
    from tkinter import *
import numpy as np
"""
real=cv2.imread("test.jpg")
cv2.imshow("REAL",real)
img=cv2.imread("test.jpg",cv2.IMREAD_GRAYSCALE)
"""
target_s=20000
cap=cv2.VideoCapture(0)
ser=serial.Serial('/dev/777',115200)
while(1):
    
    ret,frame=cap.read()
    if ret==False:
        break
    (h,w) = frame.shape[:2]
    """h=480 w=640"""
    center = (w / 2,h / 2)
    M = cv2.getRotationMatrix2D(center,180,1)
    rotated = cv2.warpAffine(frame,M,(w,h))
    gray=cv2.cvtColor(rotated,cv2.COLOR_BGR2GRAY)
    _, threshold = cv2.threshold(gray,80,255,cv2.THRESH_BINARY)
    

    contours,_=cv2.findContours(threshold,cv2.RETR_LIST,cv2.CHAIN_APPROX_SIMPLE)


    for cnt in contours:
        approx=cv2.approxPolyDP(cnt,0.02*cv2.arcLength(cnt,True),True)
        
        x=approx.ravel()[0]
        y=approx.ravel()[1]
        sumx=0
        sumy=0
        midx=0
        midy=0
        s=0
        if(x>1 and y>1):
            for i in range(len(approx.ravel())/2):
                sumx+=approx.ravel()[2*i]
                sumy+=approx.ravel()[2*i+1]
                if i==len(approx.ravel())/2-1:
                    s+=approx.ravel()[2*i]*approx.ravel()[1]-approx.ravel()[0]*approx.ravel()[2*i+1]
                else:
                    s+=approx.ravel()[2*i]*approx.ravel()[2*i+3]-approx.ravel()[2*i+1]*approx.ravel()[2*i+2]
                """print("%d %d"%(approx.ravel()[2*i],approx.ravel()[2*i+1])) """               
            midx=sumx/len(approx.ravel())*2
            midy=sumy/len(approx.ravel())*2
            if(midx<480 and midy<640):
                b,g,r=rotated[midy,midx]
            
            if len(approx)==3:
                if (s>target_s and 150<midx<350 and 200<midy<450):
                    print("a triangle at (%d %d) s=%d" %(midy,midx,s/2))
                    ser.write("1")
            elif len(approx)==4:
                if (s>target_s and 150<midx<350 and 200<midy<450):
                    print("a rectangle at (%d %d) s=%d" %(midy,midx,s/2))
            		ser.write("2")
			elif len(approx)==5:
                if (s>target_s and 150<midx<350 and 200<midy<450):
                    print("a pentagon at (%d %d) s=%d" %(midy,midx,s/2))
            elif len(approx)==10:
                s=0
                for i in range(len(approx.ravel())/4):
                    if i==len(approx.ravel())/4-1:
                        s+=approx.ravel()[18]*approx.ravel()[1]+approx.ravel()[0]*approx.ravel()[3]+approx.ravel()[2]*approx.ravel()[19]-approx.ravel()[0]*approx.ravel()[19]-approx.ravel()[2]*approx.ravel()[1]-approx.ravel()[19]*approx.ravel()[3]
                    else:
                        s+=approx.ravel()[4*i+2]*approx.ravel()[4*i+5]+approx.ravel()[4*i+4]*approx.ravel()[4*i+7]+approx.ravel()[4*i+6]*approx.ravel()[4*i+3]-approx.ravel()[4*i+4]*approx.ravel()[4*i+3]-approx.ravel()[4*i+6]*approx.ravel()[4*i+5]-approx.ravel()[4*i+2]*approx.ravel()[4*i+7]
                for i in range(5):
                    if i==4:
                        s+=approx.ravel()[18]*approx.ravel()[3]-approx.ravel()[2]*approx.ravel()[19]
                    else:
                        s+=approx.ravel()[4*i+2]*approx.ravel()[4*i+7]-approx.ravel()[4*i+6]*approx.ravel()[4*i+3]
                if (s>target_s and 150<midx<350 and 200<midy<450):
                    print("a star at (%d %d) s=%d" %(midy,midx,s/2))
            else:
                if (s>target_s and 150<midx<350 and 200<midy<450):
                    print("a circle at (%d %d) s=%d" %(midy,midx,s/2))
					ser.write("3")
        if (s>target_s and 150<midx<350 and 200<midy<450):
            cv2.drawContours(rotated,[approx],0,(0,0,255),2)
            print("RGB=(%d %d %d) fiirstspot=(%d %d)"%(r,g,b,approx.ravel()[0],approx.ravel()[1]))
            if(r>b and r>g):
                print("color=red")
            elif (g>b and g>r):
                print("color=green")
            elif (b>g and b>r):
                print("color=blue")
            else:
                print("color=unknown")
    cv2.imshow("shapes",rotated)

    cv2.imshow("Threshold",threshold)
    if cv2.waitKey(5)&0xFF== ord('q'):
        break
cv2.destoryALLwindows()
