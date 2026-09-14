// ----------------------------------------------------------------------- //
//
// MODULE  : ServerDlg.cpp
//
// PURPOSE : Defines the CServerDlg class.  This class creates the main
//           server dialog.
//
// CREATED : 09/09/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
// (c) 2004 Touchdown Entertainment Inc.  All Rights Reserved
//
// ----------------------------------------------------------------------- //

#include "stdafx.h"
#include "server_interface.h"
#include "ServerApp.h"
#include "ServerDlg.h"
#include <stdio.h>
#include <afxtempl.h>
#include <process.h>
#include "mmsystem.h"
#include "SelectConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char const REGPRODUCTVER[] = "1.0";


// Column identifiers for player list control.
enum PlayerColumns
{
	ePlayerColumnsName,
	ePlayerColumnsPing,
	ePlayerColumnsKills,
	ePlayerColumnsDeaths,
	ePlayerColumnsScore,
	ePlayerColumnsTime,
};

extern LTGUID GAMEGUID;

/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog

CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerDlg::IDD, pParent)
{
	m_sServerName = ("SealHunter Server");
	m_sGameType = ("SealMatch");
	m_sServerTime = _T("");
	m_sTimeInLevel = _T("");
	m_sNumPlayers = _T("0");
	m_sTotalPlayers = _T( "0/12" );
	m_nPeakPlayers = 0;
	m_nAveragePing = 0;

	m_bConfirmExit = TRUE;
	m_bFirstShow = TRUE;
	m_nMaxPlayers = 12;
	m_nGamePlayers = 0;

	m_pImageList = NULL;
}


CServerDlg::~CServerDlg() 
{
	// free image list
	if (m_pImageList)
	{
		delete m_pImageList;
	}

	ShutdownServer(); 
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMMANDS_SELLEVEL, m_SelectLevel);
	DDX_Control(pDX, IDC_PLAYER_BOOT, m_PlayerBoot);
	DDX_Control(pDX, IDC_PLAYERS, m_Players);
	DDX_Control(pDX, IDC_LEVELS, m_Levels);
	DDX_Control(pDX, IDC_CONSOLE_WINDOW, m_edConsole);
	DDX_Text(pDX, IDC_SERVER_NAME, m_sServerName);
	DDX_Text(pDX, IDC_GAME_TYPE, m_sGameType);
	DDX_Text(pDX, IDC_SERVER_TIME, m_sServerTime);
	DDX_Text(pDX, IDC_TIMEINLEVEL, m_sTimeInLevel);
	DDX_Text(pDX, IDC_NUM_PLAYERS, m_sNumPlayers);
	DDX_Text(pDX, IDC_PEAKPLAYERS, m_nPeakPlayers);
	DDX_Text(pDX, IDC_TOTALPLAYERS, m_sTotalPlayers);
	DDX_Text(pDX, IDC_AVERAGEPING, m_nAveragePing);
}


BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
	ON_BN_CLICKED(IDC_CONSOLE_SEND, OnConsoleSend)
	ON_BN_CLICKED(IDC_CONSOLE_CLEAR, OnConsoleClear)
	ON_BN_CLICKED(IDC_COMMANDS_NEXTLEVEL, OnCommandsNextLevel)
	ON_BN_CLICKED(IDC_COMMANDS_SELLEVEL, OnCommandsSelectLevel)
	ON_BN_CLICKED(IDC_PLAYER_BOOT, OnPlayersBoot)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_NOTIFY(NM_DBLCLK, IDC_LEVELS, OnDblclkLevels)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_STOPSERVER, OnStopserver)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LEVELS, OnItemchangedLevels)
	ON_WM_CLOSE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PLAYERS, OnItemchangedPlayers)
	ON_COMMAND( IDC_STARTUP, OnStartup )
END_MESSAGE_MAP()

bool CServerDlg::LoadServer()
{
	CWaitCursor wc;
	CString sWorlds, sWorldsKey, sWorld;

	WriteConsoleStringID("IDS_SERVERAPP_CONSOLE_INITSERVER");

	ResourceFileList cResourceFiles;
	if (!GetResourceFiles(cResourceFiles))
	{
		HandleFatalError("IDS_SERVERAPP_ERROR_LOADREZ");
		return FALSE;
	}

	if (!InitializeServer(SI_VERSION, GAMEGUID, "server.dll", cResourceFiles))
	{
		return FALSE;
	}

	// Clear some shell variables.
	m_nCurLevel = -1;
	m_bPlayerUpdate_Shell = FALSE;
	m_bChangingLevels_Shell = FALSE;
	m_bChangedLevel_Shell = FALSE;

	//WriteConsoleStringID( "IDS_SERVERAPP_CONSOLE_SERVERINITED" );

	return(TRUE);
}

