//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2011 EMC Corp.
//
//	@filename:
//		CLogicalRightOuterJoin.cpp
//
//	@doc:
//		Implementation of right outer join operator
//---------------------------------------------------------------------------

#include "gpos/base.h"

#include "gpopt/base/CColRefSet.h"
#include "gpopt/operators/CExpression.h"
#include "gpopt/operators/CExpressionHandle.h"

#include "gpopt/operators/CLogicalRightOuterJoin.h"

using namespace gpopt;


//---------------------------------------------------------------------------
//	@function:
//		CLogicalRightOuterJoin::CLogicalRightOuterJoin
//
//	@doc:
//		ctor
//
//---------------------------------------------------------------------------
CLogicalRightOuterJoin::CLogicalRightOuterJoin(CMemoryPool *mp)
	: CLogicalJoin(mp)
{
	GPOS_ASSERT(NULL != mp);
}


//---------------------------------------------------------------------------
//	@function:
//		CLogicalRightOuterJoin::DeriveMaxCard
//
//	@doc:
//		Derive max card
//
//---------------------------------------------------------------------------
CMaxCard
CLogicalRightOuterJoin::DeriveMaxCard(CMemoryPool *,  // mp
									  CExpressionHandle &exprhdl) const
{
	CMaxCard maxCard = exprhdl.DeriveMaxCard(0);
	CMaxCard maxCardInner = exprhdl.DeriveMaxCard(1);

	// if the inner has a max card of 0, that will not make the LOJ's
	// max card go to 0
	if (0 < maxCardInner.Ull())
	{
		maxCard *= maxCardInner;
	}

	return CLogical::Maxcard(exprhdl, 2 /*ulScalarIndex*/, maxCard);
}

//---------------------------------------------------------------------------
//	@function:
//		CLogicalRightOuterJoin::PxfsCandidates
//
//	@doc:
//		Get candidate xforms
//
//---------------------------------------------------------------------------
CXformSet *
CLogicalRightOuterJoin::PxfsCandidates(CMemoryPool *mp) const
{
	CXformSet *xform_set = GPOS_NEW(mp) CXformSet(mp);

	(void) xform_set->ExchangeSet(CXform::ExfRightOuterJoin2HashJoin);

	return xform_set;
}



// EOF
