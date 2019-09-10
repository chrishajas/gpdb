//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2019 Pivotal, Inc.
//
//	@filename:
//		CMemoryLightweightPool.h
//
//	@doc:
//		Bridge between PostgreSQL memory contexts and GPORCA memory pools.
//
//	@test:
//
//---------------------------------------------------------------------------

#ifndef GPDXL_CMemoryLightweightPool_H
#define GPDXL_CMemoryLightweightPool_H

#include "gpos/base.h"

#include "gpos/memory/CMemoryPool.h"

namespace gpos
{
	// Memory pool that maps to a Postgres MemoryContext.
	class CMemoryLightweightPool : public CMemoryPool
	{
		private:

			MemoryContext m_cxt;

		public:

			// ctor
			CMemoryLightweightPool();

			// dtor
			virtual
			~CMemoryLightweightPool();

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

	};
}

#endif // !GPDXL_CMemoryLightweightPool_H

// EOF
