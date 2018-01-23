// (C) Copyright 1997-2007 by Autodesk, Inc.
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
// docman.cpp:  MDI Document Manager API Examples
#include "stdafx.h"

#if defined(_DEBUG) && !defined(AC_FULL_DEBUG)
#error _DEBUG should not be defined except in internal Adesk debug builds
#endif

#include <string.h>
#include "adesk.h"
#include "aced.h"
#include "acdocman.h"
#include "adslib.h"
#include "actrans.h"
#include "rxmfcapi.h"
#include "AcExtensionModule.h"

#include "MDITestDialog.h"
#include "common.h"

#include "acedads.h"
#include "acedCmdNF.h"


#define acedCommand acedCommandS
#define acedCmd acedCmdS

class AsdkDocReactor;
class AsdkEditorReactor;
class AsdkDocToModReactor;


//// Global pointers to our two reactor classes
//AsdkDocReactor*      gpDocReactor = NULL ;      // MDI Safe
//AsdkEditorReactor*   gpEditReactor = NULL;      // MDI Safe
//AsdkDocToModReactor* gpDocToModReactor = NULL;  // MDI Safe
//
//// the following global will point to our modeless dialog
//CMDITestDialog*      modelessDlg = NULL; // MDI Safe
//
//
//extern "C"
//	{
//		#include "acedads.h"
//	}
//
//AcApDocument* AcadSetActiveDocument(AcApDocument* pAcDocument);


/////////////////////////////////////////////////////////////////////////////
// Define the sole extension module object.

//AC_IMPLEMENT_EXTENSION_MODULE(docmanDLL);


/////////////////////////////////////////////////////////////////////////
// The Document Reactor ...
// Subscribe to all of the document reactors the example simply monitors
// and reports when any of the following document reactors are fired.
/////////////////////////////////////////////////////////////////////////
class AsdkDocReactor : public AcApDocManagerReactor
{
public:
	AsdkDocReactor ();
	~AsdkDocReactor ();

    virtual void  documentCreated(AcApDocument* );
    virtual void  documentToBeDestroyed(AcApDocument* );
    virtual void  documentLockModeChanged(AcApDocument*,
                                          AcAp::DocLockMode,
                                          AcAp::DocLockMode,
                                          AcAp::DocLockMode,
                                          const TCHAR* pCommandName);
    virtual void  documentBecameCurrent(AcApDocument* );

    virtual void  documentToBeActivated(AcApDocument* pActivatingDoc );
    virtual void  documentToBeDeactivated(AcApDocument* pDeactivatingDoc );
	virtual void  documentActivationModified(bool bActivation);
};

// Simple AcEd reactor declaration. Notify when a save is complete.
// This is the typical method to look when a documents name might have changed
class AsdkEditorReactor : public AcEditorReactor
{
public:
	AsdkEditorReactor ();
	~AsdkEditorReactor ();

    virtual void saveComplete (AcDbDatabase*, const TCHAR* pActualName);
};




/////////////////////////////////////////////////////////////////////////
// The Document To Modeless Dialog Reactor ...
// Subscribe to all of the document reactors. The example simply monitors
// and reports when any of the following document reactors are fired.
// However this reactor is tailored to interact with the modeless dialog,
// sending messages and updating it when reactors are fired.
// Unlike the other reactors, this one will be attached/detached on
// creation/destruction of the dialog itself.
/////////////////////////////////////////////////////////////////////////
class AsdkDocToModReactor : public AcApDocManagerReactor
{
public:
	AsdkDocToModReactor();
	~AsdkDocToModReactor();

    virtual void  documentCreated(AcApDocument* );//≤‚ ‘
    virtual void  documentToBeDestroyed(AcApDocument* );
    virtual void  documentLockModeChanged(AcApDocument*,
                                          AcAp::DocLockMode,
                                          AcAp::DocLockMode,
                                          AcAp::DocLockMode,
                                          const TCHAR* pCommandName);
	virtual void  documentLockModeWillChange(AcApDocument*,
                                          AcAp::DocLockMode,
                                          AcAp::DocLockMode,
                                          AcAp::DocLockMode,
                                          const TCHAR* pGlobalCmdName);
    virtual void  documentBecameCurrent(AcApDocument* );

