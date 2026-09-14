// ----------------------------------------------------------------------- //
//
// MODULE  : SelectConfigDlg.h
//
// PURPOSE : Declares the CSelectConfigDlg dialog class.  This class
//           displays a dialog from which the end user can choose a
//           configuration file.
//
// CREATED : 09/09/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// ----------------------------------------------------------------------- //

#pragma once
#ifndef __SELECTCONFIGDLG_H__
#define __SELECTCONFIGDLG_H__

#include "Resource.h"
#include "afxwin.h"

class CSelectConfigDlg : public CDialog
{
// Construction
public:
	CSelectConfigDlg(CWnd* pParent = NULL);   // standard constructor

	// Returns the selected config.
	CString GetSelectedConfig( ) { return m_sConfig; }

	enum { IDD = IDD_SELECTCONFIG };

	CButton	m_OKCtrl;
	CComboBox	m_ConfigCtrl;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:

	// Adds all the config files to the combobox.
	BOOL AddConfigFilesToControl( );
	bool LoadLocalizedStrings();

	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
private:
	CString m_sConfig;
};

#endif  // __SELECTCONFIGDLG_H__
