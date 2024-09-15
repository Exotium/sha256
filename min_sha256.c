#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define R(N,A)(N>>A|N<<32-A)
#define G(N,F,D)for(i=0;d=F(K[i]),l=*(int64_t*)&d,i<N;D[i++]=l>>1043-(l>>52&2047));
uint a,b,c,e,f,g,h[8],i,j,K[64],p=1,t,w[64],x[9];int64_t l;double d;char*m;main(int
n,char**o){for(n==2?:exit(1),strcpy(memset(m=malloc(g=(f=strlen(o[1]))+72&~63),0,g)
,o[1])[f]=128,*(int*)(m+g-4)=htonl(f*8);i<64;K[i++]=p)_:for(j=2,p++;j<p;)if(p%j++==
0)goto _;G(8,sqrt,h)G(64,cbrt,K)for(;memcpy(x,h,32),b<g/i;++b){for(p=0;p<16;w[p++]=
ntohl(((int*)m)[p+b*16]));for(p=7;p<55;w[p+++9]=w[p-7]+w[p+2]+(R(w[p-6],7)^R(w[p-6]
,18)^w[p-6]>>3)+(R(w[p+7],17)^R(w[p+7],19)^w[p+7]>>10));for(p=0;e=x[4],a=x[0],p<//❤
i;memmove(x+1,x,32),x[4]+=t=K[p]+w[p++]+(R(e,6)^R(e,11)^R(e,25))+((e&x[6])^(~e&x[7]
))+x[8],x[0]=t+(R(a,2)^R(a,13)^R(a,22))+((a&x[2])^(a&x[3])^(x[2]&x[3])));for(p=0;p<
8;++p)h[p]+=x[p];}for(;c<8;printf("%08x",h[c++]));}
// made with love and lack of sleep
