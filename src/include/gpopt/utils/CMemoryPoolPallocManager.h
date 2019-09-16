//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2019 Pivotal, Inc.
//
//	@filename:
//		CMemoryPoolPallocManager.h
//
//	@doc:
//		Bridge between PostgreSQL memory contexts and GPORCA memory pools.
//
//	@test:
//
//---------------------------------------------------------------------------

#ifndef GPDXL_CMemoryPoolPallocManager_H
#define GPDXL_CMemoryPoolPallocManager_H

#include "gpos/base.h"

#include "gpos/memory/CMemoryPoolManager.h"

namespace gpos
{
	// memory pool manager that uses GPDB memory contexts
	class CMemoryPoolPallocManager : public CMemoryPoolManager
	{
		protected:

			// dtor
			virtual
			~CMemoryPoolPallocManager();

			// private no copy ctor
			CMemoryPoolPallocManager(const CMemoryPoolPallocManager&);

		public:

			// ctor
			CMemoryPoolPallocManager(CMemoryPool *internal);

			// FIGGY: Write a blog post about what only this needs to be overridden
			// FIGGY: Maybe remove the cpp file also?
			virtual CMemoryPool *NewMemoryPool();
	};
}

#endif // !GPDXL_CMemoryPoolPalloc_H

// EOF
