/*
 * Copyright (C) 2018 Arm Limited or its affiliates. All rights reserved.
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

/*
 * Description: End-to-end example code for running keyword spotting on 
 * STM32F746NG development kit (DISCO_F746NG in mbed-cli). The example is 
 * derived from https://os.mbed.com/teams/ST/code/DISCO-F746NG_AUDIO_demo
 */

#include "mbs_esp32.h"
#include "plot_utils.h"

MBS_ESP32 *MBS;
//Timer T;

char lcd_output_string[64];
char output_class[8][9] = {"Silence", "Unknown","SW","Blast","BlastEcho","MB",
                            "MBEcho","Shot"};
// Tune the following three parameters to improve the detection accuracy
//  and reduce false positives
// Longer averaging window and higher threshold reduce false positives
//  but increase detection latency and reduce true positive detections.

// (recording_win*frame_shift) is the actual recording window size
int recording_win = 3; 
// Averaging window for smoothing out the output predictions
int averaging_window_len = 3;  
int detection_threshold = 90;  //in percent

void run_mbs();

int main()
{
  //pc.baud(9600);
  MBS = new MBS_ESP32(recording_win,averaging_window_len);
  init_plot();
  MBS->start_mbs();

  //T.start();

  while (1) {
  /* A dummy loop to wait for the interrupts. Feature extraction and
     neural network inference are done in the interrupt service routine. */
  //  __WFI();
  }
}


/*
 * The audio recording works with two ping-pong buffers.
 * The data for each window will be tranfered by the DMA, which sends
 * sends an interrupt after the transfer is completed.
 */

// Manages the DMA Transfer complete interrupt.
void BSP_AUDIO_IN_TransferComplete_CallBack(void)
{
  xtensa_copy_q7((q7_t *)MBS->audio_buffer_in + MBS->audio_block_size*4, (q7_t *)MBS->audio_buffer_out + MBS->audio_block_size*4, MBS->audio_block_size*4);
  if(MBS->frame_len != MBS->frame_shift) {
    //copy the last (frame_len - frame_shift) audio data to the start
    xtensa_copy_q7((q7_t *)(MBS->audio_buffer)+2*(MBS->audio_buffer_size-(MBS->frame_len-MBS->frame_shift)), (q7_t *)MBS->audio_buffer, 2*(MBS->frame_len-MBS->frame_shift));
  }
  // copy the new recording data 
  for (int i=0;i<MBS->audio_block_size;i++) {
    MBS->audio_buffer[MBS->frame_len-MBS->frame_shift+i] = MBS->audio_buffer_in[2*MBS->audio_block_size+i*2];
  }
  run_mbs();
  return;
}

// Manages the DMA Half Transfer complete interrupt.
void BSP_AUDIO_IN_HalfTransfer_CallBack(void)
{
  xtensa_copy_q7((q7_t *)MBS->audio_buffer_in, (q7_t *)MBS->audio_buffer_out, MBS->audio_block_size*4);
  if(MBS->frame_len!=MBS->frame_shift) {
    //copy the last (frame_len - frame_shift) audio data to the start
    xtensa_copy_q7((q7_t *)(MBS->audio_buffer)+2*(MBS->audio_buffer_size-(MBS->frame_len-MBS->frame_shift)), (q7_t *)MBS->audio_buffer, 2*(MBS->frame_len-MBS->frame_shift));
  }
  // copy the new recording data 
  for (int i=0;i<MBS->audio_block_size;i++) {
    MBS->audio_buffer[MBS->frame_len-MBS->frame_shift+i] = MBS->audio_buffer_in[i*2];
  }
  run_mbs();
  return;
}

void run_mbs()
{
  MBS->extract_features();    //extract mfcc features
  MBS->classify();	      //classify using dnn
  MBS->average_predictions();
  plot_mfcc();
  plot_waveform();
  int max_ind = MBS->get_top_class(MBS->averaged_output);
  if(MBS->averaged_output[max_ind]>detection_threshold*128/100)
    sprintf(lcd_output_string,"%d%% %s",((int)MBS->averaged_output[max_ind]*100/128),output_class[max_ind]);
  lcd.ClearStringLine(8);
  lcd.DisplayStringAt(0, LINE(8), (uint8_t *) lcd_output_string, CENTER_MODE);
}

