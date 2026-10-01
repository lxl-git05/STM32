#include "MPU6050.h"
#include "i2c.h"          /* CubeMX provides hi2c1 here */
#include "FreeRTOS.h"
#include "task.h"
#include <math.h>
#include <stddef.h>

/* MPU6050 registers */
#define REG_SMPLRT_DIV     0x19U
#define REG_CONFIG         0x1AU
#define REG_GYRO_CONFIG    0x1BU
#define REG_ACCEL_CONFIG   0x1CU
#define REG_ACCEL_XOUT_H   0x3BU
#define REG_PWR_MGMT_1     0x6BU
#define REG_PWR_MGMT_2     0x6CU
#define REG_WHO_AM_I       0x75U

#define DEG2RAD            0.017453292519943295f
#define RAD2DEG            57.29577951308232f
#define MPU_I2C_TIMEOUT_MS 10U

MPU6050_Raw_Data MPU_Raw_Data = {0};
ImuReal_Typedef MPU_Mahony_Real = {0};

/* Initial bias for your existing sensor. AUTO_CALIB=1 overwrites these. */
float MPU_Mahony_GyroBiasX = -4.46930218f;
float MPU_Mahony_GyroBiasY = -1.47816038f;
float MPU_Mahony_GyroBiasZ = -0.679127216f;
float MPU_Mahony_AccBiasX  =  0.0518571772f;
float MPU_Mahony_AccBiasY  = -0.00286145019f;
float MPU_Mahony_AccBiasZ  = -0.0274289846f;

static float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;
static float exInt = 0.0f, eyInt = 0.0f, ezInt = 0.0f;
static float yaw_abs = 0.0f, yaw_prev = 0.0f;
static uint8_t snapshotValid = 0U;
static TickType_t snapshotTick = 0U;

static HAL_StatusTypeDef MPU_WriteReg(uint8_t reg, uint8_t value)
{
    return HAL_I2C_Mem_Write(&hi2c1, MPU6050_I2C_ADDR, reg,
                             I2C_MEMADD_SIZE_8BIT, &value, 1U,
                             MPU_I2C_TIMEOUT_MS);
}

static HAL_StatusTypeDef MPU_ReadReg(uint8_t reg, uint8_t *value)
{
    if (value == NULL) return HAL_ERROR;
    return HAL_I2C_Mem_Read(&hi2c1, MPU6050_I2C_ADDR, reg,
                            I2C_MEMADD_SIZE_8BIT, value, 1U,
                            MPU_I2C_TIMEOUT_MS);
}

uint8_t MPU6050_GetID(void)
{
    uint8_t id = 0U;
    if (MPU_ReadReg(REG_WHO_AM_I, &id) != HAL_OK) return 0U;
    return id;
}

HAL_StatusTypeDef MPU6050_Init(void)
{
    if (HAL_I2C_IsDeviceReady(&hi2c1, MPU6050_I2C_ADDR, 2U, 10U)
        != HAL_OK) return HAL_ERROR;

    if (MPU_WriteReg(REG_PWR_MGMT_1, 0x01U) != HAL_OK)
        return HAL_ERROR;                  /* Wake; gyro X clock */
    vTaskDelay(pdMS_TO_TICKS(100U));                       /* Give sensor time to settle */

    if (MPU6050_GetID() != 0x68U) return HAL_ERROR;
    if (MPU_WriteReg(REG_PWR_MGMT_2, 0x00U) != HAL_OK) return HAL_ERROR;
    if (MPU_WriteReg(REG_CONFIG, MPU6050_DLPF_CFG) != HAL_OK) return HAL_ERROR;
    if (MPU_WriteReg(REG_SMPLRT_DIV, MPU6050_SMPLRT_DIV_VALUE)
        != HAL_OK) return HAL_ERROR;
    if (MPU_WriteReg(REG_GYRO_CONFIG, MPU6050_GYRO_CFG)
        != HAL_OK) return HAL_ERROR;
    if (MPU_WriteReg(REG_ACCEL_CONFIG, MPU6050_ACCEL_CFG)
        != HAL_OK) return HAL_ERROR;
    return HAL_OK;
}

