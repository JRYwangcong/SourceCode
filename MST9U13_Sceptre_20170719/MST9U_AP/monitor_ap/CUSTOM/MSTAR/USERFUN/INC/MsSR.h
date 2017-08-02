/******************************************************************************
 Copyright (c) 2012 MStar Semiconductor, Inc.
 All rights reserved.

 [Module Name]: msSuperResolution.h
 [Date]:        09-Nov-2012
 [Comment]:
   SuperResolution file.
 [Reversion History]:
*******************************************************************************/

#ifndef _MSSR_H_
#define _MSSR_H_

#ifdef _MSSR_C_
#define INTERFACE
#else
#define INTERFACE   extern
#endif

INTERFACE void SetSuperResolution( void );

#endif

