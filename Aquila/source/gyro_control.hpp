/*
 * gyro_control.h
 *
 * Created: 2018/08/28 11:18:56
 *  Author: shun2
 */ 


#ifndef GYRO_CONTROL_H_
#define GYRO_CONTROL_H_
#include <stdint.h>
float acc_x_mes(void);
float acc_y_mes(void);
void acc_mes(void);
void gyro_send(uint8_t reg,uint8_t dh,uint8_t dl);
float gyro_angle(void);
void gyro_mes(void);
float gyro_angle_x(void);
float gyro_angle_y(void);
void gyro_time_mes(void);
void gyro_cali(void);
void gyro_set200hz(void);
void gyro_Save(void);
void jy_recv_wd(uint8_t sd,int16_t *d);


#endif /* GYRO_CONTROL_H_ */