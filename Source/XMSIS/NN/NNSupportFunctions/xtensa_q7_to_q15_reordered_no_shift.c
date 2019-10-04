/*
 * Copyright (C) 2010-2018 Arm Limited or its affiliates. All rights reserved.
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
 * Title:        xtensa_q7_to_q15_reordered_no_shift.c
 * Description:  Converts the elements of the Q7 vector to reordered Q15 vector without left-shift
 *
 * $Date:        17. January 2018
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
 * @brief Converts the elements of the Q7 vector to reordered Q15 vector without left-shift
 * @param[in]       *pSrc points to the Q7 input vector
 * @param[out]      *pDst points to the Q15 output vector
 * @param[in]       blockSize length of the input vector
 *
 * @details
 *
 * This function does the q7 to q15 expansion with re-ordering
 *
 * <pre>
 *                          |   A1   |   A2   |   A3   |   A4   |
 *
 *                           0      7 8     15 16    23 24    31
 * </pre>
 *
 * is converted into:
 *
 * <pre>
 *  |       A1       |       A3       |   and  |       A2       |       A4       |
 *
 *   0             15 16            31          0             15 16            31
 * </pre>
 *
 *
 * This looks strange but is natural considering how sign-extension is done at
 * assembly level.
 *
 * The expansion of other other oprand will follow the same rule so that the end
 * results are the same.
 *
 * The tail (i.e., last (N % 4) elements) will still be in original order.
 *
 */

void xtensa_q7_to_q15_reordered_no_shift(const q7_t * pSrc, q15_t * pDst, uint32_t blockSize)
{
    const q7_t *pIn = pSrc;     /* Src pointer */
    uint32_t  blkCnt;           /* loop counter */

    /* Run the below code for Cortex-M0 */

    /* Loop over blockSize number of values */
    blkCnt = blockSize;

    while (blkCnt > 0u)
    {
        /* C = (q15_t) A << 8 */
        /* convert from q7 to q15 and then store the results in the destination buffer */
        *pDst++ = (q15_t) * pIn++;

        /* Decrement the loop counter */
        blkCnt--;
    }

}

/**
 * @} end of q7_to_x group
 */
