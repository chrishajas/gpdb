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
			CMemoryPoolPallocManager();

			// create new memory pool
			CMemoryPool *Create
				(
				CMemoryPoolManager::AllocType alloc_type
				);

			// release memory pool
			void Destroy(CMemoryPool *);

			// delete memory pools and release manager
			void Shutdown()
			{}

	};
}

#endif // !GPDXL_CMemoryPoolPalloc_H

// EOF
