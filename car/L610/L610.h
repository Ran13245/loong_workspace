#include "main.h"
extern char *strx,*extstrx,*Readystrx;
extern char RxBuffer[1024],Rxcouter;
extern char *strstr(const char *, const char *);
extern uint8_t Res;

void L610_init(void);
void Clear_Buffer(void);