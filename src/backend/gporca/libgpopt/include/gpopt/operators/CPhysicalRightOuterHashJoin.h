//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2012 EMC Corp.
//
//	@filename:
//		CPhysicalRightOuterHashJoin.h
//
//	@doc:
//		Right outer hash join operator
//---------------------------------------------------------------------------
#ifndef GPOPT_CPhysicalRightOuterHashJoin_H
#define GPOPT_CPhysicalRightOuterHashJoin_H

#include "gpos/base.h"
#include "gpopt/operators/CPhysicalHashJoin.h"

namespace gpopt
{
//---------------------------------------------------------------------------
//	@class:
//		CPhysicalRightOuterHashJoin
//
//	@doc:
//		Right outer hash join operator
//
//---------------------------------------------------------------------------
class CPhysicalRightOuterHashJoin : public CPhysicalHashJoin
{
private:
	// private copy ctor
	CPhysicalRightOuterHashJoin(const CPhysicalRightOuterHashJoin &);

public:
	// ctor
	CPhysicalRightOuterHashJoin(CMemoryPool *mp,
								CExpressionArray *pdrgpexprOuterKeys,
								CExpressionArray *pdrgpexprInnerKeys,
								IMdIdArray *hash_opfamilies = NULL);

	// dtor
	virtual ~CPhysicalRightOuterHashJoin();

	// ident accessors
	virtual EOperatorId
	Eopid() const
	{
		return EopPhysicalRightOuterHashJoin;
	}

	// return a string for operator name
	virtual const CHAR *
	SzId() const
	{
		return "CPhysicalRightOuterHashJoin";
	}

	// conversion function
	static CPhysicalRightOuterHashJoin *
	PopConvert(COperator *pop)
	{
		GPOS_ASSERT(NULL != pop);
		GPOS_ASSERT(EopPhysicalRightOuterHashJoin == pop->Eopid());

		return dynamic_cast<CPhysicalRightOuterHashJoin *>(pop);
	}

	// compute required partition propagation of the n-th child
	virtual CPartitionPropagationSpec *PppsRequired(
		CMemoryPool *mp, CExpressionHandle &exprhdl,
		CPartitionPropagationSpec *pppsRequired, ULONG child_index,
		CDrvdPropArray *pdrgpdpCtxt, ULONG ulOptReq);

	//-------------------------------------------------------------------------------------
	// Required Plan Properties
	//-------------------------------------------------------------------------------------

	// compute required distribution of the n-th child
	virtual CDistributionSpec *PdsRequired(CMemoryPool *mp,
										   CExpressionHandle &exprhdl,
										   CDistributionSpec *pdsRequired,
										   ULONG child_index,
										   CDrvdPropArray *pdrgpdpCtxt,
										   ULONG ulOptReq) const;
};	// class CPhysicalRightOuterHashJoin

}  // namespace gpopt

#endif	// !GPOPT_CPhysicalRightOuterHashJoin_H

// EOF
