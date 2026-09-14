// ADOBE SYSTEMS INCORPORATED
// Copyright  1993 - 2002 Adobe Systems Incorporated
// All Rights Reserved
//
// NOTICE:  Adobe permits you to use, modify, and distribute this 
// file in accordance with the terms of the Adobe license agreement
// accompanying it.  If you have received this file from a source
// other than Adobe, then your use, modification, or distribution
// of it requires the prior written permission of Adobe.
//-------------------------------------------------------------------
//-------------------------------------------------------------------------------
//
//	File:
//		PIUNew.cpp
//
//	Description:
//		Operator new and delete overrides. Always use Photoshop to get
//		your memory.
//
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------------
#include "PhotoshopSDK.h"

#if _DEBUG
	#define USE_MEMWATCH 1
	#if __PIMac__
		#define PIDebugString(x)	DebugStr("\p"x)
	#else
		#define PIDebugString(x)	OutputDebugString(x"\n")
	#endif
#else
	#define USE_MEMWATCH 0
#endif

#if USE_MEMWATCH

const int kMemWatchSize = 500;

/** A _VERY_CRUDE_ memory checker. I only include it because it helped
    track down _one_ bug in Listener. 
    I am not saying that if you turn this on and don't have any problems
    your code is leak free. The bug may be in the MemWatch class
    Once you get one error message MemWatch is probably screwed up so
    I woudn't bet on the remaining errors being correct. 
    Remember the _VERY_CRUDE_ comment!
*/
class MemWatch {
	char * m[kMemWatchSize];
	int32 s[kMemWatchSize];
	int32 index;
public:
	MemWatch() : index(0)
	{
		for(int32 i = 0; i < kMemWatchSize; i++)
		{
			m[i] = 0;
			s[i] = 0;
		}
	}

	~MemWatch()
	{
		if (index != 0)
			PIDebugString("MW:INDEX NOT ZERO!");

		for(int32 i = 0; i < kMemWatchSize; i++)
			if (m[i] != 0)
				PIDebugString("MW:MEMORY NOT ZERO!");
	}
	
	void * Add(void * in, int32 insize)
	{
		// I am passed in the address I created that is bigger
		// than what they requested
		// put a header and footer on the memory
		// and pass back past the memory footer
		
		// I remember the address passed out and the actual size of
		// the memory created
		
		// abort if we have null
		if (in == NULL) return in;
			
		// offset the memory requested
		char * addressToReturn = (char *)in; 
		addressToReturn += 4; // offset by 4
		
		// write the complete memory block with 'A'
		memset(in, 'A', insize);

		// see if we have room to keep track of this address
		if (index == kMemWatchSize)
		{
			PIDebugString("MW:MEMCHECK FULL!");
			return addressToReturn;
		}

		// see if we already have this address		
		for(int32 i = 0; i < kMemWatchSize; i++)
			if (m[i] == addressToReturn)
			{
				PIDebugString("MW:MEMCHECK AGAIN!");
				return addressToReturn;
			}

		m[index] = addressToReturn;
		s[index] = insize;
		index++;

		return addressToReturn;
	}
	
