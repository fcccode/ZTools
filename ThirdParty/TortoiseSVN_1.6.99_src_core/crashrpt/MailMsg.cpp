///////////////////////////////////////////////////////////////////////////////
//
//  Module: MailMsg.cpp
//
//    Desc: See MailMsg.h
//
// Copyright (c) 2003 Michael Carruth
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MailMsg.h"
#include "auto_buffer.h"

CMailMsg::CMailMsg()
{
   m_lpCmcLogon      = NULL;
   m_lpCmcSend       = NULL;
   m_lpCmcLogoff     = NULL;
   m_lpMapiLogon     = NULL;
   m_lpMapiSendMail  = NULL;
   m_lpMapiLogoff    = NULL;
   m_lpMapiResolveName = NULL;
   m_lpMapiFreeBuffer = NULL;
   m_bReady          = FALSE;
}

CMailMsg::~CMailMsg()
{
   if (m_bReady)
      Uninitialize();
}

CMailMsg& CMailMsg::SetFrom(string sAddress, string sName)
{
   if (initIfNeeded())
   {
      // only one sender allowed
      if (m_from.size())
         m_from.empty();

	  m_from.push_back(TStrStrPair(sAddress,sName));
   }

   return *this;
}

CMailMsg& CMailMsg::SetTo(string sAddress, string sName)
{
   if (initIfNeeded())
   {
      // only one recipient allowed
      if (m_to.size())
         m_to.empty();

	  m_to.push_back(TStrStrPair(sAddress,sName));
   }

   return *this;
}

CMailMsg& CMailMsg::SetCc(string sAddress, string sName)
{
   if (initIfNeeded())
   {
      m_cc.push_back(TStrStrPair(sAddress,sName));
   }

   return *this;
}

CMailMsg& CMailMsg::SetBc(string sAddress, string sName)
{
   if (initIfNeeded())
   {
	   m_bcc.push_back(TStrStrPair(sAddress, sName));
   }

   return *this;
}

CMailMsg& CMailMsg::AddAttachment(string sAttachment, string sTitle)
{
   if (initIfNeeded())
   {
      m_attachments.push_back(TStrStrPair(sAttachment, sTitle));
   }

   return *this;
}

BOOL CMailMsg::Send()
{
   // try mapi
   const int status = MAPISend();
   if (status != 0)
      return status == 1;
   // try cmc
//   if (CMCSend())
//      return TRUE;

   return FALSE;
}

/*
+------------------------------------------------------------------------------
|
|	Function:	cResolveName()
|
|	Parameters:	[IN]	lpszName = Name of e-mail recipient to resolve.
|				[OUT]	ppRecips = Pointer to a pointer to an lpMapiRecipDesc
|
|	Purpose:	Resolves an e-mail address and returns a pointer to a 
|				MapiRecipDesc structure filled with the recipient information
|				contained in the address book.
|
|	Note:		ppRecips is allocated off the heap using MAPIAllocateBuffer.
|				Any user of this method must be sure to release ppRecips when 
|				done with it using either MAPIFreeBuffer or cFreeBuffer.
+-------------------------------------------------------------------------------
*/
int CMailMsg::cResolveName( LHANDLE m_lhSession, const char * lpszName, lpMapiRecipDesc *ppRecip )
{	
	// Always check to make sure there is an active session
	if (!m_lhSession)
	{
		return E_FAIL;
	}

	FLAGS flFlags = 0L;
	ULONG ulReserved = 0L;
	lpMapiRecipDesc pRecips = NULL;

	const HRESULT hRes = m_lpMapiResolveName (
							     m_lhSession,	// Session handle
								 0L,			// Parent window.
								 const_cast<LPSTR>(lpszName),		// Name of recipient.  Passed in by argv.
								 flFlags,		// Flags set to 0 for MAPIResolveName.
								 ulReserved,
								 &pRecips
								 );				

	if ( hRes == SUCCESS_SUCCESS )
	{  
		// Copy the recipient descriptor returned from MAPIResolveName to 
		// the out parameter for this function,
		*ppRecip = pRecips;
	}  
	return hRes;
}

