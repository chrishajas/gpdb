//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2011 EMC Corp.
//
//	@filename:
//		CLogicalRightOuterJoin.h
//
//	@doc:
//		Right outer join operator
//---------------------------------------------------------------------------
#ifndef GPOS_CLogicalRightOuterJoin_H
#define GPOS_CLogicalRightOuterJoin_H

#include "gpos/base.h"
#include "gpopt/operators/CLogicalJoin.h"

namespace gpopt
{
//---------------------------------------------------------------------------
//	@class:
//		CLogicalRightOuterJoin
//
//	@doc:
//		Right outer join operator
//
//---------------------------------------------------------------------------
class CLogicalRightOuterJoin : public CLogicalJoin
{
private:
	// private copy ctor
	CLogicalRightOuterJoin(const CLogicalRightOuterJoin &);

public:
	// ctor
	explicit CLogicalRightOuterJoin(CMemoryPool *mp);

	// dtor
	virtual ~CLogicalRightOuterJoin()
	{
	}

	// ident accessors
	virtual EOperatorId
	Eopid() const
	{
		return EopLogicalRightOuterJoin;
	}

	// return a string for operator name
	virtual const CHAR *
	SzId() const
	{
		return "CLogicalRightOuterJoin";
	}

	// return true if we can pull projections up past this operator from its given child
	virtual BOOL
	FCanPullProjectionsUp(ULONG child_index) const
	{
		return (0 == child_index);
	}

	//-------------------------------------------------------------------------------------
	// Derived Relational Properties
	//-------------------------------------------------------------------------------------

	// derive not nullable output columns
	virtual CColRefSet *
	DeriveNotNullColumns(CMemoryPool *,	 // mp
						 CExpressionHandle &exprhdl) const
	{
		// left outer join passes through not null columns from outer child only
		return PcrsDeriveNotNullPassThruOuter(exprhdl);
	}

	// derive max card
	virtual CMaxCard DeriveMaxCard(CMemoryPool *mp,
								   CExpressionHandle &exprhdl) const;

	// derive constraint property
	virtual CPropConstraint *
	DerivePropertyConstraint(CMemoryPool *,	 //mp,
							 CExpressionHandle &exprhdl) const
	{
		return PpcDeriveConstraintPassThru(exprhdl, 0 /*ulChild*/);
	}

	//-------------------------------------------------------------------------------------
	// Transformations
	//-------------------------------------------------------------------------------------

	// candidate set of xforms
	CXformSet *PxfsCandidates(CMemoryPool *mp) const;

	//-------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------

	// conversion function
	static CLogicalRightOuterJoin *
	PopConvert(COperator *pop)
	{
		GPOS_ASSERT(NULL != pop);
		GPOS_ASSERT(EopLogicalRightOuterJoin == pop->Eopid());

		return dynamic_cast<CLogicalRightOuterJoin *>(pop);
	}

};	// class CLogicalRightOuterJoin

}  // namespace gpopt


#endif	// !GPOS_CLogicalRightOuterJoin_H

// EOF