/* Task-only recovery; I2C1 is exclusively owned by the MPU task. */
HAL_StatusTypeDef MPU6050_I2C_Recover(void)
{
    GPIO_InitTypeDef gpio = {0};
    if (HAL_I2C_DeInit(&hi2c1) != HAL_OK) return HAL_ERROR;
    HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(I2C_SDA_GPIO_Port, I2C_SDA_Pin, GPIO_PIN_SET);
    gpio.Mode = GPIO_MODE_OUTPUT_OD;
    gpio.Pull = GPIO_PULLUP;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio.Pin = I2C_SCL_Pin;
    HAL_GPIO_Init(I2C_SCL_GPIO_Port, &gpio);
    gpio.Pin = I2C_SDA_Pin;
    HAL_GPIO_Init(I2C_SDA_GPIO_Port, &gpio);

    for (uint8_t i = 0U; i < 9U; i++)
    {
        HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, GPIO_PIN_RESET);
        vTaskDelay(pdMS_TO_TICKS(1U));
        HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, GPIO_PIN_SET);
        vTaskDelay(pdMS_TO_TICKS(1U));
    }
    /* STOP: release SDA while SCL is high. */
    HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(I2C_SDA_GPIO_Port, I2C_SDA_Pin, GPIO_PIN_RESET);
    vTaskDelay(pdMS_TO_TICKS(1U));
    HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, GPIO_PIN_SET);
    vTaskDelay(pdMS_TO_TICKS(1U));
    HAL_GPIO_WritePin(I2C_SDA_GPIO_Port, I2C_SDA_Pin, GPIO_PIN_SET);
    vTaskDelay(pdMS_TO_TICKS(1U));
    __HAL_RCC_I2C1_FORCE_RESET();
    __HAL_RCC_I2C1_RELEASE_RESET();
    return HAL_I2C_Init(&hi2c1);
}

/* Burst read keeps all 6 sensor axes from the same register sample. */
HAL_StatusTypeDef MPU6050_GetData(int16_t *ax, int16_t *ay, int16_t *az,
                                  int16_t *gx, int16_t *gy, int16_t *gz)
{
    uint8_t buf[14] = {0};
    if (ax == NULL || ay == NULL || az == NULL ||
        gx == NULL || gy == NULL || gz == NULL) return HAL_ERROR;

    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(
        &hi2c1, MPU6050_I2C_ADDR, REG_ACCEL_XOUT_H,
        I2C_MEMADD_SIZE_8BIT, buf, sizeof(buf), MPU_I2C_TIMEOUT_MS);
    if (status != HAL_OK) return status;  /* Never use an invalid buffer. */

    *ax = (int16_t)((((uint16_t)buf[0])  << 8) | buf[1]);
    *ay = (int16_t)((((uint16_t)buf[2])  << 8) | buf[3]);
    *az = (int16_t)((((uint16_t)buf[4])  << 8) | buf[5]);
    *gx = (int16_t)((((uint16_t)buf[8])  << 8) | buf[9]);
    *gy = (int16_t)((((uint16_t)buf[10]) << 8) | buf[11]);
    *gz = (int16_t)((((uint16_t)buf[12]) << 8) | buf[13]);
    return HAL_OK;
}

HAL_StatusTypeDef MPU6050_Update_Data(void)
{
    int16_t ax, ay, az, gx, gy, gz;
    HAL_StatusTypeDef status = MPU6050_GetData(&ax, &ay, &az,
                                                &gx, &gy, &gz);
    if (status != HAL_OK) return status;

    MPU6050_Raw_Data data;
    data.AX = (float)ax / MPU6050_ACCEL_SENS;
    data.AY = (float)ay / MPU6050_ACCEL_SENS;
    data.AZ = (float)az / MPU6050_ACCEL_SENS;
    data.GX = (float)gx / MPU6050_GYRO_SENS;
    data.GY = (float)gy / MPU6050_GYRO_SENS;
    data.GZ = (float)gz / MPU6050_GYRO_SENS;
    MPU_Raw_Data = data;                  /* Only the MPU task should write. */
    return HAL_OK;
}

static void MPU_ResetFusion(void)
{
    q0 = 1.0f; q1 = 0.0f; q2 = 0.0f; q3 = 0.0f;
    exInt = 0.0f; eyInt = 0.0f; ezInt = 0.0f;

    ImuReal_Typedef output = {0};
    output.AccZ = 1.0f;
    taskENTER_CRITICAL();
    yaw_abs = 0.0f;
    yaw_prev = 0.0f;
    MPU_Mahony_Real = output;
    snapshotValid = 0U;
    taskEXIT_CRITICAL();
}