bool CServerDlg::RunServer()
{
	CWaitCursor wc;

	if (!GetServerInterface())
		return false;

	// get game options
//	const char* szError = GetServerGameOptions();
//	if (szError && (szError[0] != '\0') )
//	{
//		return false;
//	}

	
	// start the server
//	StartServer(m_ServerGameOptions);

	StartServer();

	// Give us a timer update every 100 ms.
	SetTimer( 0, 100, NULL );

	// Init some stuff now that we're running...
	m_serverStartTime = CTime::GetCurrentTime( );

	// All done...
	WriteConsoleStringID("Server running");

	return true;
}

bool CServerDlg::StopServer()
{
	// call base class to shutdown the server
	ShutdownServer();

	return true;
}

/* const char* CServerDlg::GetServerGameOptions()
{
	CWaitCursor wc;
	
	// Make sure we have server interface.
	if (!GetServerInterface())
	{
		return "IDS_SERVERAPP_CONSOLE_UNABLETOHOST";
	}

	// Process command line for the config file to use.
	ParseCommandLine( true, &m_ServerGameOptions, __argc, __argv );

	// Check if no profile was specified.
	if( m_ServerGameOptions.m_sProfileName.empty( ))
	{
		// Ask the user to select from a config.
		CSelectConfigDlg dlg;
		if( dlg.DoModal( ) != IDOK )
		{
			return "IDS_SERVERAPP_NETERR_NOCONFIGS";
		}

		// Get the config the user selected.
		m_ServerGameOptions.m_sProfileName = dlg.GetSelectedConfig( );

		// If the config is still empty, then we cannot proceed.
		if( m_ServerGameOptions.m_sProfileName.empty( ))
		{
			return "IDS_SERVERAPP_NETERR_NOCONFIGS";
		}
	}

	// Get the path to the profile file for parsing.
//	std::string sProfilePath = GetProfileFile( );

	// Load the server options
	std::string fn = GetProfileDir(m_ServerGameOptions.m_sProfileName.c_str( ));
	if( CWinUtil::DirExist( fn.c_str() ))
	{
		fn += g_szServerOptionFileName;
		if( !m_ServerGameOptions.Load(MPA2W(fn.c_str()).c_str()))
		{
			return "IDS_SERVERAPP_NETERR_CORRUPTCONFIG";
		}
	}
	else
	{
		return "IDS_SERVERAPP_NETERR_CORRUPTCONFIG";
	}

	DATABASE_CATEGORY( GameModes ).Init( );
	HRECORD hGameModeRecord = g_pLTDatabase->GetRecord( DATABASE_CATEGORY( GameModes ).GetCategory(), 
		m_ServerGameOptions.m_sGameMode.c_str( ));
	if( !GameModeMgr::Instance( ).Init( hGameModeRecord ))
	{
		return "IDS_SERVERAPP_NETERR_CORRUPTCONFIG";
	}
	GameModeMgr::Instance( ).ReadFromOptionsFile( ".\\serveroptions.txt" );


	// Force this to be a dedicated server.
	m_ServerGameOptions.m_bDedicated = true;

	// Process command line for any overriding game info.
	ParseCommandLine( false, &m_ServerGameOptions, __argc, __argv );

	// Get the server name so we can display it.
	m_sServerName = GameModeMgr::Instance( ).m_grwsSessionName;

	// Fill in the name of the game type and max players.
	HRECORD hRecord = g_pLTDatabase->GetRecord( DATABASE_CATEGORY( GameModes ).GetCategory( ), 
		m_ServerGameOptions.m_sGameMode.c_str( ));
	m_sGameType = MPW2CT(LoadString( DATABASE_CATEGORY( GameModes ).GETRECORDATTRIB( hRecord, Label ))).c_str();
	m_nMaxPlayers = GameModeMgr::Instance( ).m_grnMaxPlayers;
	m_sNumPlayers.Format( _T("%d/%d") , 0, m_nMaxPlayers);

	// Set the selected mod so players know what we are playing...

	// Open the registry.
	CRegMgr32 regMgr;
	regMgr.Init();
	if( !regMgr.OpenKey( HKEY_LOCAL_MACHINE, "SOFTWARE", "Monolith Productions", (char*)GAME_NAME,
		(char *)REGPRODUCTVER ))
	{
		return "";
	}

	char szSelectedMod[256] = {0};
	DWORD bufSize = sizeof(szSelectedMod);
	
	if( regMgr.GetField( FIELD_SELECTEDMOD, szSelectedMod, bufSize ))
	{
		if( szSelectedMod[0] )
		{
			m_ServerGameOptions.m_sModName = MPA2W(szSelectedMod).c_str();
		}

	}

	if( !szSelectedMod[0] )
	{
		CString sModName;
		sModName = RETAIL_MOD_NAME;
		m_ServerGameOptions.m_sModName = MPT2CW(sModName).c_str();
	}

	return "";
}
*/

void CServerDlg::WriteConsoleStringID( const char* szStringID )
{
	WriteConsoleString( szStringID );
}


