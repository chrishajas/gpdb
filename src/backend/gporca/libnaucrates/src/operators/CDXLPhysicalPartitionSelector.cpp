//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2014 VMware, Inc. or its affiliates.
//
//	@filename:
//		CDXLPhysicalPartitionSelector.cpp
//
//	@doc:
//		Implementation of DXL physical partition selector
//---------------------------------------------------------------------------

#include "naucrates/dxl/operators/CDXLPhysicalPartitionSelector.h"

#include "naucrates/dxl/operators/CDXLNode.h"
#include "naucrates/dxl/xml/CXMLSerializer.h"

using namespace gpos;
using namespace gpdxl;

//---------------------------------------------------------------------------
//	@function:
//		CDXLPhysicalPartitionSelector::CDXLPhysicalPartitionSelector
//
//	@doc:
//		Ctor
//
//---------------------------------------------------------------------------
CDXLPhysicalPartitionSelector::CDXLPhysicalPartitionSelector(
	CMemoryPool *mp, IMDId *mdid_rel, ULONG num_of_part_levels, ULONG scan_id)
	: CDXLPhysical(mp),
	  m_rel_mdid(mdid_rel),
	  m_num_of_part_levels(num_of_part_levels),
	  m_scan_id(scan_id)
{
}

//---------------------------------------------------------------------------
//	@function:
//		CDXLPhysicalPartitionSelector::~CDXLPhysicalPartitionSelector
//
//	@doc:
//		Destructor
//
//---------------------------------------------------------------------------
CDXLPhysicalPartitionSelector::~CDXLPhysicalPartitionSelector()
{
}

//---------------------------------------------------------------------------
//	@function:
//		CDXLPhysicalPartitionSelector::GetDXLOperator
//
//	@doc:
//		Operator type
//
//---------------------------------------------------------------------------
Edxlopid
CDXLPhysicalPartitionSelector::GetDXLOperator() const
{
	return EdxlopPhysicalPartitionSelector;
}

//---------------------------------------------------------------------------
//	@function:
//		CDXLPhysicalPartitionSelector::GetOpNameStr
//
//	@doc:
//		Operator name
//
//---------------------------------------------------------------------------
const CWStringConst *
CDXLPhysicalPartitionSelector::GetOpNameStr() const
{
	return CDXLTokens::GetDXLTokenStr(EdxltokenPhysicalPartitionSelector);
}

//---------------------------------------------------------------------------
//	@function:
//		CDXLPhysicalPartitionSelector::SerializeToDXL
//
//	@doc:
//		Serialize operator in DXL format
//
//---------------------------------------------------------------------------
void
CDXLPhysicalPartitionSelector::SerializeToDXL(CXMLSerializer *xml_serializer,
											  const CDXLNode *dxlnode) const
{
	const CWStringConst *element_name = GetOpNameStr();

	xml_serializer->OpenElement(
		CDXLTokens::GetDXLTokenStr(EdxltokenNamespacePrefix), element_name);
	m_rel_mdid->Serialize(xml_serializer,
						  CDXLTokens::GetDXLTokenStr(EdxltokenRelationMdid));
	xml_serializer->AddAttribute(
		CDXLTokens::GetDXLTokenStr(EdxltokenPhysicalPartitionSelectorLevels),
		m_num_of_part_levels);
	xml_serializer->AddAttribute(
		CDXLTokens::GetDXLTokenStr(EdxltokenPhysicalPartitionSelectorScanId),
		m_scan_id);
	dxlnode->SerializePropertiesToDXL(xml_serializer);

	// serialize project list and filter lists
	(*dxlnode)[0]->SerializeToDXL(xml_serializer);

	// serialize printable filter
	xml_serializer->OpenElement(
		CDXLTokens::GetDXLTokenStr(EdxltokenNamespacePrefix),
		CDXLTokens::GetDXLTokenStr(EdxltokenScalarPrintableFilter));
	(*dxlnode)[1]->SerializeToDXL(xml_serializer);
	xml_serializer->CloseElement(
		CDXLTokens::GetDXLTokenStr(EdxltokenNamespacePrefix),
		CDXLTokens::GetDXLTokenStr(EdxltokenScalarPrintableFilter));

	// serialize relational child, if any
	(*dxlnode)[2]->SerializeToDXL(xml_serializer);

	xml_serializer->CloseElement(
		CDXLTokens::GetDXLTokenStr(EdxltokenNamespacePrefix), element_name);
}

#ifdef GPOS_DEBUG
//---------------------------------------------------------------------------
//	@function:
//		CDXLPhysicalPartitionSelector::AssertValid
//
//	@doc:
//		Checks whether operator node is well-structured
//
//---------------------------------------------------------------------------
void
CDXLPhysicalPartitionSelector::AssertValid(const CDXLNode *dxlnode,
										   BOOL validate_children) const
{
	const ULONG arity = dxlnode->Arity();
	GPOS_ASSERT(6 == arity || 7 == arity);
	for (ULONG idx = 0; idx < arity; ++idx)
	{
		CDXLNode *child_dxlnode = (*dxlnode)[idx];
		if (validate_children)
		{
			child_dxlnode->GetOperator()->AssertValid(child_dxlnode,
													  validate_children);
		}
	}
}
#endif	// GPOS_DEBUG

// EOF