HAL_StatusTypeDef MPU6050_Mahony_Calibrate(uint16_t samples)
{
    if (samples < 100U || samples > 2000U)
        samples = MPU6050_CALIB_SAMPLES;

    /* Level and keep stationary: expected accelerometer = [0,0,1] g. */
    float sumAx = 0.0f, sumAy = 0.0f, sumAz = 0.0f;
    float sumGx = 0.0f, sumGy = 0.0f, sumGz = 0.0f;
    for (uint16_t i = 0U; i < samples; ++i)
    {
        HAL_StatusTypeDef status = MPU6050_Update_Data();
        if (status != HAL_OK) return status;
        sumAx += MPU_Raw_Data.AX;
        sumAy += MPU_Raw_Data.AY;
        sumAz += MPU_Raw_Data.AZ;
        sumGx += MPU_Raw_Data.GX;
        sumGy += MPU_Raw_Data.GY;
        sumGz += MPU_Raw_Data.GZ;
        if (i + 1U < samples) vTaskDelay(pdMS_TO_TICKS(10U));
    }

    float invN = 1.0f / (float)samples;
    MPU_Mahony_GyroBiasX = sumGx * invN;
    MPU_Mahony_GyroBiasY = sumGy * invN;
    MPU_Mahony_GyroBiasZ = sumGz * invN;
    MPU_Mahony_AccBiasX  = sumAx * invN;
    MPU_Mahony_AccBiasY  = sumAy * invN;
    MPU_Mahony_AccBiasZ  = sumAz * invN - 1.0f;
    MPU_ResetFusion();
    return HAL_OK;
}

HAL_StatusTypeDef MPU6050_Mahony_Init(uint8_t doCalib)
{
    HAL_StatusTypeDef status = MPU6050_Init();
    if (status != HAL_OK) return status;
    if (doCalib != 0U)
        return MPU6050_Mahony_Calibrate(MPU6050_CALIB_SAMPLES);
    MPU_ResetFusion();                  /* Keep EEPROM/default biases. */
    return HAL_OK;
}