void CServerDlg::WriteConsoleString(LPCTSTR pMsg, ...)
{

	char		str[500];
	va_list		marker;
	int			nLen;

	// make sure the window exists
	if( !::IsWindow(m_edConsole.GetSafeHwnd()) )
		return;

	static int nMax = 250;

	if(m_edConsole.GetLineCount() > nMax)
	{
		// Nuke the oldest 75%.
		int iLine = (m_edConsole.GetLineCount()*75) / 100;
		int iChar = m_edConsole.LineIndex(iLine);
		
		m_edConsole.SetRedraw(FALSE);
		m_edConsole.SetSel(0, iChar);
		m_edConsole.ReplaceSel("", FALSE);
		m_edConsole.SetRedraw(TRUE);
	}

	va_start(marker, pMsg);
	_vsnprintf( str, sizeof( str ) - 1, pMsg, marker );
	va_end(marker);

	int len = (int) strlen(str);
	if (len > 0)
	{
		if (len > 0 && str[len-1] < 32) str[len-1] = '\0';

		strcat(str, "\r\n");

		nLen = (int) m_edConsole.SendMessage(EM_GETLIMITTEXT, 0, 0);
		m_edConsole.SetSel(nLen, nLen);
		m_edConsole.ReplaceSel(str);

		// Write to log file too.
		WriteToErrorLog( str );
	}

}

/////////////////////////////////////////////////////////////////////////////
// CServerDlg message handlers

BOOL CServerDlg::OnInitDialog() 
{
	// Parse command line for any startup info.
	ParseCommandLine( false, NULL, __argc, __argv );


	// Load up the server.
	if( !LoadServer() )
	{
		HandleFatalError("IDS_SERVERAPP_ERROR_CANTSTARTSERVER");
		return FALSE;
	}


	CRect rect;
	CTimeSpan timeSpan( 0 );

	CDialog::OnInitDialog();
	CenterWindow();

	m_wndSplash.Create( this );

	m_wndSplash.ShowSplashScreen();

	// just for show
	WriteConsoleStringID( "Initialize Server" );

	// Set the server up time.
	m_sServerTime = FormatTime( timeSpan );

	// Init the game progress.
	m_nMaxPlayers = 12;
	m_nGamePlayers = 0;

	m_sTimeInLevel = FormatTime( timeSpan );
	m_levelStartTime = CTime::GetCurrentTime( );
	m_serverStartTime = CTime::GetCurrentTime( );

	// Create the image list
//	m_pImageList = new CImageList;
//	m_pImageList->Create( 16, 16, TRUE, 9, 1 );
//	m_pImageList->Add( AfxGetApp( )->LoadIcon( IDI_SELECTED ));

	// Setup player list.
//	m_Players.SetImageList( m_pImageList, LVSIL_STATE );

//	sColumnHeader = LoadString( "IDS_SERVERAPP_COLUMNHEADER_NAME" );
	m_Players.InsertColumn( ePlayerColumnsName, "NAME" ); // MPW2CT(sColumnHeader).c_str() );
//	sColumnHeader = LoadString( "IDS_SERVERAPP_COLUMNHEADER_PING" );
	m_Players.InsertColumn( ePlayerColumnsPing, "PING" ); // MPW2CT(sColumnHeader).c_str() );
//	sColumnHeader = LoadString( "IDS_SERVERAPP_COLUMNHEADER_KILLS" );
	m_Players.InsertColumn( ePlayerColumnsKills, "KILLS" ); // MPW2CT(sColumnHeader).c_str() );
//	sColumnHeader = LoadString( "IDS_SERVERAPP_COLUMNHEADER_DEATHS" );
	m_Players.InsertColumn( ePlayerColumnsDeaths, "DEATHS" ); // MPW2CT(sColumnHeader).c_str() );
//	sColumnHeader = LoadString( "IDS_SERVERAPP_COLUMNHEADER_SCORE" );
	m_Players.InsertColumn( ePlayerColumnsScore, "SCORE" ); // MPW2CT(sColumnHeader).c_str() );
//	sColumnHeader = LoadString( "IDS_SERVERAPP_COLUMNHEADER_TIMEONSERVER" );
	m_Players.InsertColumn( ePlayerColumnsTime, "TIME" ); // MPW2CT(sColumnHeader).c_str() );

	m_Players.GetWindowRect( &rect );
	int nWidth = rect.Width() - 22;

	m_Players.SetColumnWidth( ePlayerColumnsName,	(int)(nWidth * 0.35));
	m_Players.SetColumnWidth( ePlayerColumnsPing,	(int)(nWidth * 0.11));
	m_Players.SetColumnWidth( ePlayerColumnsKills,	(int)(nWidth * 0.11));
	m_Players.SetColumnWidth( ePlayerColumnsDeaths,	(int)(nWidth * 0.11));
	m_Players.SetColumnWidth( ePlayerColumnsScore,	(int)(nWidth * 0.13));
	m_Players.SetColumnWidth( ePlayerColumnsTime,	(int)(nWidth * 0.18));

	// Disable the boot button.
	m_PlayerBoot.EnableWindow( FALSE );

	// Disable the select level button.
	m_SelectLevel.EnableWindow( FALSE );

	// Setup the world listctrl.
//	m_Levels.SetImageList( m_pImageList, LVSIL_STATE );

//	sColumnHeader = LoadString( "IDS_SERVERAPP_COLUMNHEADER_WORLD" );
//	m_Levels.InsertColumn( 0, "WORLD" ); // MPW2CT(sColumnHeader).c_str() );
//	m_Levels.SetColumnWidth( 0, LVSCW_AUTOSIZE_USEHEADER );

	// Set the icon.
	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(hIcon, TRUE);
	SetIcon(hIcon, FALSE);


	// All done...
	return(TRUE);
}


