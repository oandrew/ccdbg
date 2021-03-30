/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2014 Billy Millare
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * 05may2014
 */

#include "ccdbg.h"
#include <stdio.h>
#include <err.h>
#include <gpiod.h>
#include <time.h>

static struct gpiod_chip * chip;

#define RESET	25	// 0
#define DC		23	// 1
#define DD		24	// 2

static struct gpiod_line* gpio[3];
//= { gpio_reset, gpio_dc, gpio_dd };

//#define _printf(...) printf(__VA_ARGS__)
#define _printf(...) 
int ccdbgDevice_initialize(void)
{
  _printf("%s\n", __FUNCTION__);
  chip = gpiod_chip_open_by_number(0);
  if(chip == 0) {
    warn("bad chip");
    return -1;
  }

  const struct gpiod_line_request_config cfg = {
  	"ccdbg", GPIOD_LINE_REQUEST_DIRECTION_OUTPUT, 0
  }; 

  gpio[CCDBG_PIN_RESET] = gpiod_chip_get_line(chip, RESET);
  gpiod_line_request(gpio[CCDBG_PIN_RESET], &cfg, 0);
  gpio[CCDBG_PIN_DC] = gpiod_chip_get_line(chip, DC);
  gpiod_line_request(gpio[CCDBG_PIN_DC], &cfg, 0);
  gpio[CCDBG_PIN_DD] = gpiod_chip_get_line(chip, DD);
  gpiod_line_request(gpio[CCDBG_PIN_DD], &cfg, 0);

  for(int i = 0; i < 3; i++) {
    if(gpio[i] == 0) return -1;
    _printf("gpio %d ok\n", i);
  }

  return 0;

}

void ccdbgDevice_destroy(void)
{
  _printf("%s\n", __FUNCTION__);
}

void ccdbgDevice_setPinState(CCDBG_PIN pin, int high)
{
  gpiod_line_set_value(gpio[pin], high);
  _printf("%s %d set %d\n", __FUNCTION__, pin, high);
}

int ccdbgDevice_getPinState(CCDBG_PIN pin)
{
  int v =gpiod_line_get_value(gpio[pin]); 
  _printf("%s %d get %d\n", __FUNCTION__, pin, v);
	return  v;
}

void ccdbgDevice_setPinDirection(CCDBG_PIN pin, int output)
{
  //gpiod_line_release(gpio[pin]);
  if(output) {
	//gpiod_line_request_output(gpio[pin], "ccdebug", 0);
	gpiod_line_set_direction_output(gpio[pin], 0);
  } else {
	//gpiod_line_request_input(gpio[pin], "ccdebug");
	gpiod_line_set_direction_input(gpio[pin]);
  }
  //gpiod_line_update(gpio[pin]);
  _printf("%s %d set %d\n", __FUNCTION__, pin, output);
}

static volatile unsigned int  _delay_i = 0;

void ccdbgDevice_delay(void)
{
	// no operation
	//const struct timespec req = {0, 50};
	//nanosleep(&req, 0);
	for (_delay_i = 0; _delay_i < 1000; ++_delay_i) {}

}
