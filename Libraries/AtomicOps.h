#ifndef ATOMICOPS_H
#define ATOMICOPS_H

#include <stdint.h>

#pragma diag_suppress 3731-D

inline uint32_t AtomicFetchAndAdd(volatile uint32_t* a_valPtr, uint32_t a_summand) {
	uint32_t oldVal, newVal;
	
	do {
		oldVal = __ldrex(a_valPtr);
		newVal = oldVal + a_summand;
		} while(__strex(newVal, a_valPtr));
	
	return oldVal;
	}
	
inline uint32_t AtomicAddAndFetch(volatile uint32_t* a_valPtr, uint32_t a_summand) {
	uint32_t oldVal, newVal;
	
	do {
		oldVal = __ldrex(a_valPtr);
		newVal = oldVal + a_summand;
		} while(__strex(newVal, a_valPtr));
	
	return newVal;
	}

inline bool CompareExchange(volatile uint32_t * a_valPtr, uint32_t a_oldVal, uint32_t a_newVal) {
	// exclusively read current var value and comare with the old one 
	if(__ldrex(a_valPtr) == a_oldVal)
		// if the value haven't changed since we've known it
		// then try to exclusively write a new value and 
		// return if we were successful
		return __strex(a_newVal, a_valPtr) == 0;
		
	// somebody have chagned value before us
	__clrex();
	
	return false;
	}


#endif // ATOMICOPS_H
