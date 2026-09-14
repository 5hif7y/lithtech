// ----------------------------------------------------------------------- //
//
// MODULE  : SelectConfigDlg.cpp
//
// PURPOSE : Defines the CSelectConfigDlg dialog class.  This class
//           displays a dialog from which the end user can choose a
//           configuration file.
//
// CREATED : 09/09/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
// (c) 2004 Touchdown Entertainment Inc.  All Rights Reserved
//
// ----------------------------------------------------------------------- //

#include "stdafx.h"
#include "ServerApp.h"
#include "SelectConfigDlg.h"
#include "ServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSelectConfigDlg::CSelectConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectConfigDlg::IDD, pParent)
{
}

void CSelectConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_OKCtrl);
	DDX_Control(pDX, IDC_CONFIG, m_ConfigCtrl);
}

BEGIN_MESSAGE_MAP(CSelectConfigDlg, CDialog)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectConfigDlg message handlers

BOOL CSelectConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// localize the text in the dialog
//	LoadLocalizedStrings();

	// Add all the config files to the combobox.
//	AddConfigFilesToControl( );

	// Enable the OK button only if there are config files found.
	m_OKCtrl.EnableWindow( !!m_ConfigCtrl.GetCount( ));

	// Get the number of configs.
	int nNumConfigs = m_ConfigCtrl.GetCount( );

	// If there are no items to select from, then exit.
	if( nNumConfigs == 0 )
	{
		PostMessage( WM_COMMAND, MAKEWPARAM( IDCANCEL, 0 ));
	}
	else
	{	
		// Select the first item if there are items.
		m_ConfigCtrl.SetCurSel( 0 );

		// If there's just one config, just select it.
		if( nNumConfigs == 1 )
			PostMessage( WM_COMMAND, MAKEWPARAM( IDOK, 0 ));
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}





void CSelectConfigDlg::OnOK() 
{
	// Save the config selected.
	if( m_ConfigCtrl.GetCurSel( ) != LB_ERR )
	{
		m_ConfigCtrl.GetWindowText( m_sConfig );
	}
	
	CDialog::OnOK();
}

