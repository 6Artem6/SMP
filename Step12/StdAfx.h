#ifndef __STDAFX_H
#define __STDAFX_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN			/// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>				/// MFC core and standard components
#include <afxext.h>				/// MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>				/// MFC OLE classes
#include <afxodlgs.h>			/// MFC OLE dialog classes
#include <afxdisp.h>			/// MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef __LIBTOOL_H
#include <libtool.h>
#endif

#ifndef __LHEAD3D_H
#include <LHead3d.h>
#endif

#pragma warning( disable : 4192 )
#pragma warning( disable : 4278 )

#import "ksConstants3D.tlb" no_namespace named_guids
#import "ksConstants.tlb"	no_namespace named_guids
#import "kAPI2D5COM.tlb"	no_namespace named_guids
#import "kAPI3D5COM.tlb"	no_namespace named_guids

#ifndef __LIGHT_VERSION__
#import "kAPI7.tlb"			no_namespace named_guids
#else
#import "klAPI7.tlb"		no_namespace named_guids
#endif

#pragma warning( once : 4192 )
#pragma warning( once : 4278 )

//{{AFX_INSERT_LOCATION}}
#endif // !defined(__STDAFX_H)
