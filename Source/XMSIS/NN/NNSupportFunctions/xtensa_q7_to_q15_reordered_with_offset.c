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
 * Title:        xtensa_q7_to_q15_reordered_with_offset.c
 * Description:  Converts the elements of the Q7 vector to a reordered Q15 vector with an added offset. The re-ordering
 *               is a signature of sign extension intrinsic(DSP extension).
 *
 * $Date:        August 2019
 * $Revision:    V.1.0.0
 *
 * Target Processor:  Cortex-M cores
 *
 * -------------------------------------------------------------------- */

#include "xtensa_nnsupportfunctions.h"

/**
 * @ingroup groupSupport
 */

/**
 * @addtogroup nndata_convert
 * @{
 */

/**
 * @brief Converts the elements of the Q7 vector to a reordered Q15 vector with an added offset.
 *
 * @note  Refer header file for details.
 *
 */

void xtensa_q7_to_q15_reordered_with_offset(const q7_t *src, q15_t *dst, uint32_t block_size, q7_t offset)
{
    (void)src;
    (void)dst;
    (void)block_size;
    (void)offset;
    /* Not available */
}

/**
 * @} end of nndata_convert group
 */
