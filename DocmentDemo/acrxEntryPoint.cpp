// (C) Copyright 2002-2012 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "docman/docman.h"
//-----------------------------------------------------------------------------
#define szRDS _RXST("")


// Global pointers to our two reactor classes
AsdkDocReactor*      gpDocReactor = NULL ;      // MDI Safe
AsdkEditorReactor*   gpEditReactor = NULL;      // MDI Safe
AsdkDocToModReactor* gpDocToModReactor = NULL;  // MDI Safe

// the following global will point to our modeless dialog
CMDITestDialog*      modelessDlg = NULL; // MDI Safe


//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CDocmentDemoApp : public AcRxArxApp {

public:
	CDocmentDemoApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here
		// This application can be unloaded
		acrxDynamicLinker->unlockApplication(pkt);
		// This is an MDI aware application
		acrxDynamicLinker->registerAppMDIAware(pkt);
		initApp();

		// add the reactor once the dialog was created
		gpDocToModReactor =new AsdkDocToModReactor();

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here
		unloadApp();

		delete gpDocToModReactor;
		gpDocToModReactor = NULL;

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}
	
	// The ACED_ARXCOMMAND_ENTRY_AUTO macro can be applied to any static member 
	// function of the CDocmentDemoApp class.
	// The function should take no arguments and return nothing.
	//
	// NOTE: ACED_ARXCOMMAND_ENTRY_AUTO has overloads where you can provide resourceid and
	// have arguments to define context and command mechanism.
	
	// ACED_ARXCOMMAND_ENTRY_AUTO(classname, group, globCmd, locCmd, cmdFlags, UIContext)
	// ACED_ARXCOMMAND_ENTRYBYID_AUTO(classname, group, globCmd, locCmdId, cmdFlags, UIContext)
	// only differs that it creates a localized name using a string in the resource file
	//   locCmdId - resource ID for localized command

	// Modal Command with localized name
	// ACED_ARXCOMMAND_ENTRY_AUTO(CDocmentDemoApp, MyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL)
	static void MyGroupMyCommand () {
		// Put your command code here

	}

	// Modal Command with pickfirst selection
	// ACED_ARXCOMMAND_ENTRY_AUTO(CDocmentDemoApp, MyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET)
	static void MyGroupMyPickFirst () {
		ads_name result ;
		int iRet =acedSSGet (ACRX_T("_I"), NULL, NULL, NULL, result) ;
		if ( iRet == RTNORM )
		{
			// There are selected entities
			// Put your command using pickfirst set code here
		}
		else
		{
			// There are no selected entities
			// Put your command code here
		}
	}

	// Application Session Command with localized name
	// ACED_ARXCOMMAND_ENTRY_AUTO(CDocmentDemoApp, MyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION)
	static void MyGroupMySessionCmd () {
		// Put your command code here
	}

	// The ACED_ADSFUNCTION_ENTRY_AUTO / ACED_ADSCOMMAND_ENTRY_AUTO macros can be applied to any static member 
	// function of the CDocmentDemoApp class.
	// The function may or may not take arguments and have to return RTNORM, RTERROR, RTCAN, RTFAIL, RTREJ to AutoCAD, but use
	// acedRetNil, acedRetT, acedRetVoid, acedRetInt, acedRetReal, acedRetStr, acedRetPoint, acedRetName, acedRetList, acedRetVal to return
	// a value to the Lisp interpreter.
	//
	// NOTE: ACED_ADSFUNCTION_ENTRY_AUTO / ACED_ADSCOMMAND_ENTRY_AUTO has overloads where you can provide resourceid.
	
	//- ACED_ADSFUNCTION_ENTRY_AUTO(classname, name, regFunc) - this example
	//- ACED_ADSSYMBOL_ENTRYBYID_AUTO(classname, name, nameId, regFunc) - only differs that it creates a localized name using a string in the resource file
	//- ACED_ADSCOMMAND_ENTRY_AUTO(classname, name, regFunc) - a Lisp command (prefix C:)
	//- ACED_ADSCOMMAND_ENTRYBYID_AUTO(classname, name, nameId, regFunc) - only differs that it creates a localized name using a string in the resource file

	// Lisp Function is similar to ARX Command but it creates a lisp 
	// callable function. Many return types are supported not just string
	// or integer.
	// ACED_ADSFUNCTION_ENTRY_AUTO(CDocmentDemoApp, MyLispFunction, false)
	static int ads_MyLispFunction () {
		//struct resbuf *args =acedGetArgs () ;
		
		// Put your command code here

		//acutRelRb (args) ;
		
		// Return a value to the AutoCAD Lisp Interpreter
		// acedRetNil, acedRetT, acedRetVoid, acedRetInt, acedRetReal, acedRetStr, acedRetPoint, acedRetName, acedRetList, acedRetVal

		return (RTNORM) ;
	}
	
	void
		initApp()
	{
		acedRegCmds->addCommand(_T(/*NOXLATE*/"ASDK_DOCUMENTMANAGER"),
			_T(/*NOXLATE*/"ASDK_LISTDOCS"),
			_T("LISTDOCS"),
			ACRX_CMD_MODAL,
			listDocuments);

		acedRegCmds->addCommand(_T(/*NOXLATE*/"ASDK_DOCUMENTMANAGER"),
			_T(/*NOXLATE*/"ASDK_WATCHDOCS"),
			_T("WATCHDOCS"),
			ACRX_CMD_MODAL,
			watchDocuments);


		acedRegCmds->addCommand(_T(/*NOXLATE*/"ASDK_DOCUMENTMANAGER"),
			_T(/*NOXLATE*/"ASDK_SEND"),
			_T("send"),
			ACRX_CMD_MODAL | ACRX_CMD_NONEWSTACK ,
			send);

		acedRegCmds->addCommand(_T(/*NOXLATE*/"ASDK_DOCUMENTMANAGER"),
			_T(/*NOXLATE*/"ASDK_ACTIVATE"),
			_T("activate"),
			ACRX_CMD_MODAL | ACRX_CMD_NONEWSTACK ,
			activate);

		acedRegCmds->addCommand(_T(/*NOXLATE*/"ASDK_DOCUMENTMANAGER"),
			_T(/*NOXLATE*/"ASDK_NEWDOC"),
			_T("newdoc"),
			ACRX_CMD_MODAL ,
			newdoc);

		acedRegCmds->addCommand(_T(/*NOXLATE*/"ASDK_DOCUMENTMANAGER"),
			_T(/*NOXLATE*/"ASDK_OPENDOC"),
			_T("opendoc"),
			ACRX_CMD_MODAL | ACRX_CMD_NONEWSTACK ,
			opendoc);

		acedRegCmds->addCommand(_T(/*NOXLATE*/"ASDK_DOCUMENTMANAGER"),
			_T(/*NOXLATE*/"ASDK_CLOSEDOC"),
			_T("closedoc"),
			ACRX_CMD_MODAL | ACRX_CMD_NONEWSTACK ,
			closedoc);

		acedRegCmds->addCommand(_T(/*NOXLATE*/"ASDK_DOCUMENTMANAGER"),
			_T(/*NOXLATE*/"ASDK_CMD"),
			_T("cmd"),
			ACRX_CMD_MODAL ,
			cmd);

		acedRegCmds->addCommand(_T(/*NOXLATE*/"ASDK_DOCUMENTMANAGER"),
			_T(/*NOXLATE*/"ASDK_TOGGLE"),
			_T("toggle"),
			ACRX_CMD_MODAL | ACRX_CMD_NONEWSTACK,
			toggleActivate);

		acedRegCmds->addCommand(_T(/*NOXLATE*/"ASDK_DOCUMENTMANAGER"),
			_T(/*NOXLATE*/"ASDK_APPPROC"),
			_T("appcontext"),
			ACRX_CMD_MODAL | ACRX_CMD_NONEWSTACK,
			appContext);

		acedRegCmds->addCommand(_T(/*NOXLATE*/"ASDK_DOCUMENTMANAGER"),
			_T(/*NOXLATE*/"ASDK_NEWSYNCDOC"),
			_T("newsyncdoc"),
			ACRX_CMD_MODAL ,
			newSyncDoc);

		acedRegCmds->addCommand(_T(/*NOXLATE*/"ASDK_DOCUMENTMANAGER"),
			_T(/*NOXLATE*/"ASDK_OPENSYNCDOC"),
			_T("opensyncdoc"),
			ACRX_CMD_MODAL ,
			openSyncDoc);

		acedRegCmds->addCommand(_T(/*NOXLATE*/"ASDK_DOCUMENTMANAGER"),
			_T(/*NOXLATE*/"ASDK_TESTDLG"),
			_T("testdlg"),
			ACRX_CMD_MODAL ,
			testdlg,
			NULL,
			-1,
			GetModuleInstance());
	}

	void
		unloadApp()
	{
		acedRegCmds->removeGroup(_T(/*NOXLATE*/"ASDK_DOCUMENTMANAGER"));

		// destroy modeless dialog if displayed
		endDlg() ;

		delete gpDocReactor;
		gpDocReactor = NULL;
		delete gpEditReactor;
		gpEditReactor = NULL;
	}

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CDocmentDemoApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CDocmentDemoApp, MyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CDocmentDemoApp, MyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CDocmentDemoApp, MyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION, NULL)
ACED_ADSSYMBOL_ENTRY_AUTO(CDocmentDemoApp, MyLispFunction, false)
