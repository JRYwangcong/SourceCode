////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2010 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
#define _MAPP_EASING_C
///////////////////////////////////////////////////////////////////////////////////////////////////
// Description: Easing functions
///////////////////////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// Include Files
//------------------------------------------------------------------------------
#include "MApp_ZUI_APIEasing.h"
#define PI  (3.14159265)
#define abs(x)                    ((x>0)?x:(-x))

S32 MApp_ZUI_API_EaseLinear(S32 t, S32 b, S32 c, S32 d)
{
    return (S32)(c*t/d + b);
}

S32 MApp_ZUI_API_EaseInQuad(S32 t, S32 b, S32 c, S32 d)
{
    S32 tt = t;
    return (S32)(c*(tt/=d)*tt + b);
}

S32 MApp_ZUI_API_EaseOutQuad(S32 t, S32 b, S32 c, S32 d)
{
    S32 tt = t;
    return (S32)(-c *(tt/=d)*(tt-2) + b);
}

S32 MApp_ZUI_API_EaseInOutQuad(S32 t, S32 b, S32 c, S32 d)
{
    S32 tt = t;
    if ((tt/=d/2) < 1)
        return (S32)(c/2*tt*tt + b);
    return (S32)(-c/2 * ((--tt)*(tt-2) - 1) + b);
}

S32 MApp_ZUI_API_EaseOutInQuad(S32 t, S32 b, S32 c, S32 d)
{
    if (t < d/2)
        return MApp_ZUI_API_EaseOutQuad (t*2, b, c/2, d);
    return MApp_ZUI_API_EaseInQuad((t*2)-d, b+c/2, c/2, d);
}

S32 MApp_ZUI_API_EaseInCubic(S32 t, S32 b, S32 c, S32 d)
{
    S32 tt = t;
    return (S32)(c*(tt/=d)*tt*tt + b);
}

S32 MApp_ZUI_API_EaseOutCubic(S32 t, S32 b, S32 c, S32 d)
{
    S32 tt = t;
    return (S32)(c*((tt=tt/d-1)*tt*tt + 1) + b);
}

S32 MApp_ZUI_API_EaseInOutCubic(S32 t, S32 b, S32 c, S32 d)
{
    S32 tt = t;
    if ((tt/=d/2) < 1)
        return (S32)(c/2*tt*tt*tt + b);
    return (S32)(c/2*((tt-=2)*tt*tt + 2) + b);
}

S32 MApp_ZUI_API_EaseOutInCubic(S32 t, S32 b, S32 c, S32 d)
{
    if (t < d/2)
        return MApp_ZUI_API_EaseOutCubic (t*2, b, c/2, d);
    return MApp_ZUI_API_EaseInCubic((t*2)-d, b+c/2, c/2, d);
}

S32 MApp_ZUI_API_EaseInQuart(S32 t, S32 b, S32 c, S32 d)
{
    S32 tt = t;
    return (S32)(c*(tt/=d)*tt*tt*tt + b);
}

S32 MApp_ZUI_API_EaseOutQuart(S32 t, S32 b, S32 c, S32 d)
{
    S32 tt = t;
    return (S32)(-c * ((tt=tt/d-1)*tt*tt*tt - 1) + b);
}

S32 MApp_ZUI_API_EaseInOutQuart(S32 t, S32 b, S32 c, S32 d)
{
    S32 tt = t;
    if ((tt/=d/2) < 1)
        return (S32)(c/2*tt*tt*tt*tt + b);
    return (S32)(-c/2 * ((tt-=2)*tt*tt*tt - 2) + b);
}

S32 MApp_ZUI_API_EaseOutInQuart(S32 t, S32 b, S32 c, S32 d)
{
    if (t < d/2)
        return MApp_ZUI_API_EaseOutQuart (t*2, b, c/2, d);
    return MApp_ZUI_API_EaseInQuart((t*2)-d, b+c/2, c/2, d);
}

S32 MApp_ZUI_API_EaseInQuint(S32 t, S32 b, S32 c, S32 d)
{
    S32 tt = t;
    return (S32)(c*(tt/=d)*tt*tt*tt*tt + b);
}

