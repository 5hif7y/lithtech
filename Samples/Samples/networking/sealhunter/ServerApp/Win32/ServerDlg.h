// ----------------------------------------------------------------------- //
//
// MODULE  : ServerDlg.h
//
// PURPOSE : Declares the CServerDlg class.  This class creates the main
//           server dialog.
//
// CREATED : 09/09/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// ----------------------------------------------------------------------- //

#pragma once
#ifndef __SERVERDLG_H__
#define __SERVERDLG_H__

#include <afxtempl.h>
#include "resource.h"
#include "server_interface.h"
#include <strstream>
//#include "ProfileUtils.h"
#include "DedicatedServerBase.h"
#include "PlayerInfo.h"
#include "Splash.h"

class CServerDlg : public CDialog, 
				   public CDedicatedServerBase
{
public:

// Construction
public:
	CServerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CServerDlg();

	enum { IDD = IDD_GAMESERVER };
	CButton	m_SelectLevel;
	CButton	m_PlayerBoot;
	CListCtrl	m_Players;
	CListCtrl	m_Levels;
	CEdit	m_edConsole;
	CString	m_sServerName;
	CString m_sGameType;
	CString	m_sServerTime;
	CString	m_sTimeInLevel;
	CString	m_sNumPlayers;
	CString	m_sTotalPlayers;
	DWORD	m_nPeakPlayers;
	DWORD	m_nAveragePing;


	public:
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:

	virtual BOOL OnInitDialog();
	afx_msg void OnConsoleSend();
	afx_msg void OnConsoleClear();
	afx_msg void OnCommandsNextLevel();
	afx_msg void OnCommandsSelectLevel();
	afx_msg void OnPlayersBoot();
	afx_msg void OnCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDblclkLevels(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnStopserver();
	afx_msg void OnItemchangedLevels(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedPlayers(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnStartup();
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()

public:

	void RemoveMessage(int nMsg, int nMax);
	void WriteConsoleString(LPCTSTR pMsg, ...);
	void WriteConsoleStringID( const char* szStringID );
	bool SetSelectedWorlds( );
	
private:

	// DedicatedServerBase
	virtual void OnServerInit();
	virtual void OnServerPreAddClient(CPlayerInfo& cPlayerInfo);
	virtual void OnServerPostAddClient(CPlayerInfo& cPlayerInfo);
	virtual void OnServerRemovedClient(CPlayerInfo& cPlayerInfo);
	virtual void OnServerUpdate();
	virtual void OnServerPreLoadWorld();
	virtual void OnServerPostLoadWorld();
	virtual void OnServerError(ServerErrorEnum eServerError);
//	virtual wchar_t const* OnServerLoadString(const char* szResId);

	// helper functions
//	const char* GetServerGameOptions();
	bool	ConfirmStop();
	void	UpdateUI();
	bool	GetResourceFiles(ResourceFileList& cResourceFiles);
	bool	LoadServer();
	bool	RunServer();
	bool	StopServer();
	bool	SelectLevel(int nLevelIndex);
	CString	FormatTime(CTimeSpan const& timeSpan);
	void    HandleFatalError( const char* szStringId );
	bool	LoadLocalizedStrings();

	// state
	bool	  m_bFirstShow;
	CTime	  m_serverStartTime;
	CTimeSpan m_serverRunTime;
	CTime	  m_levelStartTime;
	CTimeSpan m_levelRunTime;
	int		  m_nCurLevel;
	DWORD	  m_nMaxPlayers;
	DWORD	  m_nGamePlayers;

	bool m_bPlayerUpdate_Shell;
	bool m_bChangingLevels_Shell;
	bool m_bChangedLevel_Shell;
	bool m_bConfirmExit;

	CImageList*		  m_pImageList;
//	ServerGameOptions m_ServerGameOptions;

	CSplashWnd m_wndSplash;
};

#endif  // __SERVERDLG_H__
