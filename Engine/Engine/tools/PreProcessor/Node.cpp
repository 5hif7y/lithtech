
#include "bdefs.h"
#include "Node.h"
#include "PrePoly.h"



void CNode::ClearPolyPostRemove()
{
	if(!IsValidNode(this))
		return;

	if(m_pPoly)
		m_pPoly->SetPostRemove(FALSE);

	m_Sides[0]->ClearPolyPostRemove();
	m_Sides[1]->ClearPolyPostRemove();
}


void CNode::ClearNodePostRemove()
{
	if(!IsValidNode(this))
		return;
	
	m_bPostRemove = FALSE;

	m_Sides[0]->ClearNodePostRemove();
	m_Sides[1]->ClearNodePostRemove();
}
