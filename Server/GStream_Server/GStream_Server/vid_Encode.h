#ifndef VID_ENCODE_H
#define VID_ENCODE_H

void vid_prep(int width, int height);
void vid_writeFrame(uint8_t* imgData);
void vid_end();

#endif