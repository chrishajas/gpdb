//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2019 Pivotal, Inc.
//
//	@filename:
//		CMemoryLightweightPoolManager.cpp
//
//	@doc:
//		CMemoryLightweightPoolManager implementation that uses PostgreSQL
//		memory contexts.
//
//---------------------------------------------------------------------------

extern "C" {
#include "postgres.h"

#include "utils/memutils.h"
}

#include "gpopt/utils/CMemoryLightweightPool.h"
#include "gpopt/utils/CMemoryLightweightPoolManager.h"

using namespace gpos;

//---------------------------------------------------------------------------
//	@function:
//		CMemoryLightweightPoolManager::CMemoryLightweightPoolManager
//
//	@doc:
//		Ctor.
//
//---------------------------------------------------------------------------
CMemoryLightweightPoolManager::CMemoryLightweightPoolManager()
{
	m_global_memory_pool = Create(EatTracker);
}


//---------------------------------------------------------------------------
//	@function:
//		CMemoryLightweightPoolManager::~CMemoryLightweightPoolManager
//
//	@doc:
//		Dtor.
//
//---------------------------------------------------------------------------
CMemoryLightweightPoolManager::~CMemoryLightweightPoolManager()
{}

CMemoryPool *
CMemoryLightweightPoolManager::Create(CMemoryPoolManager::AllocType alloc_type)
{
	/*
	 * We use the same implementation for all "kinds" of pools.
	 * 'alloc_type' is ignored.
	 */
	return new CMemoryLightweightPool();
}


void
CMemoryLightweightPoolManager::Destroy(CMemoryPool *mp)
{
	mp->TearDown();
	delete (CMemoryLightweightPool *) mp;
}

// EOF
