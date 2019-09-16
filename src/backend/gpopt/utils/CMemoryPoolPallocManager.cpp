//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2019 Pivotal, Inc.
//
//	@filename:
//		CMemoryPoolPallocManager.cpp
//
//	@doc:
//		CMemoryPoolPallocManager implementation that uses PostgreSQL
//		memory contexts.
//
//---------------------------------------------------------------------------

extern "C" {
#include "postgres.h"

#include "utils/memutils.h"
}

#include "gpopt/utils/CMemoryPoolPalloc.h"
#include "gpopt/utils/CMemoryPoolPallocManager.h"

using namespace gpos;

//---------------------------------------------------------------------------
//	@function:
//		CMemoryPoolPallocManager::CMemoryPoolPallocManager
//
//	@doc:
//		Ctor.
//
//---------------------------------------------------------------------------
CMemoryPoolPallocManager::CMemoryPoolPallocManager(CMemoryPool *internal)
	: CMemoryPoolManager(internal)
{
}


//---------------------------------------------------------------------------
//	@function:
//		CMemoryPoolPallocManager::~CMemoryPoolPallocManager
//
//	@doc:
//		Dtor.
//
//---------------------------------------------------------------------------
CMemoryPoolPallocManager::~CMemoryPoolPallocManager()
{}

CMemoryPool *
CMemoryPoolPallocManager::New(AllocType alloc_type)
{
	return GPOS_NEW(GetInternalMemoryPool()) CMemoryPoolPalloc();
}
// EOF