/*
// loads localized text into the controls
bool CServerDlg::LoadLocalizedStrings()
{

	UINT pnIDControls[] = {
		IDC_GRPSERVERINFO,
		IDC_LBLSERVERNAME,
		IDC_LBLGAMETYPE,
		IDC_LBLRUNNINGTIME,
		IDC_LBLPEAKPLAYERS,
		IDC_LBLTOTALPLAYERS,
		IDC_STOPSERVER,
		IDC_GRPPLAYERS,
		IDC_GRPMISSIONS,
		IDC_COMMANDS_NEXTLEVEL,
		IDC_COMMANDS_SELLEVEL,
		IDC_GRPCONSOLE,
		IDC_CONSOLE_CLEAR,
		IDC_LBLCURRENTPLAYERS,
		IDC_LBLAVERAGEPING,
		IDC_PLAYER_BOOT,
		IDC_LBLTIMEINLEVEL
	};
	const char* pszStringIDs[] = {
		"IDS_SERVERAPP_GRPSERVERINFO",
		"IDS_SERVERAPP_LBLSERVERNAME",
		"IDS_SERVERAPP_LBLGAMETYPE",
		"IDS_SERVERAPP_LBLRUNNINGTIME",
		"IDS_SERVERAPP_LBLPEAKPLAYERS",
		"IDS_SERVERAPP_LBLTOTALPLAYERS",
		"IDS_SERVERAPP_STOPSERVER",
		"IDS_SERVERAPP_GRPPLAYERS",
		"IDS_SERVERAPP_GRPMISSIONS",
		"IDS_SERVERAPP_NEXTLEVEL",
		"IDS_SERVERAPP_SELECTLEVEL",
		"IDS_SERVERAPP_GRPCONSOLE",
		"IDS_SERVERAPP_CONSOLECLEAR",
		"IDS_SERVERAPP_LBLCURRENTPLAYERS",
		"IDS_SERVERAPP_LBLAVERAGEPING",
		"IDS_SERVERAPP_PLAYERBOOT",
		"IDS_SERVERAPP_LBLTIMEINLEVEL"
	};

	// make sure the number of elements in both arrays match
	if( LTARRAYSIZE(pnIDControls) != LTARRAYSIZE(pszStringIDs) )
	{
		LTERROR( "Array element size mismatch!" );
		return false;
	}

	for(uint32 nControl=0;nControl<LTARRAYSIZE(pnIDControls);++nControl)
	{
		CWnd* pControl = GetDlgItem( pnIDControls[nControl] );
		if( pControl == NULL )
		{
			LTERROR( "Failed to locate control" );
			continue;
		}

		const wchar_t* pwszText = LoadString( pszStringIDs[nControl] );
		pControl->SetWindowText( MPW2CT(pwszText).c_str() );
	}

	const wchar_t* pwszTitle = LoadString( "IDS_SERVERAPP_GAMESERVER_TITLE" );
	SetWindowText( MPW2CT(pwszTitle).c_str() );

	return true;
}
*/

void CServerDlg::OnTimer(UINT nIDEvent) 
{
	UpdateUI();
	
	CDialog::OnTimer(nIDEvent);
}

