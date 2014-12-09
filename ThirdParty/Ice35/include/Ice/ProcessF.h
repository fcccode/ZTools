// **********************************************************************
//
// Copyright (c) 2003-2013 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************
//
// Ice version 3.5.0
//
// <auto-generated>
//
// Generated from file `ProcessF.ice'
//
// Warning: do not edit this file.
//
// </auto-generated>
//

#ifndef __Ice_ProcessF_h__
#define __Ice_ProcessF_h__

#include <Ice/ProxyF.h>
#include <Ice/ObjectF.h>
#include <Ice/Exception.h>
#include <Ice/LocalObject.h>
#include <Ice/StreamHelpers.h>
#include <IceUtil/ScopedArray.h>
#include <IceUtil/Optional.h>
#include <Ice/UndefSysMacros.h>

#ifndef ICE_IGNORE_VERSION
#   if ICE_INT_VERSION / 100 != 305
#       error Ice version mismatch!
#   endif
#   if ICE_INT_VERSION % 100 > 50
#       error Beta header file detected
#   endif
#   if ICE_INT_VERSION % 100 < 0
#       error Ice patch level mismatch!
#   endif
#endif

#ifndef ICE_API
#   ifdef ICE_API_EXPORTS
#       define ICE_API ICE_DECLSPEC_EXPORT
#   else
#       define ICE_API ICE_DECLSPEC_IMPORT
#   endif
#endif

namespace IceProxy
{

namespace Ice
{

class Process;
ICE_API void __read(::IceInternal::BasicStream*, ::IceInternal::ProxyHandle< ::IceProxy::Ice::Process>&);
ICE_API ::IceProxy::Ice::Object* upCast(::IceProxy::Ice::Process*);

}

}

namespace Ice
{

class Process;
bool operator==(const Process&, const Process&);
bool operator<(const Process&, const Process&);
ICE_API ::Ice::Object* upCast(::Ice::Process*);
typedef ::IceInternal::Handle< ::Ice::Process> ProcessPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::Ice::Process> ProcessPrx;
ICE_API void __patch(ProcessPtr&, const ::Ice::ObjectPtr&);

}

#endif