    virtual void  documentToBeActivated(AcApDocument* pActivatingDoc );
    virtual void  documentToBeDeactivated(AcApDocument* pDeactivatingDoc );
	virtual void  documentActivationModified(bool bActivation);

	//////////////////////////////////////////////////////////////////////////

	static void CloseDocInContext(void *pData);

	static void CloseDoc(void *pData );

};


// Commands ...

//
//  NAME: getYorN        
//
//  REMARKS: Simple Yes or No utility prompt
//           
//          
// 
//  RETURNS:
//      TRUE for Yes False for No 
static Adesk::Boolean 
getYorN(const TCHAR* pStr);

//
//	startDlg()
//
//	Start up the modeless dialog.
//
//
Adesk::Boolean startDlg()
;

Adesk::Boolean endDlg();


//
//  NAME: listDocuments        
//
//  REMARKS: Iterate over all the open documents. Very common piece of code
//           for MDI aware applications.
//          
// 
//  RETURNS:
//      void 
void
listDocuments()
;
//
//  NAME: watchDocuments        
//
//  REMARKS:  Turn on all reactor notifications
//          
//          
// 
//  RETURNS:
//      void 
void
watchDocuments()
;
//
//  NAME: selectDocument        
//
//  REMARKS:  Simple utility to have the user choose an open document to
//            perform some action on.
//          
//          
// 
//  RETURNS:
//      void 
static AcApDocument* selectDocument()
;
//
//  NAME: activate        
//
//  REMARKS:  Simple utility to have the user choose an open document to
//            be activated programmatically. 
//            NOTE: This and other APIs that change context (switch from one
//            active document to another) have the effect of suspending the current
//            document context. What this implies is if the ARX application
//            had code immediately following the activateDocument() API call
//            and this command was executed from a current document (ie. NOT
//            from the application context) The code following the API
//            call would never execute until that document became activated
//            again.
//          
//          
// 
//  RETURNS:
//      void 
void
activate()
;
//
//  NAME: send        
//
//  REMARKS:  Simple routine to exercise switching between documents and sending commands.
//            
//         NOTEs: The actual signature for sendStringToExecute is:
//                virtual Acad::ErrorStatus sendStringToExecute(
//                                       AcApDocument* pAcTargetDocument,
//                                       const char * pszExecute,
//                                       bool bActivate = true,
//                                       bool bWrapUpInactiveDoc = false) = 0;
//
//         By default this API activates the document you are sending the string to
//         to be executed to. The last parameter, bWrapUpInactiveDoc is only applicable
//         if bActivate == False. This special ability allows you to "clean up" 
//         a document you might have left with some dangling command when constructing
//         a command that spans documents. One scenario is you are in the middle of
//         a custom command whose implementation is designed to "follow" the user
//         as they move from one document to another. If you have registered a reactor
//         on documentToBeActivated(). When this reactor fires you wish to complete
//         your current command in the document that is being deactivated and start
//         a command in the document about to be activated. 
//         Given these conditions, you would possibly send a "return" to the old 
//         document and "execute my new command" string to the one about to activated.
//         The "return" you would want to process in the background so you would pass
//         bActivate = False and bWrapUpInactiveDoc = True. 
// 
//  RETURNS:
//      void 
// 
void
send()
;
//
//  NAME: cmd()        
//
//  REMARKS:  Simple routine to exercise acedCommand. While this still works in MDI
//            it will ALWAYS apply to the active document. You might find
//            sendStringToExecute to be more modern and robust.
//            Also, sendStringToExecute allows you to send strings to sub prompts
//            something not possible from acedCommand where the entire command
//            sequence must be explicit.
//            
//          
//          
// 
//  RETURNS:
//      void 
void
cmd()
;
//  NAME: toggleActivate()        
//
//  REMARKS:  Simple routine to exercise toggling activation on and off
//            
//            
//            NOTEs: This is a very powerful API you are completly 
//            circumventing the users ability to switch between documents
//            BE VERY SURE YOU ALWAYS RE-ENABLE ACTIVATION.
//            Given that warning it can also be very useful for porting old
//            non-reentrant commands. If you have some command that performs
//            perhaps a modal action like file i/o and you know the user should
//            never switch documents during its operation you can bracket
//            the call with these APIs.
//          
// 
//  RETURNS:
//      void 

void
toggleActivate()
;


