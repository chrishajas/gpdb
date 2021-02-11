//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2012 EMC Corp.
//
//	@filename:
//		CPartitionPropagationSpec.cpp
//
//	@doc:
//		Specification of partition propagation requirements
//---------------------------------------------------------------------------

#include "gpopt/base/CPartitionPropagationSpec.h"

#include "gpos/memory/CAutoMemoryPool.h"

#include "gpopt/exception.h"
#include "gpopt/operators/CPhysicalPartitionSelector.h"
#include "gpopt/operators/CPredicateUtils.h"

using namespace gpos;
using namespace gpopt;


//---------------------------------------------------------------------------
//	@function:
//		CPartitionPropagationSpec::CPartitionPropagationSpec
//
//	@doc:
//		Ctor
//
//---------------------------------------------------------------------------
CPartitionPropagationSpec::CPartitionPropagationSpec()
{
}


//---------------------------------------------------------------------------
//	@function:
//		CPartitionPropagationSpec::~CPartitionPropagationSpec
//
//	@doc:
//		Dtor
//
//---------------------------------------------------------------------------
CPartitionPropagationSpec::~CPartitionPropagationSpec()
{
}


//---------------------------------------------------------------------------
//	@function:
//		CPartitionPropagationSpec::HashValue
//
//	@doc:
//		Hash of components
//
//---------------------------------------------------------------------------
ULONG
CPartitionPropagationSpec::HashValue() const
{
	// FIXME: Fix this!
	return 0;
}


//---------------------------------------------------------------------------
//	@function:
//		CPartitionPropagationSpec::Matches
//
//	@doc:
//		Check whether two partition propagation specs are equal
//
//---------------------------------------------------------------------------
BOOL
CPartitionPropagationSpec::Equals(const CPartitionPropagationSpec *) const
{
	// FIXME: Fix this!
	return false;
}


BOOL
CPartitionPropagationSpec::FSatisfies(
	const CPartitionPropagationSpec *pps) const
{
	// FIXME: Is this the right thing?
	return Equals(pps);
}

//---------------------------------------------------------------------------
//	@function:
//		CPartitionPropagationSpec::AppendEnforcers
//
//	@doc:
//		Add required enforcers to dynamic array
//
//---------------------------------------------------------------------------
void
CPartitionPropagationSpec::AppendEnforcers(CMemoryPool *, CExpressionHandle &,
										   CReqdPropPlan *
#ifdef GPOS_DEBUG

#endif	// GPOS_DEBUG
										   ,
										   CExpressionArray *, CExpression *)
{
}

//---------------------------------------------------------------------------
//	@function:
//		CPartitionPropagationSpec::PexprFilter
//
//	@doc:
//		Return the filter expression for the given Scan Id
//
//---------------------------------------------------------------------------
//CExpression *
//CPartitionPropagationSpec::PexprFilter(CMemoryPool *mp, ULONG scan_id)
//{
//	CExpression *pexprScalar = m_ppfm->Pexpr(scan_id);
//	GPOS_ASSERT(NULL != pexprScalar);
//
//	if (CUtils::FScalarIdent(pexprScalar))
//	{
//		// condition of the form "pkey": translate into pkey = true
//		pexprScalar->AddRef();
//		pexprScalar = CUtils::PexprScalarEqCmp(
//			mp, pexprScalar,
//			CUtils::PexprScalarConstBool(mp, true /*value*/,
//										 false /*is_null*/));
//	}
//	else if (CPredicateUtils::FNot(pexprScalar) &&
//			 CUtils::FScalarIdent((*pexprScalar)[0]))
//	{
//		// condition of the form "!pkey": translate into pkey = false
//		CExpression *pexprId = (*pexprScalar)[0];
//		pexprId->AddRef();
//
//		pexprScalar = CUtils::PexprScalarEqCmp(
//			mp, pexprId,
//			CUtils::PexprScalarConstBool(mp, false /*value*/,
//										 false /*is_null*/));
//	}
//	else
//	{
//		pexprScalar->AddRef();
//	}
//
//	return pexprScalar;
//}

//---------------------------------------------------------------------------
//	@function:
//		CPartitionPropagationSpec::PcrsKeys
//
//	@doc:
//		Return a colrefset containing all the part keys
//
//---------------------------------------------------------------------------
//CColRefSet *
//CPartitionPropagationSpec::PcrsKeys(CMemoryPool *mp,
//									CColRef2dArray *pdrgpdrgpcrKeys)
//{
//	CColRefSet *pcrs = GPOS_NEW(mp) CColRefSet(mp);
//
//	const ULONG ulLevels = pdrgpdrgpcrKeys->Size();
//	for (ULONG ul = 0; ul < ulLevels; ul++)
//	{
//		CColRef *colref = CUtils::PcrExtractPartKey(pdrgpdrgpcrKeys, ul);
//		pcrs->Include(colref);
//	}
//
//	return pcrs;
//}


//---------------------------------------------------------------------------
//	@function:
//		CPartitionPropagationSpec::OsPrint
//
//	@doc:
//		Print function
//
//---------------------------------------------------------------------------
IOstream &
CPartitionPropagationSpec::OsPrint(IOstream &os) const
{
	// FIXME
	return os;
}


// EOF
