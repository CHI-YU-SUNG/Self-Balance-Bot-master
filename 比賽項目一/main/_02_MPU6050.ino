// Goal: Get psi

Kalman kalman; // Create the Kalman instances

/* IMU Data */
double accX, accY, accZ;
double gyro;

double kalAngle; // Calculated angle using a Kalman filter

uint32_t timer;
uint8_t i2cData[14]; // Buffer for I2C data

MPU6050 accelgyro;

void SetupMPU6050(){
    Wire.begin();
    #if ARDUINO >= 157
        Wire.setClock(400000UL); // Set I2C frequency to 400kHz
    #else
        TWBR = ((F_CPU / 400000UL) - 16) / 2; // Set I2C frequency to 400kHz
    #endif

    i2cData[0] = 7; // Set the sample rate to 1000Hz - 8kHz/(7+1) = 1000Hz
    i2cData[1] = 0x00; // Disable FSYNC and set 260 Hz Acc filtering, 256 Hz Gyro filtering, 8 KHz sampling
    i2cData[2] = 0x00; // Set Gyro Full Scale Range to ±250deg/s
    i2cData[3] = 0x00; // Set Accelerometer Full Scale Range to ±2g
    while (i2cWrite(0x19, i2cData, 4, false)); // Write to all four registers at once
    while (i2cWrite(0x6B, 0x01, true)); // PLL with X axis gyroscope reference and disable sleep mode

    while (i2cRead(0x75, i2cData, 1));
    if (i2cData[0] != 0x68) { // Read "WHO_AM_I" register
        Serial.print(F("Error reading sensor"));
        while (1);
    }
/*
    // calibration: offset
    accelgyro.setXAccelOffset(844);
    accelgyro.setYAccelOffset(511);
    accelgyro.setZAccelOffset(1829);
    accelgyro.setXGyroOffset(16);
    accelgyro.setYGyroOffset(-24);
    accelgyro.setZGyroOffset(5);
    
    delay(100); // Wait for sensor to stabilize
*/
    /* Set kalman and gyro starting angle */
    while (i2cRead(0x3B, i2cData, 6));
    accX = (int16_t)((i2cData[0] << 8) | i2cData[1]);
    accY = (int16_t)((i2cData[2] << 8) | i2cData[3]);
    accZ = (int16_t)((i2cData[4] << 8) | i2cData[5]);

    // Source: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf eq. 25 and eq. 26
    // atan2 outputs the value of -π to π (radians) - see http://en.wikipedia.org/wiki/Atan2
    // It is then converted from radians to degrees
    #ifdef RESTRICT_PITCH // Eq. 25 and 26
    double roll  = atan2(accY, accZ) * RAD_TO_DEG;
    double pitch = atan(-accX / sqrt(accY * accY + accZ * accZ)) * RAD_TO_DEG;
    #else // Eq. 28 and 29
    double roll  = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
    #endif

    kalman.setAngle(roll); // Set starting angle

    timer = micros();
}

double GetPsi(){
    /* Update all the values */
    while (i2cRead(0x3B, i2cData, 14));
    accX = (int16_t)((i2cData[0] << 8) | i2cData[1]);
    accY = (int16_t)((i2cData[2] << 8) | i2cData[3]);
    accZ = (int16_t)((i2cData[4] << 8) | i2cData[5]);
    gyro = (int16_t)((i2cData[8] << 8) | i2cData[9]);

    double dt = (double)(micros() - timer) / 1000000; // Calculate delta time
    timer = micros();

    #ifdef RESTRICT_PITCH // Eq. 25 and 26
    double roll  = atan2(accY, accZ) * RAD_TO_DEG;
    #else // Eq. 28 and 29
    double roll  = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
    #endif

    double gyroXrate = gyro / 131.0; // Convert to deg/s
    kalAngle = kalman.getAngle(roll, gyroXrate, dt); // Calculate the angle using a Kalman filter

    return kalAngle;
}
