//---------------------------------------------------------------------------
//	Greenplum Database
//  Copyright (c) 2020 VMware, Inc.
//
//	@filename:
//		CPhysicalRightOuterHashJoin.cpp
//
//	@doc:
//		Implementation of right outer hash join operator
//---------------------------------------------------------------------------

#include "gpopt/operators/CPhysicalRightOuterHashJoin.h"

#include "gpos/base.h"

#include "gpopt/base/CDistributionSpecNonSingleton.h"
#include "gpopt/base/CDistributionSpecReplicated.h"
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
	ULONG ulDistrReqs = 1 + NumDistrReq();
	SetDistrRequests(ulDistrReqs);
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
CEnfdDistribution *
CPhysicalRightOuterHashJoin::Ped(CMemoryPool *mp, CExpressionHandle &exprhdl,
								 CReqdPropPlan *prppInput, ULONG child_index,
								 CDrvdPropArray *pdrgpdpCtxt, ULONG ulOptReq)
{
	// create the following requests:
	// 1) hash-hash (provided by CPhysicalHashJoin::Ped)
	// 2) singleton-singleton
	//
	// We also could create a replicated-hashed and replicated-non-singleton request, but that isn't a promising
	// alternative as we would be broadcasting the outer side. In that case, an LOJ would be better.

	CDistributionSpec *const pdsInput = prppInput->Ped()->PdsRequired();
	CEnfdDistribution::EDistributionMatching dmatch =
		Edm(prppInput, child_index, pdrgpdpCtxt, ulOptReq);

	if (exprhdl.NeedsSingletonExecution() || exprhdl.HasOuterRefs())
	{
		return GPOS_NEW(mp) CEnfdDistribution(
			PdsRequireSingleton(mp, exprhdl, pdsInput, child_index), dmatch);
	}

	const ULONG ulHashDistributeRequests = NumDistrReq();

	if (ulOptReq < ulHashDistributeRequests)
	{
		// requests 1 .. N are (redistribute, redistribute)
		CDistributionSpec *pds = PdsRequiredRedistribute(
			mp, exprhdl, pdsInput, child_index, pdrgpdpCtxt, ulOptReq);
		if (CDistributionSpec::EdtHashed == pds->Edt())
		{
			CDistributionSpecHashed *pdsHashed =
				CDistributionSpecHashed::PdsConvert(pds);
			pdsHashed->ComputeEquivHashExprs(mp, exprhdl);
		}
		return GPOS_NEW(mp) CEnfdDistribution(pds, dmatch);
	}
	GPOS_ASSERT(ulOptReq == NumDistrReq());
	return GPOS_NEW(mp) CEnfdDistribution(
		PdsRequiredSingleton(mp, exprhdl, pdsInput, child_index, pdrgpdpCtxt),
		dmatch);
}

// EOF