void CServerDlg::UpdateUI()
{
	int nPlayer;
//	int nCurLevel;
//	CString sCurLevel = "";
	MessageList lstShellMsgs;
	bool bPlayerUpdate;
	bool bLevelChanged = FALSE;
	CString sVal;
	CTimeSpan serverRunTime, levelRunTime;
	CTimeSpan playerOnServerTime;
	int nCurSel, nPlayerId;

	// Make sure server is running.
	if( !IsRunning())
	{
		return;
	}

	// See if shell wants to stop itself.
	if (GetStopEvent().IsSet())
	{
		ShutdownServer();
	}

	// Check if we're in the middle of changing levels.
//	if( m_bChangingLevels_Shell )
//		return;

	// Block the shell thread while we make some copies of its data.
	GetCriticalSection().Enter();
	
	bPlayerUpdate = m_bPlayerUpdate_Shell;
	m_bPlayerUpdate_Shell = FALSE;
//	nCurLevel = GetCurrentMap();
//	sCurLevel = GetCurrentMapName().c_str();
	lstShellMsgs = GetShellMessages();
	ClearShellMessages();
//	bLevelChanged = m_bChangedLevel_Shell;
//	m_bChangedLevel_Shell = FALSE;

	// Unblock the shell thread.
	GetCriticalSection().Leave();

	// Update the server time.
	serverRunTime = CTime::GetCurrentTime( ) - m_serverStartTime;
	if( serverRunTime != m_serverRunTime )
	{
		// Set new time.
		m_serverRunTime = serverRunTime;

		// Choose format for time.
		m_sServerTime = FormatTime( m_serverRunTime );
	}

	// Check if we just changed levels.
//	if (bLevelChanged)
//	{
//		m_levelStartTime = CTime::GetCurrentTime( );
//		m_sTimeInLevel = FormatTime( CTimeSpan( 0 ));
//	}

	// If we're not in a level currently, then there's nothing to update.
//	if (nCurLevel < 0)
//	{
//		return;
//	}

	// Update the level time.
//	levelRunTime = CTime::GetCurrentTime( ) - m_levelStartTime;
//	if (levelRunTime != m_levelRunTime)
//	{
//		m_levelRunTime = levelRunTime;

		// Choose format for time.
//		m_sTimeInLevel = FormatTime( m_levelRunTime );
//	}

	// Get currently selected player id.
	nCurSel = m_Players.GetNextItem( -1, LVNI_SELECTED );
	if (nCurSel != -1)
	{
		nPlayerId = (int) m_Players.GetItemData( nCurSel );
	}
	else
	{
		nPlayerId = -1;
	}

	// Block the server shell thread while we copy the playerinfo data.
	GetCriticalSection().Enter();

	PlayerInfoList lstPlayerInfo = GetPlayers();

	// Unblock the server shell thread.
	GetCriticalSection().Leave();

	// Make sure there are the right number of items in the list control.
	while( m_Players.GetItemCount( ) < int(lstPlayerInfo.size( )) )
	{
		if (m_Players.InsertItem( m_Players.GetItemCount( ), _T("") ) == -1)
		{
			break;
		}
	}
	while( m_Players.GetItemCount( ) > int(lstPlayerInfo.size( )) )
	{
		if (m_Players.DeleteItem( m_Players.GetItemCount( ) - 1 ) == -1)
		{
			break;
		}
	}

	// Clear out the average ping value so we can add to it as we
	// loop over the players.
	m_nAveragePing = 0;

	// Loop over each player in the playerinfo array.  Set the listcontrol 
	// items in a index to index mapping.  This may cause playerinfo data
	// to hop from one index to another, but it makes the update very easy.
	// Since most of the time, nothing is changing, this won't cause any
	// flashing on the control.
	for( nPlayer = 0; nPlayer < m_Players.GetItemCount( ); nPlayer++ )
	{
		// Update any shell information for this slot.
		if (bPlayerUpdate)
		{
			// Set the user data for the list control.
			m_Players.SetItemData( nPlayer, lstPlayerInfo[nPlayer].GetClientId() );

			// If this is the player we were pointing at before, update our
			// selection.
			if( lstPlayerInfo[nPlayer].GetClientId() == nPlayerId )
			{
				m_Players.SetSelectionMark( nPlayer );
			}

			// Update kills.
			sVal.Format( _T("%d"), lstPlayerInfo[nPlayer].GetKills() );
			if( m_Players.GetItemText( nPlayer, ePlayerColumnsKills ).Compare( sVal ))
			{
				m_Players.SetItemText( nPlayer, ePlayerColumnsKills, sVal );
			}

			// Update deaths.
			sVal.Format( _T("%d"), lstPlayerInfo[nPlayer].GetDeaths() );
			if( m_Players.GetItemText( nPlayer, ePlayerColumnsDeaths ).Compare( sVal ))
			{
				m_Players.SetItemText( nPlayer, ePlayerColumnsDeaths, sVal );
			}

			// Update score.
			sVal.Format( _T("%d"), lstPlayerInfo[nPlayer].GetScore() );
			if( m_Players.GetItemText( nPlayer, ePlayerColumnsScore ).Compare( sVal ))
			{
				m_Players.SetItemText( nPlayer, ePlayerColumnsScore, sVal );
			}

			// Update the player name.
			sVal = m_Players.GetItemText( nPlayer, ePlayerColumnsName );
			char const* pszPlayerHandle = lstPlayerInfo[nPlayer].GetPlayerHandle().c_str( );
//			MPA2CT strPlayerHandle(pszPlayerHandle);
			if( strcmp( sVal, pszPlayerHandle ))
			{
				m_Players.SetItemText( nPlayer, ePlayerColumnsName, pszPlayerHandle );
			}
		}

		// Always update the ping regardless of shell update.
		float fPing = 0.0f;
		DWORD nPing;
		GetServerInterface()->GetClientPing( (uint32) m_Players.GetItemData( nPlayer ), fPing );
		nPing = ( DWORD )( fPing + 0.5f );
		m_nAveragePing += nPing;
		sVal.Format( _T("%d"), nPing );
		if( m_Players.GetItemText( nPlayer, ePlayerColumnsPing ).Compare( sVal ))
		{
			m_Players.SetItemText( nPlayer, ePlayerColumnsPing, sVal );
		}

		// Update the player time.
		playerOnServerTime = CTime::GetCurrentTime( ) - CTime(lstPlayerInfo[nPlayer].GetTimeOnServer());

		sVal = FormatTime( playerOnServerTime );
		if( m_Players.GetItemText( nPlayer, ePlayerColumnsTime ).Compare( sVal ))
		{
			m_Players.SetItemText( nPlayer, ePlayerColumnsTime, sVal );
		}
	}

	// Set the number of players.
	if( m_Players.GetItemCount( ) != ( int )m_nGamePlayers )
	{
		m_nGamePlayers = m_Players.GetItemCount( );

		m_sNumPlayers.Format(_T("%d/%d"), m_nGamePlayers, m_nMaxPlayers);

		// Check if this is the most players we've had at one time.
		if( m_nGamePlayers > m_nPeakPlayers )
		{
			m_nPeakPlayers = m_nGamePlayers;
		}
	}

	// Finish calculation of average ping.
	if( m_nGamePlayers > 0 )
	{
		m_nAveragePing = ( DWORD )(( float )m_nAveragePing / m_nGamePlayers + 0.5f );
	}
/*
	// Change selected level.
	if( nCurLevel != m_nCurLevel )
	{
		// Deselect previous level.
		if( 0 <= m_nCurLevel && m_nCurLevel < m_Levels.GetItemCount( ))
		{
			m_Levels.SetItemState( m_nCurLevel, 0, LVIS_STATEIMAGEMASK );
			m_Levels.RedrawItems( m_nCurLevel, m_nCurLevel );
			m_nCurLevel = -1;
		}

		// Update the current level.
		if( 0 <= nCurLevel && nCurLevel < m_Levels.GetItemCount( ))
		{
			m_Levels.SetItemState( nCurLevel, INDEXTOSTATEIMAGEMASK( 1 ), LVIS_STATEIMAGEMASK );
			m_Levels.RedrawItems( nCurLevel, nCurLevel );

			m_nCurLevel = nCurLevel;
		}
	}

	// See if the level name changed.
	if( bLevelChanged && !sCurLevel.IsEmpty( ))
	{
		if( sCurLevel.CompareNoCase( m_Levels.GetItemText( nCurLevel, nCurLevel )))
		{
			m_Levels.SetItemText( nCurLevel, 0, sCurLevel );
		}
	}
*/

	// Update any console strings from the shell.
	for (MessageList::iterator itMessage = lstShellMsgs.begin(); itMessage != lstShellMsgs.end(); ++itMessage)
	{
		WriteConsoleString((*itMessage).c_str() );
	}

	// Send data to dialog.
	UpdateData( FALSE );
}

