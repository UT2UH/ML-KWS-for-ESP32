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
 * Title:        xtensa_elementwise_mul_s8
 * Description:  Element wise multiplication
 *
 * $Date:        August 2019
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

/**
   * @brief s8 element wise multiplication of two vectors
   *
   * @note   Refer header file for details.
   *
   */

xtensa_status
xtensa_elementwise_mul_s8(const int8_t *input_1_vect,
                       const int8_t *input_2_vect,
                       const int32_t input_1_offset,
                       const int32_t input_2_offset,
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
  int32_t mul_res;

  loop_count = block_size;

  while (loop_count > 0U)
  {
    /* C = A * B */

    input_1 = *input_1_vect++ + input_1_offset;
    input_2 = *input_2_vect++ + input_2_offset;

    mul_res = input_1 * input_2;
    mul_res = xtensa_nn_divide_by_power_of_two(xtensa_nn_sat_doubling_high_mult(mul_res, out_mult), -out_shift) + out_offset;

    mul_res = MAX(mul_res, out_activation_min);
    mul_res = MIN(mul_res, out_activation_max);

    *output++ = (q7_t)mul_res;

    /* Decrement loop counter */
    loop_count--;
  }

  return (XTENSA_MATH_SUCCESS);
}

/**
 * @} end of BasicMath group
 */