	void * Delete(void * in)
	{
		// now see if the memory is corupt and remove it from our array

		// I rememberd the address passed out and the actual size of
		// the memory created
		
		if (in == NULL)			return in;

		char * addressIRememberd = (char *)in;

		int32 thesizeactuallycreated = GetSize(addressIRememberd);
		
		char * addressActuallyCreated = (char *)in;
		addressActuallyCreated -= 4;
		
		char * addressOfSignatureStart = addressActuallyCreated; // this is what was really created and needs to be deleted
	
		if (addressOfSignatureStart[0] != 'A' || 
		    addressOfSignatureStart[1] != 'A' || 
		    addressOfSignatureStart[2] != 'A' || 
		    addressOfSignatureStart[3] != 'A')
			PIDebugString("MW:BAD SIGNATURE AT BEGINNING!");
		
		if (thesizeactuallycreated == 0)
			PIDebugString("MW:NO SIZE!");
		
		char * addressOfSignatureEnd = addressOfSignatureStart + thesizeactuallycreated - 4;
	
		if (addressOfSignatureEnd[0] != 'A' || 
		    addressOfSignatureEnd[1] != 'A' || 
		    addressOfSignatureEnd[2] != 'A' || 
		    addressOfSignatureEnd[3] != 'A')
			PIDebugString("MW:BAD SIGNATURE AT END!");
		
		if (index == 0)
		{
			PIDebugString("MW:EMPTY!");
			return addressActuallyCreated;
		}

		int32 i;
		for(i = 0; i < kMemWatchSize; i++)
			if (m[i] == addressIRememberd)
			{
				m[i] = NULL;
				break;
			}

		if (i == kMemWatchSize)
		{
			PIDebugString("MW:NOT FOUND!");
			return addressActuallyCreated;
		}

		// shuffle the rest down
		for(int32 j = i; j < index; j++)
		{
			m[j] = m[j+1];
			s[j] = s[j+1];
		}
		m[index] = NULL;
		s[index] = 0;

		index--;

		return addressActuallyCreated;
	}
	
	int32 GetSize(void * in)
	{
		for(int32 j = 0; j < index; j++)
			if (m[j] ==(char *)in)
				return s[j];
		return 0;
	}
	
	size_t Increase(void)
	{
		return (8);
	}
};


MemWatch mw;		

#endif		//#if USE_MEMWATCH


			
//-------------------------------------------------------------------------------
//
//	operator new
//
// Override operator new so that you use memory that Photoshop has.
//
// NOTE:
// Do not use the new operator from the OS. You will end up fighting with 
// Photoshop for memory and you will lose.
//
//-------------------------------------------------------------------------------
void * operator new(size_t s)
{
	void * addr = NULL;

	#if USE_MEMWATCH
		s += mw.Increase();
	#endif

	if (sSPBasic != NULL)
	{
		SPErr error = sSPBasic->AllocateBlock(s, &addr);
		if (error)
		{
			throw( (int16) memFullErr );
			addr = NULL;
		}
		
		#if USE_MEMWATCH
			addr = mw.Add(addr, s);
		#endif
	}
	else
	{
		throw( (int16) paramErr );
	}

	return (addr);
}



//-------------------------------------------------------------------------------
//
//	operator new[]
//
// Override operator new so that you use memory that Photoshop has.
//
// NOTE:
// Do not use the new operator from the OS. You will end up fighting with 
// Photoshop for memory and you will lose.
//
//-------------------------------------------------------------------------------
void * operator new[](size_t s)
{
	void * addr = NULL;

	#if USE_MEMWATCH
		s += mw.Increase();
	#endif

	if (sSPBasic != NULL)
	{
		SPErr error = sSPBasic->AllocateBlock(s, &addr);
		if (error)
		{
			throw( (int16) memFullErr );
			addr = NULL;
		}

		#if USE_MEMWATCH
			addr = mw.Add(addr, s);
		#endif

	}
	else
	{
		throw( (int16) paramErr );
	}

	return (addr);
}



//-------------------------------------------------------------------------------
//
//	operator delete
//
// Override operator delete so that you free the memory that Photoshop has.
//
//-------------------------------------------------------------------------------
void operator delete(void * p)
{

	#if USE_MEMWATCH
		p = mw.Delete(p);
	#endif

	if (sSPBasic != NULL)
	{
		sSPBasic->FreeBlock(p);
	}
}



//-------------------------------------------------------------------------------
//
//	operator delete[]
//
// Override operator delete so that you free the memory that Photoshop has.
//
//-------------------------------------------------------------------------------
void operator delete[](void * p)
{

	#if USE_MEMWATCH
		p = mw.Delete(p);
	#endif

	if (sSPBasic != NULL)
	{
		sSPBasic->FreeBlock(p);
	}
}
// end PIUNew.cpp
