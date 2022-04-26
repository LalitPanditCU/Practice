/*
 * test_sine.c
 *
 *  Created on: Apr 25, 2022
 *      Author: lpandit
 */
#include <stdio.h>
#include <math.h>

#include "sine.h"
#include "test_sine.h"

void test_sin()
{
  double act_sin;
  double exp_sin;
  double err;
  double sum_sq = 0;
  double max_err = 0;

  for (int i=-TWO_PI; i <= TWO_PI; i++) {
    exp_sin = sin( (double)i / TRIG_SCALE_FACTOR) * TRIG_SCALE_FACTOR;
    act_sin = fp_sin(i);

    err = act_sin - exp_sin;
    if (err < 0)
      err = -err;

    if (err > max_err)
      max_err = err;
    sum_sq += err*err;
  }

  printf("max_err=%f  sum_sq=%f\n", max_err, sum_sq);
}
