/*
 * mv_control.h
 *
 * Created: 2018/09/18 16:51:37
 *  Author: shun2
 */ 


#ifndef MV_CONTROL_HPP_
#define MV_CONTROL_HPP_
//#include "core.hpp"
//#include "iris.hpp"
#include <stdint.h>
// struct hako{
// 	int key;
// 	int type;
// };
// extern hako hhh;
void mv_sig(uint8_t i,bool ud);
void init_mv(void);
bool kit_chk(uint8_t num);

void mv_cap(uint8_t di,bool st);
uint8_t mv_spi_send(uint8_t val, uint8_t i);
uint8_t check_mv(uint8_t dir);

typedef enum{
    MV_LEFT,
    MV_FRONT,
    MV_RIGHT
}mv_ch_t;

#endif /* MV_CONTROL_H_ */