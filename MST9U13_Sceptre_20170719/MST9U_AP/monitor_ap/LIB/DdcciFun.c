unsigned char xdata ddcciTbl1[16]=
{
100,2,3,15,
5,233,7,8,
9,90,11,12,
13,14,4,200
};
unsigned char xdata ddcciTbl2[16]=
{
19,122,21,16,
25,26,207,28,
29,158,20,22,
13,14,24,189
};
unsigned int DdcciCheck(unsigned int index)
{
 index=index&0x00FF;
 return (((*(ddcciTbl1+(index>>4)))<<8)|(*(ddcciTbl2+(index&0x0F))));
}

