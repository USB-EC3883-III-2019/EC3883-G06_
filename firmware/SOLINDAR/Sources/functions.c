#include "functions.h"



//Motor de paso control
int motor_run(int* dir,int* counter,int max){
  if((*dir))
    (*counter)++;
  else
    (*counter)--;
  if((*counter)>=max)
    (*dir)=0;
  if((*counter)==0)
    (*dir)=1;
  return (*counter)%8;
}
