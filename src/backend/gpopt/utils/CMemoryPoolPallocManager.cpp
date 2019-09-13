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
CMemoryPoolPallocManager::CMemoryPoolPallocManager()
{
	m_global_memory_pool = Create(EatTracker);
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
CMemoryPoolPallocManager::Create(CMemoryPoolManager::AllocType alloc_type)
{
	/*
	 * We use the same implementation for all "kinds" of pools.
	 * 'alloc_type' is ignored.
	 */
	return new CMemoryPoolPalloc();
}


void
CMemoryPoolPallocManager::Destroy(CMemoryPool *mp)
{
	mp->TearDown();
	delete (CMemoryPoolPalloc *) mp;
}

// EOF
