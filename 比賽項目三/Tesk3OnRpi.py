from picamera.array import PiRGBArray
from picamera import PiCamera
import time
import cv2 
import numpy as np
import serial
''''
camera = PiCamera()
rawCapture = PiRGBArray(camera)
 
# allow the camera to warmup
time.sleep(0.1)
 
# grab an image from the camera
camera.capture(rawCapture, format="bgr")
frame = rawCapture.array
#frame=cv2.resize(frame,(90,60),interpolation=cv2.INTER_AREA)
cv2.imshow("Image", frame)
cv2.waitKey(0)
''''

#設定camera 以及serial port
target_s=20000
cap=cv2.VideoCapture(0)
ser=serial.Serial('/dev/777',115200)

while(1):
    #等待『拍攝並判斷』的指令
    look=0
    look= ser.readline()
    #若上一個工作完成即判斷下一個工作
    if look==1 :
        r,frame=cap.read()
            if r==False:
                break
        #frame = cv2.imread("/Users/prophet/Desktop/3.png")
        #讀取為HSV的格式
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        # 設定三個顏色區間
        lower_blue = np.array([80,50,10])
        upper_blue = np.array([100,255,220])
        lower_red = np.array([150,50,50])
        upper_red = np.array([180,255,220])
        lower_green = np.array([43,60,50])
        upper_green = np.array([85,255,220])

        # 創建三種顏色的mask
        maskb = cv2.inRange(hsv, lower_blue, upper_blue)
        maskr = cv2.inRange(hsv, lower_red, upper_red)
        maskg = cv2.inRange(hsv, lower_green, upper_green)


        # 將影像都通過三個mask
        resb = cv2.bitwise_and(frame,frame, mask= maskb)
        resr = cv2.bitwise_and(frame,frame, mask= maskr)
        resg = cv2.bitwise_and(frame,frame, mask= maskg)