static HAL_StatusTypeDef MPU_Mahony_Update(float dt)
{
    /* Uncorrected gyroscope (deg/s), useful for the motor D term. */
    const float gxDeg = MPU_Raw_Data.GX - MPU_Mahony_GyroBiasX;
    const float gyDeg = MPU_Raw_Data.GY - MPU_Mahony_GyroBiasY;
    const float gzDeg = MPU_Raw_Data.GZ - MPU_Mahony_GyroBiasZ;
    float gx = gxDeg * DEG2RAD;
    float gy = gyDeg * DEG2RAD;
    float gz = gzDeg * DEG2RAD;

    float ax = MPU_Raw_Data.AX - MPU_Mahony_AccBiasX;
    float ay = MPU_Raw_Data.AY - MPU_Mahony_AccBiasY;
    float az = MPU_Raw_Data.AZ - MPU_Mahony_AccBiasZ;
    float accNormSq = ax * ax + ay * ay + az * az;

    if (accNormSq > 1.0e-12f)
    {
        float invNorm = 1.0f / sqrtf(accNormSq);
        ax *= invNorm; ay *= invNorm; az *= invNorm;

        /* Reject gravity feedback during significant linear acceleration. */
        if (accNormSq > (0.85f * 0.85f) &&
            accNormSq < (1.15f * 1.15f))
        {
            float vx = 2.0f * (q1*q3 - q0*q2);
            float vy = 2.0f * (q0*q1 + q2*q3);
            float vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;
            float ex = ay*vz - az*vy;
            float ey = az*vx - ax*vz;
            float ez = ax*vy - ay*vx;

            if (MPU6050_MAHONY_KI > 0.0f)
            {
                exInt += MPU6050_MAHONY_KI * ex * dt;
                eyInt += MPU6050_MAHONY_KI * ey * dt;
                ezInt += MPU6050_MAHONY_KI * ez * dt;
                /* Simple integral anti-windup (rad/s). */
                if (exInt > 0.05f) exInt = 0.05f;
                if (exInt < -0.05f) exInt = -0.05f;
                if (eyInt > 0.05f) eyInt = 0.05f;
                if (eyInt < -0.05f) eyInt = -0.05f;
                if (ezInt > 0.05f) ezInt = 0.05f;
                if (ezInt < -0.05f) ezInt = -0.05f;
            }
            else { exInt = 0.0f; eyInt = 0.0f; ezInt = 0.0f; }

            gx += MPU6050_MAHONY_KP * ex + exInt;
            gy += MPU6050_MAHONY_KP * ey + eyInt;
            gz += MPU6050_MAHONY_KP * ez + ezInt;
        }
    }
    else { ax = 0.0f; ay = 0.0f; az = 0.0f; }

    /* Quaternion derivative: all four components use the SAME old q. */
    float a = q0, b = q1, c = q2, d = q3;
    float halfT = 0.5f * dt;
    q0 = a + (-b*gx - c*gy - d*gz) * halfT;
    q1 = b + ( a*gx + c*gz - d*gy) * halfT;
    q2 = c + ( a*gy - b*gz + d*gx) * halfT;
    q3 = d + ( a*gz + b*gy - c*gx) * halfT;

    float qNormSq = q0*q0 + q1*q1 + q2*q2 + q3*q3;
    if (!(qNormSq > 1.0e-12f)) return HAL_ERROR;
    float invQ = 1.0f / sqrtf(qNormSq);
    q0 *= invQ; q1 *= invQ; q2 *= invQ; q3 *= invQ;

    ImuReal_Typedef output;
    output.roll = atan2f(2.0f * (q0*q1 + q2*q3),
                         1.0f - 2.0f * (q1*q1 + q2*q2)) * RAD2DEG;
    float pitchSin = 2.0f * (q0*q2 - q3*q1);
    if (pitchSin > 1.0f) pitchSin = 1.0f;
    if (pitchSin < -1.0f) pitchSin = -1.0f;
    output.pitch = asinf(pitchSin) * RAD2DEG;
    output.yaw = atan2f(2.0f * (q0*q3 + q1*q2),
                        1.0f - 2.0f * (q2*q2 + q3*q3)) * RAD2DEG;
    output.AccX = ax; output.AccY = ay; output.AccZ = az;
    output.GyroX = gxDeg; output.GyroY = gyDeg; output.GyroZ = gzDeg;

    /* Publish one internally consistent snapshot to other tasks. */
    taskENTER_CRITICAL();
    float delta = output.yaw - yaw_prev;
    if (delta > 180.0f) delta -= 360.0f;
    if (delta < -180.0f) delta += 360.0f;
    yaw_abs -= delta;                     /* Matches your clockwise convention. */
    yaw_prev = output.yaw;
    output.yaw_abs = yaw_abs;
    MPU_Mahony_Real = output;
    snapshotTick = xTaskGetTickCount();
    snapshotValid = 1U;
    taskEXIT_CRITICAL();
    return HAL_OK;
}

HAL_StatusTypeDef MPU6050_Mahony_Update_Tick(float dt_s)
{
    if (!(dt_s >= 0.001f && dt_s <= 0.100f)) return HAL_ERROR;
    HAL_StatusTypeDef status = MPU6050_Update_Data();
    if (status != HAL_OK) return status;
    return MPU_Mahony_Update(dt_s);
}

HAL_StatusTypeDef MPU6050_GetSnapshot(ImuReal_Typedef *out)
{
    if (out == NULL) return HAL_ERROR;
    taskENTER_CRITICAL();
    if (snapshotValid == 0U ||
        xTaskGetTickCount() - snapshotTick > pdMS_TO_TICKS(100U))
    {
        taskEXIT_CRITICAL();
        return HAL_ERROR;
    }
    *out = MPU_Mahony_Real;
    taskEXIT_CRITICAL();
    return HAL_OK;
}

float MPU_Yaw_Abs_Get(void)
{
    float result;
    taskENTER_CRITICAL();
    result = yaw_abs;
    taskEXIT_CRITICAL();
    return result;
}

void MPU_Yaw_Abs_Reset(void)
{
    taskENTER_CRITICAL();
    yaw_abs = 0.0f;
    yaw_prev = MPU_Mahony_Real.yaw;
    MPU_Mahony_Real.yaw_abs = 0.0f;
    taskEXIT_CRITICAL();
}
