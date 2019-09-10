//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2019 Pivotal, Inc.
//
//	@filename:
//		CMemoryLightweightPoolManager.h
//
//	@doc:
//		Bridge between PostgreSQL memory contexts and GPORCA memory pools.
//
//	@test:
//
//---------------------------------------------------------------------------

#ifndef GPDXL_CMemoryLightweightPoolManager_H
#define GPDXL_CMemoryLightweightPoolManager_H

#include "gpos/base.h"

#include "gpos/memory/CMemoryPoolManager.h"

namespace gpos
{
	// memory pool manager that uses GPDB memory contexts
	class CMemoryLightweightPoolManager : public CMemoryPoolManager
	{
		protected:

			// dtor
			virtual
			~CMemoryLightweightPoolManager();

			// private no copy ctor
			CMemoryLightweightPoolManager(const CMemoryLightweightPoolManager&);

		public:

			// ctor
			CMemoryLightweightPoolManager();

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

#endif // !GPDXL_CMemoryLightweightPool_H

// EOF