void CServerDlg::OnServerInit()
{
	// setup the level list in the UI
/*	m_Levels.DeleteAllItems();

	const MapNameList& mapNames = GetMapNames();

	uint8 nNumMissions = mapNames.size();

	for (int nIndex = 0; nIndex < nNumMissions; ++nIndex)
	{
		// Add the mission to the list.
		m_Levels.InsertItem(m_Levels.GetItemCount(), mapNames[nIndex].c_str() );
	}

	m_Levels.SetColumnWidth( 0, LVSCW_AUTOSIZE_USEHEADER );
*/
}

void CServerDlg::OnServerPreAddClient(CPlayerInfo& cPlayerInfo)
{
	// Load the temporary player name.
//	const wchar_t* wszVal = LoadString( "IDS_SERVERAPP_NEWPLAYER" );
//	std::string strValue( MPW2A(wszVal).c_str() );
	cPlayerInfo.SetPlayerHandle( "New Player" ) ; //strValue );
}

void CServerDlg::OnServerPostAddClient(CPlayerInfo& cPlayerInfo)
{
	// Add this player to the total number of visitors.
	m_sTotalPlayers.Format( _T("%d/%d"), GetPlayers().size(), GetUniqueClients().size());

	TRACE("Adding player (%d).\n", cPlayerInfo.GetClientId());

	// Signal to update ui that the player info changed.
	m_bPlayerUpdate_Shell = TRUE;
}

