/*
 * Copyright (C) 2010-2019 Arm Limited or its affiliates. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* ----------------------------------------------------------------------
 * Project:      CMSIS NN Library
 * Title:        xtensa_elementwise_add_s8
 * Description:  Element wise add
 *
 * $Date:        7. August 2019
 * $Revision:    V.1.0.0
 *
 * Target Processor:  Cortex-M cores
 *
 * -------------------------------------------------------------------- */

#include "xtensa_math.h"
#include "xtensa_nnfunctions.h"
#include "xtensa_nnsupportfunctions.h"

/**
 *  @ingroup groupNN
 */

/**
 * @addtogroup BasicMath
 * @{
 */

  /*
   * s8 element wise add
   *
   * Refer header file for details.
   *
   */

#define SAT_INPUT(__INPUT, __MULT, __SHIFT)                \
  __INPUT = xtensa_nn_sat_doubling_high_mult(__INPUT, __MULT); \
  __INPUT = xtensa_nn_divide_by_power_of_two(__INPUT, -__SHIFT);

xtensa_status
xtensa_elementwise_add_s8(const int8_t *input_1_vect,
                       const int8_t *input_2_vect,
                       const int32_t input_1_offset,
                       const int32_t input_1_mult,
                       const int32_t input_1_shift,
                       const int32_t input_2_offset,
                       const int32_t input_2_mult,
                       const int32_t input_2_shift,
                       const int32_t left_shift,
                       int8_t *output,
                       const int32_t out_offset,
                       const int32_t out_mult,
                       const int32_t out_shift,
                       const int32_t out_activation_min,
                       const int32_t out_activation_max,
                       const uint32_t block_size)
{

  uint32_t loop_count;
  int32_t input_1;
  int32_t input_2;
  int32_t sum;

  loop_count = block_size;

  while (loop_count > 0U)
  {
    /* C = A + B */

    input_1 = (*input_1_vect++ + input_1_offset) << left_shift;
    input_2 = (*input_2_vect++ + input_2_offset) << left_shift;

    input_1 = xtensa_nn_sat_doubling_high_mult(input_1, input_1_mult);
    input_1 = xtensa_nn_divide_by_power_of_two(input_1, -input_1_shift);

    input_2 = xtensa_nn_sat_doubling_high_mult(input_2, input_2_mult);
    input_2 = xtensa_nn_divide_by_power_of_two(input_2, -input_2_shift);

    sum = input_1 + input_2;
    sum = xtensa_nn_requantize(sum, out_mult, out_shift) + out_offset;

    sum = MAX(sum, out_activation_min);
    sum = MIN(sum, out_activation_max);

    *output++ = (q7_t)sum;

    /* Decrement loop counter */
    loop_count--;
  }

  return (XTENSA_MATHATH_SUCCESS);
}

/**
 * @} end of BasicMath group
 */