int CMailMsg::MAPISend()
{
	if (!initIfNeeded())
	{
		return 1;
	}

   TStrStrVector::iterator p;
   int                  nIndex = 0;
   MapiRecipDesc*       pOriginator = NULL;
   MapiRecipDesc*       pFirstRecipient = NULL;
   MapiMessage          message;
   std::vector<MapiRecipDesc*>	buffersToFree;
   MapiRecipDesc*       pRecip;
   MapiRecipDesc		grecip;


	LHANDLE hMapiSession;
	ULONG status = m_lpMapiLogon(NULL, NULL, NULL, MAPI_NEW_SESSION | MAPI_LOGON_UI, 0, &hMapiSession);
	if (SUCCESS_SUCCESS != status) {
		return FALSE;
	}

	const size_t nRecipients = m_to.size() + m_cc.size() + m_bcc.size() + m_from.size();
	auto_buffer<MapiRecipDesc> pRecipients(nRecipients);
	if (nRecipients)
	{
		memset(pRecipients, 0, nRecipients * sizeof  MapiRecipDesc);
	}

	const ULONG nAttachments = (ULONG)m_attachments.size();
	auto_buffer<MapiFileDesc> pAttachments(nAttachments);

	if (pRecipients)
	{
		pFirstRecipient = pRecipients;
		if (m_from.size())
		{
			// set from
			if (cResolveName(hMapiSession, m_from.begin()->first.c_str(), &pOriginator) == SUCCESS_SUCCESS) {
				buffersToFree.push_back(pOriginator);
			}
		}
		if (m_to.size())
		{
			if (cResolveName(hMapiSession, m_to.begin()->first.c_str(), &pRecip) == SUCCESS_SUCCESS) {
				if (pFirstRecipient == NULL)
					pFirstRecipient = &pRecipients[nIndex];
				pRecip->ulRecipClass = MAPI_TO;
				memcpy(&pRecipients[nIndex], pRecip, sizeof pRecipients[nIndex]);
				buffersToFree.push_back(pRecip);
			}
			else
			{
				if (pFirstRecipient == NULL)
					pFirstRecipient = &pRecipients[nIndex];
				grecip.ulRecipClass = MAPI_TO;
				grecip.lpEntryID = 0;
				grecip.lpszName = 0;
				grecip.ulEIDSize = 0;
				grecip.ulReserved = 0;
				grecip.lpszAddress = (LPTSTR)(LPCTSTR)m_to.begin()->first.c_str();
				memcpy(&pRecipients[nIndex], &grecip, sizeof pRecipients[nIndex]);
			}
			nIndex++;
		}		
		if (m_cc.size())
		{
			// set cc's
			for (p = m_cc.begin(); p != m_cc.end(); p++, nIndex++)
			{
				if ( cResolveName(hMapiSession, p->first.c_str(), &pRecip) == SUCCESS_SUCCESS) {
					if (pFirstRecipient == NULL)
						pFirstRecipient = &pRecipients[nIndex];
					pRecip->ulRecipClass = MAPI_CC;
					memcpy(&pRecipients[nIndex], pRecip, sizeof pRecipients[nIndex]);
					buffersToFree.push_back(pRecip);
					nIndex++;
				}
            }
		}

		if (m_bcc.size())
		{
			// set bcc
			for (p = m_bcc.begin(); p != m_bcc.end(); p++, nIndex++)
            {
				if ( cResolveName(hMapiSession, p->first.c_str(), &pRecip) == SUCCESS_SUCCESS) {
					if (pFirstRecipient == NULL)
						pFirstRecipient = &pRecipients[nIndex];
					pRecip->ulRecipClass = MAPI_BCC;
					memcpy(&pRecipients[nIndex], pRecip, sizeof pRecipients[nIndex]);
					buffersToFree.push_back(pRecip);
					nIndex++;
				}
            }
		}
	}
	if (pAttachments)
	{
		// add attachments
		for (p = m_attachments.begin(), nIndex = 0;
			p != m_attachments.end(); p++, nIndex++)
		{
			pAttachments[nIndex].ulReserved        = 0;
            pAttachments[nIndex].flFlags           = 0;
            pAttachments[nIndex].nPosition         = 0;
            pAttachments[nIndex].lpszPathName      = (LPTSTR)p->first.c_str();
            pAttachments[nIndex].lpszFileName      = (LPTSTR)p->second.c_str();
            pAttachments[nIndex].lpFileType        = NULL;
		}
	}
	memset(&message, 0, sizeof message);
	message.ulReserved = 0;
	if (m_sSubject.empty())
		message.lpszSubject = "No Subject";
	else
		message.lpszSubject = (LPTSTR)m_sSubject.c_str();		
	if (m_sMessage.empty())
		message.lpszNoteText = "No Message Body";
	else
		message.lpszNoteText = (LPTSTR)m_sMessage.c_str();

	message.lpszMessageType                   = NULL;
	message.lpszDateReceived                  = NULL;
	message.lpszConversationID                = NULL;
	message.flFlags                           = 0;
	message.lpOriginator                      = pOriginator;
	message.nRecipCount                       = nIndex;
	message.lpRecips                          = pFirstRecipient;
	message.nFileCount                        = nAttachments;
	message.lpFiles                           = pAttachments;

	status = m_lpMapiSendMail(hMapiSession, 0, &message, MAPI_DIALOG, 0);
	m_lpMapiLogoff(hMapiSession, NULL, 0, 0);

	std::vector<MapiRecipDesc*>::iterator iter;
	for (iter = buffersToFree.begin(); iter != buffersToFree.end(); iter++) {
		m_lpMapiFreeBuffer(*iter);
	}

	if (SUCCESS_SUCCESS == status) {
		return 1;
	}

	string txt;
	TCHAR buf[MAX_PATH];
	_tprintf_s(buf, "Message did not get sent due to error code %d.\r\n", status);
	txt = buf;
	switch (status)
	{  
	case MAPI_E_AMBIGUOUS_RECIPIENT:
		txt += "A recipient matched more than one of the recipient descriptor structures and MAPI_DIALOG was not set. No message was sent.\r\n" ;
		break;
	case MAPI_E_ATTACHMENT_NOT_FOUND:
		txt += "The specified attachment was not found. No message was sent.\r\n" ;
		break;
	case MAPI_E_ATTACHMENT_OPEN_FAILURE:
		txt += "The specified attachment could not be opened. No message was sent.\r\n" ;
		break;
	case MAPI_E_BAD_RECIPTYPE:
		txt += "The type of a recipient was not MAPI_TO, MAPI_CC, or MAPI_BCC. No message was sent.\r\n" ;
		break;
	case MAPI_E_FAILURE:
		txt += "One or more unspecified errors occurred. No message was sent.\r\n" ;
		break;
	case MAPI_E_INSUFFICIENT_MEMORY:
		txt += "There was insufficient memory to proceed. No message was sent.\r\n" ;
		break;
	case MAPI_E_INVALID_RECIPS:
		txt += "One or more recipients were invalid or did not resolve to any address.\r\n" ;
		break;
	case MAPI_E_LOGIN_FAILURE:
		txt += "There was no default logon, and the user failed to log on successfully when the logon dialog box was displayed. No message was sent.\r\n" ;
		break;
	case MAPI_E_TEXT_TOO_LARGE:
		txt += "The text in the message was too large. No message was sent.\r\n" ;
		break;
	case MAPI_E_TOO_MANY_FILES:
		txt += "There were too many file attachments. No message was sent.\r\n" ;
		break;
	case MAPI_E_TOO_MANY_RECIPIENTS:
		txt += "There were too many recipients. No message was sent.\r\n" ;
		break;
	case MAPI_E_UNKNOWN_RECIPIENT:
		txt += "A recipient did not appear in the address list. No message was sent.\r\n" ;
		break;
	case MAPI_E_USER_ABORT:
		txt += "The user canceled one of the dialog boxes. No message was sent.\r\n" ;
		break;
	default:
		txt += "Unknown error code.\r\n" ;
		break;
	}
	::MessageBox(0, txt.c_str(), "Error", MB_OK);

	if (MAPI_E_USER_ABORT == status)
		return -1;
	// other failure
	return 0;
}

