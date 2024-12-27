/**
 * @file CommandData.h
 * @author Arunkumar Mourougappane (amouroug@buffalo.edu)
 * @brief Header declaring the data type for commands.
 * @version 0.1
 * @date 2024-12-26
 * 
 * Copyright (c) Arunkumar Mourougappane
 * 
 */

#ifndef COMMAND_DATA_H
#define COMMAND_DATA_H

typedef struct {
   int joypad_direction_r;
   int joypad_direction_l;
} command_data_t;

#endif // !COMMAND_DATA_H