S32 MApp_ZUI_API_EaseOutQuint(S32 t, S32 b, S32 c, S32 d)
{
    S32 tt = t;
    return (S32)(c*((tt=tt/d-1)*tt*tt*tt*tt + 1) + b);
}

S32 MApp_ZUI_API_EaseInOutQuint(S32 t, S32 b, S32 c, S32 d)
{
    S32 tt = t;
    if ((tt/=d/2) < 1)
        return (S32)(c/2*tt*tt*tt*tt*tt + b);
    return (S32)(c/2*((tt-=2)*tt*tt*tt*tt + 2) + b);
}

S32 MApp_ZUI_API_EaseOutInQuint(S32 t, S32 b, S32 c, S32 d)
{
    if (t < d/2)
        return MApp_ZUI_API_EaseOutQuint (t*2, b, c/2, d);
    return MApp_ZUI_API_EaseInQuint((t*2)-d, b+c/2, c/2, d);
}

S32 MApp_ZUI_API_EaseInSine(S32 t, S32 b, S32 c, S32 d)
{
    return (S32)(-c * cos(t/d * (PI/2)) + c + b);
}

S32 MApp_ZUI_API_EaseOutSine(S32 t, S32 b, S32 c, S32 d)
{
    return (S32)(c * sin(t/d * (PI/2)) + b);
}

S32 MApp_ZUI_API_EaseInOutSine(S32 t, S32 b, S32 c, S32 d)
{
    return (S32)(-c/2 * (cos(PI*t/d) - 1) + b);
}

S32 MApp_ZUI_API_EaseOutInSine(S32 t, S32 b, S32 c, S32 d)
{
    if (t < d/2)
        return MApp_ZUI_API_EaseOutSine (t*2, b, c/2, d);
    return MApp_ZUI_API_EaseInSine((t*2)-d, b+c/2, c/2, d);
}

S32 MApp_ZUI_API_EaseInExpo(S32 t, S32 b, S32 c, S32 d)
{
    return (t==0) ? b : (S32)(c * pow(2, 10 * (t/d - 1)) + b - c * 0.001);
}

S32 MApp_ZUI_API_EaseOutExpo(S32 t, S32 b, S32 c, S32 d)
{
    return (t==d) ? b+c : (S32)(c * 1.001 * (-pow(2, -10 * t/d) + 1) + b);
}

S32 MApp_ZUI_API_EaseInOutExpo(S32 t, S32 b, S32 c, S32 d)
{
    if (t==0) return b;
    if (t==d) return b+c;
    S32 tt = t;
    if ((tt/=d/2) < 1)
        return (S32)(c/2 * pow(2, 10 * (tt - 1)) + b - c * 0.0005);
    return (S32)(c/2 * 1.0005 * (-pow(2, -10 * --tt) + 2) + b);
}

S32 MApp_ZUI_API_EaseOutInExpo(S32 t, S32 b, S32 c, S32 d)
{
    if (t < d/2)
        return MApp_ZUI_API_EaseOutExpo (t*2, b, c/2, d);
    return MApp_ZUI_API_EaseInExpo((t*2)-d, b+c/2, c/2, d);
}

S32 MApp_ZUI_API_EaseInCirc(S32 t, S32 b, S32 c, S32 d)
{
    S32 tt = t;
    return (S32)(-c * (sqrt(1 - (tt/=d)*tt) - 1) + b);
}

S32 MApp_ZUI_API_EaseOutCirc(S32 t, S32 b, S32 c, S32 d)
{
    S32 tt = t;
    return (S32)(c * sqrt(1 - (tt=tt/d-1)*tt) + b);
}

S32 MApp_ZUI_API_EaseInOutCirc(S32 t, S32 b, S32 c, S32 d)
{
    S32 tt = t;
    if ((tt/=d/2) < 1)
        return (S32)(-c/2 * (sqrt(1 - tt*tt) - 1) + b);
    return (S32)(c/2 * (sqrt(1 - (tt-=2)*tt) + 1) + b);
}

