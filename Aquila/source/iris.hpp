#ifndef IRIS_HPP_
#define IRIS_HPP_
/*
 * iris.hpp
 * Created: 2018/12/20 12:00:00
 *  Author: Emile
 */ 
#include "core.hpp"
/*
#include "initializing.hpp"
#include "ui_control.hpp"
#include "lcd_control.hpp"
#include "action.hpp"
#include "color_control.hpp"
#include "sermo_control.hpp"
#include "serial.hpp" 
#include "ping_control.hpp"
#include "motor_control.hpp"
#include <avr/io.h>
#include <util/delay.h>
*/
//#include "core.hpp" // it's my code

void make_nodes();

void black_tile();

void nachylenie();
bool nachylenie2();

void move(int num);
void move_n(node* n);

void move_toa(node* a);
void stack_dfs();
#endif
