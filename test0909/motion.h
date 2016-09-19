#define neutral		0b00000000;
#define Walk_Before	0b00000001;
#define Walk_Behind 0b00000010;
#define Walk_left	0b00000100;
#define Walk_Right	0b00001000;

#ifndef MOTION_H_
#define MOTION_H_

void switch_pattern(void);
void motion_select(void);

#endif
