//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2019 Pivotal, Inc.
//
//	@filename:
//		CMemoryContextPool.h
//
//	@doc:
//		Bridge between PostgreSQL memory contexts and GPORCA memory pools.
//
//	@test:
//
//---------------------------------------------------------------------------

#ifndef GPDXL_CMemoryContextPool_H
#define GPDXL_CMemoryContextPool_H

#include "gpos/base.h"

#include "gpos/memory/CMemoryPool.h"

namespace gpos
{
	// Memory pool that maps to a Postgres MemoryContext.
	class CMemoryContextPool : public CMemoryPool
	{
		private:

			MemoryContext m_cxt;

		public:

			// ctor
			CMemoryContextPool();

			// dtor
			virtual
			~CMemoryContextPool();

			// allocate memory
			void *Allocate
				(
				const ULONG bytes,
				const CHAR *file,
				const ULONG line
				);

			// free memory
			void Free(void *ptr);

			// prepare the memory pool to be deleted
			void TearDown();

			// return total allocated size
			ULLONG TotalAllocatedSize() const
			{
				return 0;
			}

	};
}

#endif // !GPDXL_CMemoryContextPool_H

// EOF