''''
        cv2.imshow("Image", resb)
        cv2.waitKey(0)
        cv2.imshow("Image", resr)
        cv2.waitKey(0)
        cv2.imshow("Image", resg)
        cv2.waitKey(0)
''''
        #將顏色轉為BGR並再轉為Gray準備做邊緣偵測
        imb = cv2.cvtColor(resb,cv2.COLOR_HSV2BGR)
        #imb+=50
        #cv2.imshow("Image", imb)
        #cv2.waitKey(0)
        imgrayb = cv2.cvtColor(imb,cv2.COLOR_BGR2GRAY)
        imr = cv2.cvtColor(resr,cv2.COLOR_HSV2BGR)
        imgrayr = cv2.cvtColor(imr,cv2.COLOR_BGR2GRAY)
        img = cv2.cvtColor(resg,cv2.COLOR_HSV2BGR)
        imgrayg = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
        
        #將圖片經過高斯模糊
        imgrayb=cv2.GaussianBlur(imgrayb,(11,11),0)
        imgrayg=cv2.GaussianBlur(imgrayg,(11,11),0)
        imgrayr=cv2.GaussianBlur(imgrayr,(11,11),0)
        ''''
        cv2.imshow("Image", imgrayb)
        cv2.waitKey(0)
        cv2.imshow("Image", imgrayr)
        cv2.waitKey(0)
        cv2.imshow("Image", imgrayg)
        cv2.waitKey(0)
        '''
        #將圖形經過threshold變成二值化的圖形
        ret,threshb = cv2.threshold(imgrayb,20,255,cv2.THRESH_BINARY)
        ret,threshr = cv2.threshold(imgrayr,1,255,cv2.THRESH_BINARY)
        ret,threshg = cv2.threshold(imgrayg,5,255,cv2.THRESH_BINARY)
        ''''
        cv2.imshow("Image", threshb)
        cv2.waitKey(0)
        cv2.imshow("Image", threshr)
        cv2.waitKey(0)
        cv2.imshow("Image", threshg)
        cv2.waitKey(0)
        '''
        #將圖形做邊緣偵測
        edgedb = cv2.Canny(threshb, 0, 150)
        edgedr = cv2.Canny(threshr, 0, 100)
        edgedg = cv2.Canny(threshg, 0, 150)
        ''''
        cv2.imshow("Image", edgedb)
        cv2.waitKey(0)
        cv2.imshow("Image", edgedr)
        cv2.waitKey(0)
        cv2.imshow("Image", edgedg)
        cv2.waitKey(0)
        '''
        #將Canny找出的邊緣做連續線段的處理
        (_, contoursb, _)=cv2.findContours(edgedb, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        (_, contoursg, _)=cv2.findContours(edgedg, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        (_, contoursr, _)=cv2.findContours(edgedr, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        #初始化參數
        maxcon=0
        maccol=0
        maxshape=0
        maxconer=0
        #處理偵測出來的封閉線
        for cnt in range(len(contoursb)):
                    #將邊緣做近似
                    epsilon = 5 * cv2.arcLength(contoursb[cnt], True)
                    approx = cv2.approxPolyDP(contoursb[cnt],50, True)
                    #近似出來的圖形的角數
                    corners = len(approx)
                    #利用頂點數做分類
                    if corners == 3:
             #           print("三角形")
                        if cv2.arcLength(contoursb[cnt], True) > maxcon:
                            maxcon=cv2.arcLength(contoursb[cnt], True)
                            maxshape=1
                            maccol=1
                            maxconer=corners
                    if corners == 4:
            #            print("矩形")
                        if cv2.arcLength(contoursb[cnt], True) > maxcon:
                            maxcon=cv2.arcLength(contoursb[cnt], True)
                            maxshape=2
                            maccol=1
                    if corners > 10:
            #            print("圓形")
                        if cv2.arcLength(contoursb[cnt], True) > maxcon:
                            maxcon=cv2.arcLength(contoursb[cnt], True)
                            maxshape=4
                            maccol=1
                            maxconer=corners

        for cnt in range(len(contoursr)):
                    epsilon = 0.02 * cv2.arcLength(contoursr[cnt], True)
                    approx = cv2.approxPolyDP(contoursr[cnt], epsilon, True)
                    corners = len(approx)
                    if corners == 3:
             #           print("三角形")
                        if cv2.arcLength(contoursr[cnt], True) > maxcon:
                            maxcon=cv2.arcLength(contoursr[cnt], True)
                            maxshape=1
                            maccol=2
                            maxconer=corners
                    if corners == 4:
            #            print("矩形")
                        if cv2.arcLength(contoursr[cnt], True) > maxcon:
                            maxcon=cv2.arcLength(contoursr[cnt], True)
                            maxshape=2
                            maccol=2
                            maxconer=corners
                    if corners > 10:
           #             print("圓形")
                        if cv2.arcLength(contoursr[cnt], True) > maxcon:
                            maxcon=cv2.arcLength(contoursr[cnt], True)
                            maxshape=4
                            maccol=2
                            maxconer=corners

        for cnt in range(len(contoursg)):
                    epsilon = 0.012 * cv2.arcLength(contoursg[cnt], True)
                    approx = cv2.approxPolyDP(contoursg[cnt], epsilon, True)
                    corners = len(approx)
                    if corners == 3:
           #             print("三角形")
                        if cv2.arcLength(contoursg[cnt], True) > maxcon:
                            maxcon=cv2.arcLength(contoursg[cnt], True)
                            maxshape=1
                            maccol=3
                            maxconer=corners
                    if corners == 4:
           #             print("矩形")
                        if cv2.arcLength(contoursg[cnt], True) > maxcon:
                            maxcon=cv2.arcLength(contoursg[cnt], True)
                            maxshape=2
                            maccol=3
                            maxconer=corners
                    if corners > 10:
          #              print("圓形")
                        if cv2.arcLength(contoursg[cnt], True) > maxcon:
                            maxcon=cv2.arcLength(contoursg[cnt], True)
                            maxshape=4
                            maccol=3
                            maxconer=corners
        print(maxconer)#形狀的邊數
        print(maccol)#形狀的顏色1=藍 2=紅 3=綠
        print(maxshape)#為何種形狀 1=三角形 2=矩形 3=圓形
        ser.write(maccol)#回傳顏色給nano
        ser.write(maxshape)#回傳形狀給nano

