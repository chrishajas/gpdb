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

// ctor
CMemoryPoolPallocManager::CMemoryPoolPallocManager(CMemoryPool *internal)
	: CMemoryPoolManager(internal)
{
}

// dtor
CMemoryPoolPallocManager::~CMemoryPoolPallocManager()
{}

// create new memory pool
CMemoryPool *
CMemoryPoolPallocManager::NewMemoryPool()
{
	return GPOS_NEW(GetInternalMemoryPool()) CMemoryPoolPalloc();
}
// EOF
