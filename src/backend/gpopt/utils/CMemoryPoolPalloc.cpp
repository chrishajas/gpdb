//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2019 Pivotal, Inc.
//
//	@filename:
//		CMemoryPoolPalloc.cpp
//
//	@doc:
//		CMemoryPool implementation that uses PostgreSQL memory
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

// ctor
CMemoryPoolPalloc::CMemoryPoolPalloc()
	: m_cxt(NULL)
{
	m_cxt = AllocSetContextCreate(OptimizerMemoryContext,
								  "GPORCA memory pool",
								  ALLOCSET_DEFAULT_MINSIZE,
								  ALLOCSET_DEFAULT_INITSIZE,
								  ALLOCSET_DEFAULT_MAXSIZE);
}

void *
CMemoryPoolPalloc::NewImpl
	(
	const ULONG bytes,
	const CHAR *,
	const ULONG,
	CMemoryPool::EAllocationType
	)
{
	return MemoryContextAlloc(m_cxt, bytes);
}

void
CMemoryPoolPalloc::Free
	(
	void *ptr
	)
{
	pfree(ptr);
}

// Prepare the memory pool to be deleted
void
CMemoryPoolPalloc::TearDown()
{
	MemoryContextDelete(m_cxt);
}

// Total allocated size including management overheads
ULLONG
CMemoryPoolPalloc::TotalAllocatedSize() const
{
	return MemoryContextGetCurrentSpace(m_cxt);
}

ULONG
CMemoryPoolPalloc::SizeOfAlloc(const void *ptr)
{
	// FIGGY
	return GetAllocSize(ptr);
}


// EOF