S32 MApp_ZUI_API_EaseOutInCirc(S32 t, S32 b, S32 c, S32 d)
{
    if (t < d/2)
        return MApp_ZUI_API_EaseOutCirc (t*2, b, c/2, d);
    return MApp_ZUI_API_EaseInCirc((t*2)-d, b+c/2, c/2, d);
}

S32 MApp_ZUI_API_EaseInElastic(S32 t, S32 b, S32 c, S32 d)
{
    if (t==0) return b;
    S32 tt = t;
    tt /= d;
    if (abs(tt-1.0) < 1e-9) return b+c;
    S32 p, s;
    S32 a;

    p = d*0.3, a = 0;

    if (a == 0 || a < abs(c))
    {
        a = c;
        s = p/4;
    }
    else
    {
        s = p/(2*PI) * asin (c/a);
    }
    return (S32)(-(a*pow(2,10*(tt-=1)) * sin( (tt*d-s)*(2*PI)/p )) + b);
}

S32 MApp_ZUI_API_EaseOutElastic(S32 t, S32 b, S32 c, S32 d)
{
    if (t==0) return b;
    S32 tt = t;
    tt /= d;
    if (abs(tt-1.0) < 1e-9) return b+c;
    S32 p, s;
    S32 a;
    p = d*0.3, a = 0;
    if (a == 0 || a < abs(c))
    {
        a = c;
        s = p/4;
    }
    else
    {
        s = p/(2*PI) * asin (c/a);
    }
    return (S32)(a*pow(2,-10*tt) * sin( (tt*d-s)*(2*PI)/p ) + c + b);
}


S32 MApp_ZUI_API_EaseInOutElastic(S32 t, S32 b, S32 c, S32 d)
{
    if (t==0) return b;
    S32 tt = t;
    tt /= d/2;
    if (abs(tt-2.0) < 1e-9) return b+c;
    S32 p, s;
    S32 a;
    p = d*(0.3*1.5), a = 0;
    if (a == 0 || a < abs(c))
    {
        a = c;
        s = p/4;
    }
    else
    {
        s = p/(2*PI) * asin (c/a);
    }
    if (tt < 1)
        return (S32)(-0.5*(a*pow(2,10*(tt-=1)) * sin( (tt*d-s)*(2*PI)/p )) + b);
    return (S32)(a*pow(2,-10*(tt-=1)) * sin( (tt*d-s)*(2*PI)/p )*0.5 + c + b);
}

S32 MApp_ZUI_API_EaseOutInElastic(S32 t, S32 b, S32 c, S32 d)
{
    if (t < d/2)
        return MApp_ZUI_API_EaseOutElastic (t*2, b, c/2, d);
    return MApp_ZUI_API_EaseInElastic((t*2)-d, b+c/2, c/2, d);
}

S32 MApp_ZUI_API_EaseInBack(S32 t, S32 b, S32 c, S32 d)
{
    S32 s = 1.70158;
    S32 tt = t;
    return (S32)(c*(tt/=d)*tt*((s+1)*tt - s) + b);
}

S32 MApp_ZUI_API_EaseOutBack(S32 t, S32 b, S32 c, S32 d)
{
    S32 s =1.70158;
    S32 tt = t;
    return (S32)(c*((tt=tt/d-1)*tt*((s+1)*tt + s) + 1) + b);
}

S32 MApp_ZUI_API_EaseInOutBack(S32 t, S32 b, S32 c, S32 d)
{
    S32 s = 1.70158 ;
    S32 tt = t;
    if ((tt/=d/2) < 1)
        return (S32)(c/2*(tt*tt*(((s*=(1.525))+1)*tt - s)) + b);
    return (S32)(c/2*((tt-=2)*tt*(((s*=(1.525))+1)*tt + s) + 2) + b);
}

S32 MApp_ZUI_API_EaseOutInBack(S32 t, S32 b, S32 c, S32 d)
{
    if (t < d/2)
        return MApp_ZUI_API_EaseOutBack (t*2, b, c/2, d);
    return MApp_ZUI_API_EaseInBack((t*2)-d, b+c/2, c/2, d);
}

