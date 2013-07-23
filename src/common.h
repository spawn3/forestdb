/*
 * Copyright 2013 Jung-Sang Ahn <jungsang.ahn@gmail.com>.
 * All Rights Reserved.
 */

#ifndef _JSAHN_COMMON_H
#define _JSAHN_COMMON_H

#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>


static struct timespec _ntime_gap(struct timespec a, struct timespec b) 
{
	struct timespec ret;
	if (b.tv_nsec >= a.tv_nsec) {
		ret.tv_nsec = b.tv_nsec - a.tv_nsec;
		ret.tv_sec = b.tv_sec - a.tv_sec;
	}else{
		ret.tv_nsec = 1000000000 + b.tv_nsec - a.tv_nsec;
		ret.tv_sec = b.tv_sec - a.tv_sec - 1;
	}
	return ret;
}

static struct timeval _utime_gap(struct timeval a, struct timeval b) 
{
	struct timeval ret;
	if (b.tv_usec >= a.tv_usec) {
		ret.tv_usec = b.tv_usec - a.tv_usec;
		ret.tv_sec = b.tv_sec - a.tv_sec;
	}else{
		ret.tv_usec = 1000000 + b.tv_usec - a.tv_usec;
		ret.tv_sec = b.tv_sec - a.tv_sec - 1;
	}
	return ret;
}

#ifdef __APPLE__
	#define INLINE extern inline

	#define _F64 "lld"
	#define _FSEC "ld"
	#define _FUSEC "d"

	#ifndef spin_t
	// spinlock
	#include <libkern/OSAtomic.h>
	#define spin_t OSSpinLock
	#define spin_lock(arg) OSSpinLockLock(arg)
	#define spin_unlock(arg) OSSpinLockUnlock(arg)
	#define SPIN_INITIALIZER 0
	#endif
	
#elif __linux
	#define INLINE __inline

	#define _F64 "ld"
	#define _FSEC "ld"
	#define _FUSEC "ld"

	#ifndef spin_t
	// spinlock
	#include <pthread.h>
	#define spin_t pthread_spinlock_t
	#define spin_lock(arg) pthread_spin_lock(arg)
	#define spin_unlock(arg) pthread_spin_unlock(arg)
	#define SPIN_INITIALIZER 1
	#endif
	
#else
	#define INLINE make_error
#endif

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

typedef uint64_t bid_t;
#define BLK_NOT_FOUND 0xffffffffffffffff


#define randomize() srand((unsigned)time(NULL))

// custom random: 'num' must be power of 2
/*
#define random_custom(num) randnum_tx; randnum_tx=(randnum_tx+811)&(num-1)
unsigned randnum_tx=0;
*/

#define random(num) ((rand())%(num))

//#define _BNODE_COMP
//#define _DOC_COMP

#ifdef __DEBUG
	#include <stdio.h>
	#define DBG(args...) fprintf(stderr, args)
	#define DBGCMD(command...) command
#else
	#define DBG(args...)
	#define DBGCMD(command...)
#endif

// can be faster under O3 optimization
//#define __BIT_CMP
#ifdef __BIT_CMP

#define _64_SIGN_MASK (0x8000000000000000)
#define _32_SIGN_MASK (0x0000000080000000)
#define _32_MASK (0x00000000ffffffff)
#define _64_SIGN_CHECK(a,b) ((uint64_t)((a & _64_SIGN_MASK)^(b & _64_SIGN_MASK))>>63)
#define _U64_V(ptr) ( *(uint64_t*)ptr  )
#define _CMP_U64(a, b) \
	((int32_t) ( \
	( (_64_SIGN_CHECK(a,b)-1) & (((a-b)>>32) | (((a-b)&_32_MASK)>>1) | ((a-b)&0x1) ) ) |  \
	( ((_64_SIGN_CHECK(a,b)<<31) | _64_SIGN_CHECK(a,b)) & ( ((b & _64_SIGN_MASK) >> 32) | 0x1))))
#define _CMP_U64_P(a, b) \
	((int32_t) ( \
	( (_64_SIGN_CHECK(_U64_V(a),_U64_V(b))-1) &  \
		(((_U64_V(a)-_U64_V(b))>>32) | (((_U64_V(a)-_U64_V(b))&_32_MASK)>>1) | ((_U64_V(a)-_U64_V(b))&0x1)) ) |	\
	( ((_64_SIGN_CHECK(_U64_V(a),_U64_V(b))<<31) | _64_SIGN_CHECK(_U64_V(a),_U64_V(b))) & \
		( ((_U64_V(b) & _64_SIGN_MASK) >> 32) | 0x1))))

#endif

#endif

