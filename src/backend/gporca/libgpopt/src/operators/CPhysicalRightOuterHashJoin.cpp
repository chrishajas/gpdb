//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2012 EMC Corp.
//
//	@filename:
//		CPhysicalRightOuterHashJoin.cpp
//
//	@doc:
//		Implementation of right outer hash join operator
//---------------------------------------------------------------------------

#include "gpos/base.h"
#include "gpopt/base/CDistributionSpecHashed.h"
#include "gpopt/base/CDistributionSpecSingleton.h"
#include "gpopt/base/CDistributionSpecReplicated.h"
#include "gpopt/operators/CPhysicalRightOuterHashJoin.h"
#include "gpopt/operators/CExpressionHandle.h"


using namespace gpopt;


//---------------------------------------------------------------------------
//	@function:
//		CPhysicalRightOuterHashJoin::CPhysicalRightOuterHashJoin
//
//	@doc:
//		Ctor
//
//---------------------------------------------------------------------------
CPhysicalRightOuterHashJoin::CPhysicalRightOuterHashJoin(
	CMemoryPool *mp, CExpressionArray *pdrgpexprOuterKeys,
	CExpressionArray *pdrgpexprInnerKeys, IMdIdArray *hash_opfamilies)
	: CPhysicalHashJoin(mp, pdrgpexprOuterKeys, pdrgpexprInnerKeys,
						hash_opfamilies)
{
	SetPartPropagateRequests(2);
}


//---------------------------------------------------------------------------
//	@function:
//		CPhysicalRightOuterHashJoin::~CPhysicalRightOuterHashJoin
//
//	@doc:
//		Dtor
//
//---------------------------------------------------------------------------
CPhysicalRightOuterHashJoin::~CPhysicalRightOuterHashJoin()
{
}

//---------------------------------------------------------------------------
//	@function:
//		CPhysicalRightOuterHashJoin::PppsRequired
//
//	@doc:
//		Compute required partition propagation of the n-th child
//
//---------------------------------------------------------------------------
CPartitionPropagationSpec *
CPhysicalRightOuterHashJoin::PppsRequired(
	CMemoryPool *mp, CExpressionHandle &exprhdl,
	CPartitionPropagationSpec *pppsRequired, ULONG child_index,
	CDrvdPropArray *pdrgpdpCtxt, ULONG ulOptReq)
{
	// This is identical to the inner join partition request
	if (1 == ulOptReq)
	{
		// request (1): push partition propagation requests to join's children,
		// do not consider possible dynamic partition elimination using join predicate here,
		// this is handled by optimization request (0) below
		return CPhysical::PppsRequiredPushThruNAry(mp, exprhdl, pppsRequired,
												   child_index);
	}

	// request (0): push partition progagation requests to join child considering
	// DPE possibility. For HJ, PS request is pushed to the inner child if there
	// is a consumer (DTS) on the outer side of the join.
	GPOS_ASSERT(0 == ulOptReq);
	return PppsRequiredJoinChild(mp, exprhdl, pppsRequired, child_index,
								 pdrgpdpCtxt, false);
}

//---------------------------------------------------------------------------
//	@function:
//		CPhysicalRightOuterHashJoin::PdsRequired
//
//	@doc:
//		Compute required distribution of the n-th child
//
//---------------------------------------------------------------------------
CDistributionSpec *
CPhysicalRightOuterHashJoin::PdsRequired(
	CMemoryPool *mp, CExpressionHandle &exprhdl, CDistributionSpec *pdsInput,
	ULONG child_index, CDrvdPropArray *pdrgpdpCtxt,
	ULONG ulOptReq	// identifies which optimization request should be created
) const
{
	GPOS_ASSERT(ulOptReq < UlDistrRequests());

	if (!FFirstChildToOptimize(child_index))
	{
		CDistributionSpec *pdsFirst =
			CDrvdPropPlan::Pdpplan((*pdrgpdpCtxt)[0])->Pds();
		if (0 == child_index &&
			pdsFirst->Edt() == CDistributionSpec::EdtReplicated)
		{
			// we need to replicate the outer if the inner table is replicated, otherwise we'll get duplicate results
			return GPOS_NEW(mp) CDistributionSpecReplicated();
		}
	}

	return CPhysicalHashJoin::PdsRequired(mp, exprhdl, pdsInput, child_index,
										  pdrgpdpCtxt, ulOptReq);
}

// EOF
