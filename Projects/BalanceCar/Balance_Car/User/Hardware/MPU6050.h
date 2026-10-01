#ifndef __MPU6050_H
#define __MPU6050_H

#include "main.h"       /* CubeMX HAL: stm32f1xx_hal.h / stm32f4xx_hal.h */
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* PA0/AD0 = 0: 7-bit 0x68; HAL uses left-shifted address 0xD0. */
#ifndef MPU6050_I2C_ADDR
#define MPU6050_I2C_ADDR           (0x68U << 1)
#endif

/* ±2 g, ±250 deg/s. If changing ranges, change sensitivities together. */
#define MPU6050_ACCEL_CFG          0x00U
#define MPU6050_GYRO_CFG           0x00U
#define MPU6050_ACCEL_SENS         16384.0f
#define MPU6050_GYRO_SENS          131.0f

/* Sensor output: 1 kHz / (9+1) = 100 Hz; DLPF_CFG=4 (~20 Hz gyro). */
#define MPU6050_SMPLRT_DIV_VALUE   0x09U
#define MPU6050_DLPF_CFG           0x04U

#ifndef MPU6050_CALIB_SAMPLES
#define MPU6050_CALIB_SAMPLES      200U
#endif
#ifndef MPU6050_MAHONY_KP
#define MPU6050_MAHONY_KP          2.0f
#endif
#ifndef MPU6050_MAHONY_KI
#define MPU6050_MAHONY_KI          0.0f
#endif

typedef struct
{
    float AX, AY, AZ;             /* g */
    float GX, GY, GZ;             /* deg/s, before software bias removal */
} MPU6050_Raw_Data;

typedef struct
{
    float roll, pitch, yaw;       /* deg; yaw in [-180, 180] */
    float yaw_abs;                /* deg; continuous, clockwise positive for chosen mounting */
    float AccX, AccY, AccZ;       /* bias-corrected, normalized acceleration */
    float GyroX, GyroY, GyroZ;    /* deg/s; bias-corrected */
} ImuReal_Typedef;

extern MPU6050_Raw_Data MPU_Raw_Data;
extern ImuReal_Typedef MPU_Mahony_Real;

/* Retained as writable globals for EEPROM (AT24C02) restoration. */
extern float MPU_Mahony_GyroBiasX, MPU_Mahony_GyroBiasY, MPU_Mahony_GyroBiasZ;
extern float MPU_Mahony_AccBiasX,  MPU_Mahony_AccBiasY,  MPU_Mahony_AccBiasZ;

/* Call only after CubeMX's MX_I2C1_Init(). */
HAL_StatusTypeDef MPU6050_Init(void);
HAL_StatusTypeDef MPU6050_I2C_Recover(void);
HAL_StatusTypeDef MPU6050_Update_Data(void);
HAL_StatusTypeDef MPU6050_GetData(int16_t *ax, int16_t *ay, int16_t *az,
                                  int16_t *gx, int16_t *gy, int16_t *gz);
uint8_t MPU6050_GetID(void);     /* Returns 0 if I2C read fails. */

/* Call from one task. doCalib=1 requires stationary, level, Z pointing up. */
HAL_StatusTypeDef MPU6050_Mahony_Init(uint8_t doCalib);
HAL_StatusTypeDef MPU6050_Mahony_Calibrate(uint16_t samples);
/* Read one sample and update with actual elapsed time in seconds. */
HAL_StatusTypeDef MPU6050_Mahony_Update_Tick(float dt_s);

/* Task-only snapshot; HAL_ERROR before first update or if older than 100 ms. */
HAL_StatusTypeDef MPU6050_GetSnapshot(ImuReal_Typedef *out);
float MPU_Yaw_Abs_Get(void);
void MPU_Yaw_Abs_Reset(void);

#ifdef __cplusplus
}
#endif
#endif