S32 MApp_ZUI_API_EaseOutBounce(S32 t, S32 b, S32 c, S32 d)
{
    S32 tt = t;
    if ((tt/=d) < (1/2.75))
        return (S32)(c*(7.5625*tt*tt) + b);
    else if (tt < (2/2.75))
        return (S32)(c*(7.5625*(tt-=(1.5/2.75))*tt + .75) + b);
    else if (tt < (2.5/2.75))
        return (S32)(c*(7.5625*(tt-=(2.25/2.75))*tt + .9375) + b);
    else
        return (S32)(c*(7.5625*(tt-=(2.625/2.75))*tt + .984375) + b);
}

S32 MApp_ZUI_API_EaseInBounce(S32 t, S32 b, S32 c, S32 d)
{
    return c - MApp_ZUI_API_EaseOutBounce (d-t, 0, c, d) + b;
}

S32 MApp_ZUI_API_EaseInOutBounce(S32 t, S32 b, S32 c, S32 d)
{
    if (t < d/2)
        return (S32)(MApp_ZUI_API_EaseInBounce (t*2, 0, c, d) * .5 + b);
    return (S32)(MApp_ZUI_API_EaseOutBounce (t*2-d, 0, c, d) * .5 + c*.5 + b);
}

S32 MApp_ZUI_API_EaseOutInBounce(S32 t, S32 b, S32 c, S32 d)
{
    if (t < d/2)
        return MApp_ZUI_API_EaseOutBounce (t*2, b, c/2, d);
    return MApp_ZUI_API_EaseInBounce((t*2)-d, b+c/2, c/2, d);
}

static S32 (*_MApp_ZUI_API_Easing[])(S32,S32,S32,S32) =  
{
    MApp_ZUI_API_EaseLinear,
    MApp_ZUI_API_EaseInQuad,
    MApp_ZUI_API_EaseOutQuad,
    MApp_ZUI_API_EaseInOutQuad,
    MApp_ZUI_API_EaseOutInQuad,
    MApp_ZUI_API_EaseInCubic,
    MApp_ZUI_API_EaseOutCubic,
    MApp_ZUI_API_EaseInOutCubic,
    MApp_ZUI_API_EaseOutInCubic,
    MApp_ZUI_API_EaseInQuart,
    MApp_ZUI_API_EaseOutQuart,
    MApp_ZUI_API_EaseInOutQuart,
    MApp_ZUI_API_EaseOutInQuart,
    MApp_ZUI_API_EaseInQuint,
    MApp_ZUI_API_EaseOutQuint,
    MApp_ZUI_API_EaseInOutQuint,
    MApp_ZUI_API_EaseOutInQuint,
    MApp_ZUI_API_EaseInSine,
    MApp_ZUI_API_EaseOutSine,
    MApp_ZUI_API_EaseInOutSine,
    MApp_ZUI_API_EaseOutInSine,
    MApp_ZUI_API_EaseInExpo,
    MApp_ZUI_API_EaseOutExpo,
    MApp_ZUI_API_EaseInOutExpo,
    MApp_ZUI_API_EaseOutInExpo,
    MApp_ZUI_API_EaseInCirc,
    MApp_ZUI_API_EaseOutCirc,
    MApp_ZUI_API_EaseInOutCirc,
    MApp_ZUI_API_EaseOutInCirc,
    MApp_ZUI_API_EaseInElastic,
    MApp_ZUI_API_EaseOutElastic,
    MApp_ZUI_API_EaseInOutElastic,
    MApp_ZUI_API_EaseOutInElastic,
    MApp_ZUI_API_EaseInBack,
    MApp_ZUI_API_EaseOutBack,
    MApp_ZUI_API_EaseInOutBack,
    MApp_ZUI_API_EaseOutInBack,
    MApp_ZUI_API_EaseInBounce,
    MApp_ZUI_API_EaseOutBounce,
    MApp_ZUI_API_EaseInOutBounce,
    MApp_ZUI_API_EaseOutInBounce,
};


S32 MApp_ZUI_API_Easing(E_EASING_TYPE easingType, S32 t, S32 b, S32 c, S32 d)
{
    return _MApp_ZUI_API_Easing[easingType](t,b,c,d);
}

#undef _MAPP_EASING_C