BOOL CMailMsg::CMCSend()
{
	if (!initIfNeeded())
	{
		return FALSE;
	}

	auto_buffer<CMC_recipient> pRecipients(m_to.size() + m_cc.size() + m_bcc.size() + m_from.size());
	auto_buffer<CMC_attachment> pAttachments(m_attachments.size());

	TStrStrVector::iterator p;
	int                  nIndex = 0;
	CMC_message          message;
	CMC_boolean          bAvailable = FALSE;
	CMC_time             t_now = {0};
	
	// set cc's
	for (p = m_cc.begin(); p != m_cc.end(); p++, nIndex++)
	{
		pRecipients[nIndex].name                = (LPTSTR)(LPCTSTR)p->second.c_str();
		pRecipients[nIndex].name_type           = CMC_TYPE_INDIVIDUAL;
		pRecipients[nIndex].address             = (LPTSTR)(LPCTSTR)p->first.c_str();
		pRecipients[nIndex].role                = CMC_ROLE_CC;
		pRecipients[nIndex].recip_flags         = 0;
		pRecipients[nIndex].recip_extensions    = NULL;
	}

	// set bcc
	for (p = m_bcc.begin(); p != m_bcc.end(); p++, nIndex++)
	{
		pRecipients[nIndex].name                = (LPTSTR)(LPCTSTR)p->second.c_str();
		pRecipients[nIndex].name_type           = CMC_TYPE_INDIVIDUAL;
		pRecipients[nIndex].address             = (LPTSTR)(LPCTSTR)p->first.c_str();
		pRecipients[nIndex].role                = CMC_ROLE_BCC;
		pRecipients[nIndex].recip_flags         = 0;
		pRecipients[nIndex].recip_extensions    = NULL;
	}

	// set to
	pRecipients[nIndex].name                   = (LPTSTR)(LPCTSTR)m_to.begin()->second.c_str();
	pRecipients[nIndex].name_type              = CMC_TYPE_INDIVIDUAL;
	pRecipients[nIndex].address                = (LPTSTR)(LPCTSTR)m_to.begin()->first.c_str();
	pRecipients[nIndex].role                   = CMC_ROLE_TO;
	pRecipients[nIndex].recip_flags            = 0;
	pRecipients[nIndex].recip_extensions       = NULL;

	// set from
	pRecipients[nIndex+1].name                 = (LPTSTR)(LPCTSTR)m_from.begin()->second.c_str();
	pRecipients[nIndex+1].name_type            = CMC_TYPE_INDIVIDUAL;
	pRecipients[nIndex+1].address              = (LPTSTR)(LPCTSTR)m_from.begin()->first.c_str();
	pRecipients[nIndex+1].role                 = CMC_ROLE_ORIGINATOR;
	pRecipients[nIndex+1].recip_flags          = CMC_RECIP_LAST_ELEMENT;
	pRecipients[nIndex+1].recip_extensions     = NULL;

	// add attachments
	for (p = m_attachments.begin(), nIndex = 0;
		p != m_attachments.end(); p++, nIndex++)
	{
		pAttachments[nIndex].attach_title       = (LPTSTR)(LPCTSTR)p->second.c_str();
		pAttachments[nIndex].attach_type        = NULL;
		pAttachments[nIndex].attach_filename    = (LPTSTR)(LPCTSTR)p->first.c_str();
		pAttachments[nIndex].attach_flags       = 0;
		pAttachments[nIndex].attach_extensions  = NULL;
	}
	pAttachments[nIndex-1].attach_flags = CMC_ATT_LAST_ELEMENT;

	message.message_reference = NULL;
	message.message_type = NULL;
	if (m_sSubject.empty())
		message.subject = "No Subject";
	else
		message.subject = (LPTSTR)(LPCTSTR)m_sSubject.c_str();
	message.time_sent = t_now;
	if (m_sMessage.empty())
		message.text_note = "No Body";
	else
		message.text_note = (LPTSTR)(LPCTSTR)m_sMessage.c_str();

	message.recipients                        = pRecipients;
	message.attachments                       = pAttachments;
	message.message_flags                     = 0;
	message.message_extensions                = NULL;

	CMC_return_code status = m_lpCmcQueryConfiguration(
								0,
								CMC_CONFIG_UI_AVAIL,
								(void*)&bAvailable,
								NULL
								);

	if (CMC_SUCCESS == status && bAvailable)
	{
		CMC_session_id session;
		status = m_lpCmcLogon(
					NULL,
					NULL,
					NULL,
					NULL,
					0,
					CMC_VERSION,
					CMC_LOGON_UI_ALLOWED |
					CMC_ERROR_UI_ALLOWED,
					&session,
					NULL
					);

		if (CMC_SUCCESS == status)
		{
			status = m_lpCmcSend(session, &message, 0, 0, NULL);
			m_lpCmcLogoff(session, NULL, CMC_LOGON_UI_ALLOWED, NULL);
		}
	}
	return ((CMC_SUCCESS == status) && bAvailable);
}

