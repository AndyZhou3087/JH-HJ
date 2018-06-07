

#ifndef __PayInterface_H__
#define __PayInterface_H__

void startPayment( int itemId);
char * getUserID();
void payCallback(int status, int itemId);


#endif // __StartScene_H__
