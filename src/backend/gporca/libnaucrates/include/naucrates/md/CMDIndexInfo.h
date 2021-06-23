//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2017 Pivotal Software Inc.
//
//	@filename:
//		CMDIndexInfo.h
//
//	@doc:
//		Implementation of indexinfo in relation metadata
//---------------------------------------------------------------------------

#ifndef GPMD_CMDIndexInfo_H
#define GPMD_CMDIndexInfo_H

#include "gpos/base.h"

#include "naucrates/md/IMDId.h"
#include "naucrates/md/IMDInterface.h"

namespace gpmd
{
using namespace gpos;
using namespace gpdxl;

// class for indexinfo in relation metadata
class CMDIndexInfo : public IMDInterface
{
private:
	// index mdid
	IMDId *m_mdid;

	// is the index partial
	BOOL m_is_partial;

	// set if index columns layout mismatches between root and leaf for partitioned tables,
	// otherwise false
	BOOL m_indexcols_mismatch_rootcols;

public:
	// ctor
	CMDIndexInfo(IMDId *mdid, BOOL is_partial,
				 BOOL indexcols_mismatch_rootcols);

	// dtor
	virtual ~CMDIndexInfo();

	// index mdid
	IMDId *MDId() const;

	// is the index partial
	BOOL IsPartial() const;

	// has mismatched index columns layout between root and leaf
	BOOL HasMismatchedIndexCols() const;

	// serialize indexinfo in DXL format given a serializer object
	virtual void Serialize(CXMLSerializer *) const;

#ifdef GPOS_DEBUG
	// debug print of the index info
	virtual void DebugPrint(IOstream &os) const;
#endif
};

}  // namespace gpmd

#endif	// !GPMD_CMDIndexInfo_H

// EOF