void CServerDlg::OnServerRemovedClient(CPlayerInfo& cPlayerInfo)
{
	// update the totals
	m_sTotalPlayers.Format( _T("%d/%d"), GetPlayers().size(), GetUniqueClients().size());

	// Signal to updateui that the player info changed.
	m_bPlayerUpdate_Shell = TRUE;
}

void CServerDlg::OnServerUpdate()
{
	// Signal to updateui that the player info changed.
	m_bPlayerUpdate_Shell = TRUE;
}

void CServerDlg::OnServerPreLoadWorld()
{
//	m_bChangingLevels_Shell = TRUE;
}

void CServerDlg::OnServerPostLoadWorld()
{
//	m_bChangingLevels_Shell = FALSE;
//	m_bChangedLevel_Shell = TRUE;
}

void CServerDlg::OnServerError(ServerErrorEnum eServerError)
{	
	// convert server code to string table Id
	const char* szStringId = "Net error generic";

	switch (eServerError)
	{
	case DS_ERROR_INITFAILED:
		szStringId = "Net error generic";
		break;
//	case DS_ERROR_NOMAPS:
//		szStringId = "No maps";
//		break;
	case DS_ERROR_OUTOFMEMORY:
		szStringId = "Memory error";
		break;
	case DS_ERROR_UNABLETOHOST:
		szStringId = "Unable to host";
		break;
	case DS_ERROR_BINARIES:
		szStringId = "Couldn't load object.lto";
		break;
	case DS_ERROR_HOSTSESSION:
		szStringId = "Host session error";
		break;
	case DS_ERROR_SELECTSERVICE:
		szStringId = "Network service not found";
		break;
	case DS_ERROR_LOADWORLD:
		szStringId = "Failed loading world";
		break;
	}

	HandleFatalError( szStringId );
}

void CServerDlg::OnConsoleSend() 
{

/*
	if( !GetServerInterface() )
		return;

	// Get the string from control.
	char sCmd[128];
	sCmd[0] = '\0';
	if( GetDlgItemText( IDC_CONSOLE_COMMAND, sCmd, 120 ) == 0 )
		return;

	// Check if this is a scmd command.
/*	if( ScmdConsole::Instance( ).SendCommand( MPA2W(sCmd).c_str() ))
	{
		// Clear out the string from the control.
		SetDlgItemText(IDC_CONSOLE_COMMAND, "");
		return;
	}


	const wchar_t* pwszServer = LoadString( "IDS_SERVERAPP_SERVER" );

	char sMsg[256];
	LTSNPrintF( sMsg, LTARRAYSIZE( sMsg ), "<< %hs >>  %s", pwszServer, sCmd);

	// Update the UI with string.
	WriteConsoleString(sMsg);

	// Clear out the string from the control.
	SetDlgItemText(IDC_CONSOLE_COMMAND, "");

	// Tell the server shell about the console command.
	CAutoMessage cMsg;
	cMsg.Writeuint8( SERVERSHELL_MESSAGE );
	cMsg.WriteString( sCmd );

	CLTMsgRef_Read msgRefRead = cMsg.Read( );

	GetCriticalSection().Enter();
		GetServerInterface()->SendToServerShell( *msgRefRead );
	GetCriticalSection().Leave();
*/

}

void CServerDlg::OnConsoleClear() 
{
	// Clear the console.
	SetDlgItemText(IDC_CONSOLE_WINDOW, "");
}

BOOL CServerDlg::DestroyWindow() 
{
	// We're shutting down.
	ShutdownServer();

	return(CDialog::DestroyWindow());
}

void CServerDlg::HandleFatalError( const char* szStringId )
{
/*
	if( g_pLTIStringKeeper && g_pLTDBStringKeeper )
	{
		AfxMessageBox( MPW2CT(LoadString(szStringId)).c_str() );
	}
	else
*/	{
		CString strValue;
		strValue.Format( _T("A fatal error has occurred.  The application will be shut down.\n\n%s"), szStringId );
		AfxMessageBox( strValue );
	}

	m_bConfirmExit = FALSE;
	SendMessage(WM_COMMAND, IDCANCEL);

	// Call this instead of exit(0), otherwise our destructor won't get called
	::PostQuitMessage(0);
}

void CServerDlg::OnCommandsNextLevel() 
{
/*
	if (!GetServerInterface())
		return;

	// Send next level message to server.
	CAutoMessage cMsg;
	cMsg.Writeuint8( SERVERSHELL_NEXTWORLD );

	CLTMsgRef_Read msgRefRead = cMsg.Read( );

	GetCriticalSection().Enter();
		GetServerInterface()->SendToServerShell(*msgRefRead);
	GetCriticalSection().Leave();
*/

}

void CServerDlg::OnCommandsSelectLevel() 
{
	if (!IsRunning())
	{
		return;
	}

/*
	// Get the selected item.
	POSITION pos = m_Levels.GetFirstSelectedItemPosition( );
	if (!pos)
	{
		return;
	}

	// Go to next item.
	int nWorldIndex = m_Levels.GetNextSelectedItem( pos );
	SelectLevel( nWorldIndex );
*/

}


