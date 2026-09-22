#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include "bits.h"
static uint32_t state=0x20260922;
static uint32_t rnd(void){state^=state<<13;state^=state>>17;state^=state<<5;return state;}
#define CHECK(f,want) do { uint32_t got=(f), expected=(want); checks++; if(got!=expected){fprintf(stderr,"line %d: %08x != %08x\n",__LINE__,got,expected);exit(1);} }while(0)
static uint32_t fb(float f){uint32_t u;memcpy(&u,&f,4);return u;}
static uint32_t dref(uint32_t lo,uint32_t hi){uint64_t u=((uint64_t)hi<<32)|lo;double d;memcpy(&d,&u,8);if(!isfinite(d)||d>=2147483648.0||d< -2147483648.0)return 0x80000000u;return (int)d;}
int main(void){unsigned long checks=0;for(int i=0;i<1000000;i++){
uint32_t u=rnd(),w=rnd();int x,y;memcpy(&x,&u,4);memcpy(&y,&w,4);
CHECK(bitAnd(x,y),u&w);CHECK(bitXor(x,y),u^w);CHECK(samesign(x,y),(x<0&&y<0)||(x>0&&y>0)||(!x&&!y));
unsigned v=(u&INT_MAX)|1;int lg=0;for(unsigned t=v;t>>=1;)lg++;CHECK(logtwo(v),lg);
int n=i&3,m=(i>>2)&3;unsigned a=n*8,b=m*8,delta=((u>>a)^(u>>b))&255;CHECK(byteSwap(x,n,m),u^(delta<<a)^(delta<<b));
uint32_t rev=0;for(int k=0;k<32;k++)rev=(rev<<1)|((u>>k)&1);CHECK(reverse(u),rev);
CHECK(logicalShift(x,i&31),u>>(i&31));int ones=0;for(int k=31;k>=0&&((u>>k)&1);k--)ones++;CHECK(leftBitCount(x),ones);
CHECK(float_i2f(x),fb((float)x));float f;memcpy(&f,&u,4);CHECK(floatScale2(u),isnan(f)?u:fb(2.0f*f));CHECK(float64_f2i(u,w),dref(u,w));
int e=(i%601)-300;CHECK(floatPower2(e),fb(ldexpf(1.0f,e)));
}
int edges[]={INT_MIN,INT_MIN+1,-16777219,-16777217,-1,0,1,16777215,16777216,16777217,16777219,INT_MAX};
for(unsigned i=0;i<sizeof(edges)/sizeof(*edges);i++){CHECK(float_i2f(edges[i]),fb((float)edges[i]));for(int n=0;n<32;n++)CHECK(logicalShift(edges[i],n),((uint32_t)edges[i])>>n);}
for(int e=0;e<2048;e++)for(int s=0;s<2;s++){uint32_t hi=((uint32_t)s<<31)|((unsigned)e<<20);CHECK(float64_f2i(0,hi),dref(0,hi));CHECK(float64_f2i(UINT_MAX,hi|0xfffff),dref(UINT_MAX,hi|0xfffff));}
for(int k=0;k<=32;k++){uint32_t u=k==0?0:UINT_MAX<<(32-k);CHECK(leftBitCount(u),k);}
CHECK(floatPower2(INT_MIN),0);CHECK(floatPower2(INT_MAX),0x7f800000);
printf("PASS: %lu independent checks; seed 0x20260922\n",checks);return 0;}
