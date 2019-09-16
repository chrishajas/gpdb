//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2019 Pivotal, Inc.
//
//	@filename:
//		CMemoryPoolPalloc.cpp
//
//	@doc:
//		CMemoryPoolTracker implementation that uses PostgreSQL memory
//		contexts.
//
//---------------------------------------------------------------------------

extern "C" {
#include "postgres.h"
#include "utils/memutils.h"
}

#include "gpos/memory/CMemoryPool.h"

#include "gpopt/utils/CMemoryPoolPalloc.h"

using namespace gpos;

//---------------------------------------------------------------------------
//	@function:
//		CMemoryPoolPalloc::CMemoryPoolPalloc
//
//	@doc:
//		Ctor.
//
//---------------------------------------------------------------------------
CMemoryPoolPalloc::CMemoryPoolPalloc()
	: m_cxt(NULL)
{
	m_cxt = AllocSetContextCreate(OptimizerMemoryContext,
								  "GPORCA memory pool",
								  ALLOCSET_DEFAULT_MINSIZE,
								  ALLOCSET_DEFAULT_INITSIZE,
								  ALLOCSET_DEFAULT_MAXSIZE);
}


//---------------------------------------------------------------------------
//	@function:
//		CMemoryPoolPalloc::~CMemoryPoolPalloc
//
//	@doc:
//		Dtor.
//
//---------------------------------------------------------------------------
CMemoryPoolPalloc::~CMemoryPoolPalloc()
{
}


//---------------------------------------------------------------------------
//	@function:
//		CMemoryPoolTracker::Allocate
//
//	@doc:
//		Allocate memory.
//
//---------------------------------------------------------------------------
void *
CMemoryPoolPalloc::Allocate
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
//		CMemoryPoolTracker::Free
//
//	@doc:
//		Free memory.
//
//---------------------------------------------------------------------------
void
CMemoryPoolPalloc::Free
	(
	void *ptr
	)
{
	pfree(ptr);
}


//---------------------------------------------------------------------------
//	@function:
//		CMemoryPoolTracker::TearDown
//
//	@doc:
//		Prepare the memory pool to be deleted;
// 		this function is called only once so locking is not required;
//
//---------------------------------------------------------------------------
void
CMemoryPoolPalloc::TearDown()
{
	MemoryContextDelete(m_cxt);
}

ULLONG
CMemoryPoolPalloc::TotalAllocatedSize() const
{
	// FIGGY: Implement this.
	return 0;
}

// EOF