bool CServerDlg::SelectLevel( int nLevelIndex ) 
{

/*
	// Check if we're running.
	if (!IsRunning())
	{
		return FALSE;
	}

	// Create a message.
	CAutoMessage cMsg;
	cMsg.Writeuint8( SERVERSHELL_SETWORLD );
	cMsg.Writeuint32( nLevelIndex );

	// Send message to the server.
	CLTMsgRef_Read msgRefRead = cMsg.Read( );

	GetCriticalSection().Enter();
	GetServerInterface()->SendToServerShell(*msgRefRead);
	GetCriticalSection().Leave();
*/
	return TRUE;

}


void CServerDlg::OnPlayersBoot() 
{
/*
	if (!GetServerInterface())
	{
		return;
	}

	// Get the player selected.
	int nCurSel = m_Players.GetNextItem( -1, LVNI_SELECTED );

	if( nCurSel == -1 )
	{
		return;
	}

	// Get the player's id.
	DWORD dwID = m_Players.GetItemData( nCurSel );

	// Boot that player.
	GetCriticalSection().Enter();
	GetServerInterface()->BootClient(dwID);
	GetCriticalSection().Leave();
*/
}

void CServerDlg::OnCancel() 
{
	// Ask the user to confirm stopping server.
	if (m_bConfirmExit)
	{
		if (!ConfirmStop())
		{
			return;
		}
	}

	// User really wants to stop the server.
	ShutdownServer();

	CDialog::OnCancel();
}



bool CServerDlg::GetResourceFiles(ResourceFileList& cResourceFiles)
{
	CWaitCursor wc;
	CStringList lstRez;
	CString sRezKey;
	CString sRez;
	POSITION pos;

	// Add the stock rez files.

	lstRez.AddTail( "rez" );
//	lstRez.AddTail( "game" );
//	lstRez.AddTail( "game.rez" );

	// Now add them to the array to pass to the server.
	pos = lstRez.GetHeadPosition( );

	while (pos)
	{
		CString& rez = lstRez.GetNext(pos);
		cResourceFiles.push_back((char*)(char const*)rez);
	}

	return true;
}

bool CServerDlg::ConfirmStop( )
{
	// Make sure server is really running.
	if (!IsLoaded())
	{
		return true;
	}

	return (MessageBox("Stop Server", "Seal Hunter", MB_YESNO | MB_ICONQUESTION) == IDYES);
}


void CServerDlg::OnDblclkLevels(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	POSITION pos = m_Levels.GetFirstSelectedItemPosition( );
	if( !pos )
		return;

	int nWorldIndex = m_Levels.GetNextSelectedItem( pos );
	SelectLevel( nWorldIndex );

	*pResult = 0;
}

void CServerDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	// On the first show, startup the server.
	if (bShow && m_bFirstShow)
	{
		// Only count the first show.
		m_bFirstShow = FALSE;
		
		// Now that we've been shown, we can do our startup.
		PostMessage(WM_COMMAND, WPARAM( IDC_STARTUP));
	}	
}

void CServerDlg::OnStartup( )
{
	// Run the server.
	if (!RunServer())
	{
		HandleFatalError("IDS_SERVERAPP_ERROR_CANTSTARTSERVER");
		return;
	}

	// Update our controls.
	UpdateData(FALSE);

	// hide the splash screen
	m_wndSplash.HideSplashScreen();
}

void CServerDlg::OnStopserver() 
{
	OnCancel();		
}

// called when the dialog window is destroyed
void CServerDlg::OnDestroy()
{
}


CString CServerDlg::FormatTime( CTimeSpan const& timeSpan )
{
	CString sTime;

	if (timeSpan.GetDays() > 0)
	{
		sTime = timeSpan.Format( _T("%Dd:%Hh:%Mm:%Ss") );
	}
	else if (timeSpan.GetHours() > 0)
	{
		sTime = timeSpan.Format( _T("%Hh:%Mm:%Ss") );
	}
	else
	{
		sTime = timeSpan.Format( _T("%Mm:%Ss") );
	}

	return sTime;
}

BOOL CServerDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( m_wndSplash.PreTranslateAppMessage(pMsg) )
		return TRUE;
	  
	return CDialog::PreTranslateMessage(pMsg);
}


void CServerDlg::OnItemchangedLevels(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	// Enable the select level button only when a level is selected.
	POSITION pos = m_Levels.GetFirstSelectedItemPosition( );
	m_SelectLevel.EnableWindow( pos != NULL );

	*pResult = 0;
}

void CServerDlg::OnItemchangedPlayers(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	// Enable the boot player button only when a player is selected.
	POSITION pos = m_Players.GetFirstSelectedItemPosition();
	m_PlayerBoot.EnableWindow(pos != NULL);
	
	*pResult = 0;
}
