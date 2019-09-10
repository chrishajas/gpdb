//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2019 Pivotal, Inc.
//
//	@filename:
//		CMemoryLightweightPool.cpp
//
//	@doc:
//		CMemoryLightweightPool implementation that uses PostgreSQL memory
//		contexts.
//
//---------------------------------------------------------------------------

extern "C" {
#include "postgres.h"

#include "utils/memutils.h"
}

#include "gpos/memory/CMemoryPool.h"

#include "gpopt/utils/CMemoryLightweightPool.h"

using namespace gpos;

//---------------------------------------------------------------------------
//	@function:
//		CMemoryLightweightPool::CMemoryLightweightPool
//
//	@doc:
//		Ctor.
//
//---------------------------------------------------------------------------
CMemoryLightweightPool::CMemoryLightweightPool()
{
	m_cxt = AllocSetContextCreate(TopMemoryContext,
				      "GPORCA memory pool",
				      ALLOCSET_DEFAULT_MINSIZE,
				      ALLOCSET_DEFAULT_INITSIZE,
				      ALLOCSET_DEFAULT_MAXSIZE);
}


//---------------------------------------------------------------------------
//	@function:
//		CMemoryLightweightPool::~CMemoryLightweightPool
//
//	@doc:
//		Dtor.
//
//---------------------------------------------------------------------------
CMemoryLightweightPool::~CMemoryLightweightPool()
{
}


//---------------------------------------------------------------------------
//	@function:
//		CMemoryLightweightPool:Allocate
//
//	@doc:
//		Allocate memory.
//
//---------------------------------------------------------------------------
void *
CMemoryLightweightPool::Allocate
	(
	const ULONG bytes,
	const CHAR *,
	const ULONG
	)
{
	return MemoryContextAlloc(m_cxt, bytes);
}

//---------------------------------------------------------------------------
//	@function:
//		CMemoryLightweightPool::Free
//
//	@doc:
//		Free memory.
//
//---------------------------------------------------------------------------
void
CMemoryLightweightPool::Free
	(
	void *ptr
	)
{
	pfree(ptr);
}


//---------------------------------------------------------------------------
//	@function:
//		CMemoryLightweightPool::TearDown
//
//	@doc:
//		Prepare the memory pool to be deleted;
// 		this function is called only once so locking is not required;
//
//---------------------------------------------------------------------------
void
CMemoryLightweightPool::TearDown()
{
	MemoryContextDelete(m_cxt);
}

// EOF