BOOL CMailMsg::Initialize()
{
   m_hMapi = ::LoadLibrary(_T("mapi32.dll"));
   
   if (!m_hMapi)
      return FALSE;

   m_lpCmcQueryConfiguration = (LPCMCQUERY)::GetProcAddress(m_hMapi, _T("cmc_query_configuration"));
   m_lpCmcLogon = (LPCMCLOGON)::GetProcAddress(m_hMapi, _T("cmc_logon"));
   m_lpCmcSend = (LPCMCSEND)::GetProcAddress(m_hMapi, _T("cmc_send"));
   m_lpCmcLogoff = (LPCMCLOGOFF)::GetProcAddress(m_hMapi, _T("cmc_logoff"));
   m_lpMapiLogon = (LPMAPILOGON)::GetProcAddress(m_hMapi, _T("MAPILogon"));
   m_lpMapiSendMail = (LPMAPISENDMAIL)::GetProcAddress(m_hMapi, _T("MAPISendMail"));
   m_lpMapiLogoff = (LPMAPILOGOFF)::GetProcAddress(m_hMapi, _T("MAPILogoff"));
   m_lpMapiResolveName = (LPMAPIRESOLVENAME) GetProcAddress(m_hMapi, _T("MAPIResolveName"));
   m_lpMapiFreeBuffer = (LPMAPIFREEBUFFER) GetProcAddress(m_hMapi, _T("MAPIFreeBuffer"));

   m_bReady = (m_lpCmcLogon && m_lpCmcSend && m_lpCmcLogoff) ||
              (m_lpMapiLogon && m_lpMapiSendMail && m_lpMapiLogoff);

   return m_bReady;
}

void CMailMsg::Uninitialize()
{
   ::FreeLibrary(m_hMapi);
}

bool CMailMsg::initIfNeeded()
{
	return (m_bReady || Initialize());
}