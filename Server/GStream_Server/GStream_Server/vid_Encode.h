#ifndef VID_ENCODE_H
#define VID_ENCODE_H

void vid_prep(int width, int height);
void vid_end();
void vid_writeFrame(uint8_t* imgData,size_t);
#endif