//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2012 EMC Corp.
//
//	@filename:
//		CPartitionPropagationSpec.h
//
//	@doc:
//		Partition Propagation spec in required properties
//---------------------------------------------------------------------------
#ifndef GPOPT_CPartitionPropagationSpec_H
#define GPOPT_CPartitionPropagationSpec_H

#include "gpos/base.h"
#include "gpos/common/CRefCount.h"

#include "gpopt/base/CPropSpec.h"


namespace gpopt
{
using namespace gpos;

//---------------------------------------------------------------------------
//	@class:
//		CPartitionPropagationSpec
//
//	@doc:
//		Partition Propagation specification
//
//---------------------------------------------------------------------------
class CPartitionPropagationSpec : public CPropSpec
{
private:
	// return a colrefset containing all the part keys
	// CColRefSet *PcrsKeys(CMemoryPool *mp, CColRef2dArray *pdrgpdrgpcrKeys);

	// return the filter expression for the given Scan Id
	// CExpression *PexprFilter(CMemoryPool *mp, ULONG scan_id);

public:
	CPartitionPropagationSpec(const CPartitionPropagationSpec &) = delete;

	// ctor
	CPartitionPropagationSpec();

	// dtor
	~CPartitionPropagationSpec() override;

	// append enforcers to dynamic array for the given plan properties
	void AppendEnforcers(CMemoryPool *mp, CExpressionHandle &exprhdl,
						 CReqdPropPlan *prpp, CExpressionArray *pdrgpexpr,
						 CExpression *pexpr) override;

	// hash function
	ULONG HashValue() const override;

	// extract columns used by the rewindability spec
	CColRefSet *
	PcrsUsed(CMemoryPool *mp) const override
	{
		// return an empty set
		return GPOS_NEW(mp) CColRefSet(mp);
	}

	// property type
	EPropSpecType
	Epst() const override
	{
		return EpstPartPropagation;
	}

	// equality function
	BOOL Equals(const CPartitionPropagationSpec *ppps) const;

	// satisfies function
	BOOL FSatisfies(const CPartitionPropagationSpec *ppps) const;

	// is partition propagation required
	BOOL
	FPartPropagationReqd() const
	{
		return true;
	}

	// print
	IOstream &OsPrint(IOstream &os) const override;

};	// class CPartitionPropagationSpec

}  // namespace gpopt

#endif	// !GPOPT_CPartitionPropagationSpec_H

// EOF