//  NAME: newdoc()        
//
//  REMARKS:  Simple routine to exercise creating new document in MDI using standard user interface
//            This API is functionally equivalent to an acedCommand of the NEW command,
//            but uses the standard Windows messaging mechanism.
//            (  PostMessage(hMainWnd, WM_COMMAND, ID_FILE_NEW, 0); )
//            It is cleaner  and will handle more situations like command already in progress
//            and you do not have to worry about nesting of acedCommand if you use this API.
//            
//            NOTEs: See the same caveats as activateDocument() example
//          
//          
// 
//  RETURNS:
//      void 
// 
void
newdoc()
;

//  NAME: opendoc()        
//
//  REMARKS:  Simple routine to exercise opening a document in MDI using standard user interface
//            This API is functionally equivalent to an acedCommand of the OPEN command,
//            but uses the standard Windows messaging mechanism.
//            (  PostMessage(hMainWnd, WM_COMMAND, ID_FILE_OPEN, 0); )
//            It is cleaner  and will handle more situations like command already in progress
//            and you do not have to worry about nesting of acedCommand if you use this API.
//            
//            NOTEs: See the same caveats as activateDocument() example
//          
// 
//  RETURNS:
//      void 
// 
void
opendoc()
;
//  NAME: closedoc()        
//
//  REMARKS:  Simple routine to exercise closing a document in MDI using standard user interface
//            This API is functionally equivalent to an acedCommand of the OPEN command,
//            but uses the standard Windows messaging mechanism.
//            (  PostMessage(hMainWnd, WM_CLOSE, 0, 0); )
//            It is cleaner  and will handle more situations like command already in progress
//            and you do not have to worry about nesting of acedCommand if you use this API.
//            
//            NOTEs: See the same caveats as activateDocument() example but realize in this case
//            if the CLOSE completes obviously the calling routine will never be returned to.
// 
//  RETURNS:
//      void 
// 
void
closedoc()
;

//  NAME: inAppContext()        
//
//  REMARKS:  Simple callback function to be executed in the application context.
//            
//              NOTES: The application context is unique from each document. It is the supervisor
//              that delegates between each document that is currently opened. Mode-less dialogs
//              and other floating dialogs execute in this domain. As such it is the only
//              context where a document can be created that will not be suspended when the new document
//              becomes active. In the previous OPEN and NEW APIs it was explained that once
//              suspended a line of code following those APIs would not be executed until that 
//              document is activated again. For advanced operations a developer may require the
//              ability to execute a routine from within the applciation context to overcome
//              this limitation. As such, executeInApplicationContext() and its companion funcitons
//              appContextOpenDocument() and appContextNewDocument() were provided.
//          
// 
//  RETURNS:
//      void 
// 
void 
inAppContext( void *pData)
;
//  NAME: appcontext()        
//
//  REMARKS:  Simple function which requests a routine to be executed via the application context.
//
//
//              NOTEs: See explanation for inAppContext()
//  RETURNS:
//      void 
// 
void
appContext()
;

//  NAME: openSyncDocHelper()        
//
//  REMARKS:  Simple callback function to be executed in the application context.
//              Demonstrates opening a document synchronously to retain control
//              in caller function
//
//              NOTEs: See explanation for inAppContext()
//  RETURNS:
//      void 
// 
void 
openSyncDocHelper( void *pData)
;
//  NAME: openSyncDoc()        
//
//  REMARKS:  Simple function which requests a OPEN to be executed via the application context.
//
//
//              NOTEs: See explanation for inAppContext()
//  RETURNS:
//      void 
// 
void
openSyncDoc()
;
//  NAME: newSyncDocHelper()        
//
//  REMARKS:  Simple callback function to be executed in the application context.
//              Demonstrates creating a new document synchronously to retain control
//              in caller function
//
//              NOTEs: See explanation for inAppContext()
//  RETURNS:
//      void 
// 
void 
newSyncDocHelper( void *pData)
;
//  NAME: newSyncDoc()        
//
//  REMARKS:  Simple function which requests a NEW to be executed via the application context.
//
//
//              NOTEs: See explanation for inAppContext()
//  RETURNS:
//      void 
// 
void
newSyncDoc()
;

//  NAME: testdlg()        
//
//  REMARKS:  creates a modeless dialog which calls several API functions
//            (from the application context & document context)
//
//
//              NOTEs: 
//  RETURNS:
//      void 
// 
void testdlg()
;



