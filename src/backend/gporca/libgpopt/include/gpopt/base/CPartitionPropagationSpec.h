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

#include "gpopt/base/COptCtxt.h"
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
public:
	enum EPartPropSpecInfoType
	{
		EpptPropagator,
		EpptConsumer,
		EpptSentinel
	};

private:
	struct SPartPropSpecInfo : public CRefCount
	{
		// scan id of the DynamicScan
		INT m_scan_id;

		// info type: consumer or propagator
		EPartPropSpecInfoType m_type;

		// relation id of the DynamicScan
		IMDId *m_root_rel_mdid;

		//  partition selector ids to use (reqd only)
		CBitSet *m_selector_ids = nullptr;

		// filter expressions to generate partition pruning data in the translator (reqd only)
		CExpression *m_filter_expr = nullptr;

		SPartPropSpecInfo(INT scan_id, EPartPropSpecInfoType type,
						  IMDId *rool_rel_mdid)
			: m_scan_id(scan_id), m_type(type), m_root_rel_mdid(rool_rel_mdid)
		{
			GPOS_ASSERT(m_root_rel_mdid != nullptr);

			CMemoryPool *mp = COptCtxt::PoctxtFromTLS()->Pmp();
			m_selector_ids = GPOS_NEW(mp) CBitSet(mp);
		}

		~SPartPropSpecInfo()
		{
			m_root_rel_mdid->Release();
			CRefCount::SafeRelease(m_selector_ids);
			CRefCount::SafeRelease(m_filter_expr);
		}

		IOstream &OsPrint(IOstream &os) const;

		BOOL Equals(const SPartPropSpecInfo *) const;

		BOOL FSatisfies(const SPartPropSpecInfo *) const;

		static INT CmpFunc(const void *val1, const void *val2);
	};

	typedef CDynamicPtrArray<SPartPropSpecInfo, CleanupRelease>
		SPartPropSpecInfoArray;

	// partition required/derived info, sorted by scanid
	SPartPropSpecInfoArray *m_part_prop_spec_infos = nullptr;

	// Present scanids (for easy lookup)
	CBitSet *m_scan_ids = nullptr;

	// return a colrefset containing all the part keys
	// CColRefSet *PcrsKeys(CMemoryPool *mp, CColRef2dArray *pdrgpdrgpcrKeys);

	// return the filter expression for the given Scan Id
	// CExpression *PexprFilter(CMemoryPool *mp, ULONG scan_id);

public:
	CPartitionPropagationSpec(const CPartitionPropagationSpec &) = delete;

	// ctor
	CPartitionPropagationSpec(CMemoryPool *mp);

	// dtor
	~CPartitionPropagationSpec() override;

	// append enforcers to dynamic array for the given plan properties
	void AppendEnforcers(CMemoryPool *mp, CExpressionHandle &exprhdl,
						 CReqdPropPlan *prpp, CExpressionArray *pdrgpexpr,
						 CExpression *pexpr) override;

	// hash function
	ULONG
	HashValue() const override
	{
		GPOS_RTL_ASSERT(false && "Unused");
	}

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

	BOOL
	Contains(ULONG scan_id) const
	{
		return m_scan_ids->Get(scan_id);
	}

	BOOL ContainsAnyConsumers() const;

	// equality function
	BOOL Equals(const CPartitionPropagationSpec *ppps) const;

	// satisfies function
	BOOL FSatisfies(const CPartitionPropagationSpec *ppps) const;


	SPartPropSpecInfo *FindPartPropSpecInfo(INT scan_id) const;

	void Insert(INT scan_id, EPartPropSpecInfoType type, IMDId *rool_rel_mdid,
				CBitSet *selector_ids, CExpression *expr);

	void InsertCopy(SPartPropSpecInfo *other);

	void InsertAll(CPartitionPropagationSpec *pps);

	void InsertAllowedConsumers(CPartitionPropagationSpec *pps,
								CBitSet *allowed_scan_ids);

	void InsertAllExcept(CPartitionPropagationSpec *pps, INT scan_id);

	const CBitSet *SelectorIds(INT scan_id) const;

	// is partition propagation required
	BOOL
	FPartPropagationReqd() const
	{
		return true;
	}

	// print
	IOstream &OsPrint(IOstream &os) const override;

	void InsertAllResolve(CPartitionPropagationSpec *pSpec);
};	// class CPartitionPropagationSpec

}  // namespace gpopt

#endif	// !GPOPT_CPartitionPropagationSpec_H

// EOF
