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
// Generated from file `pcp.ice'
//
// Warning: do not edit this file.
//
// </auto-generated>
//

#include <StdAfx.h>
#include "pcp.h"
#include <Ice/LocalException.h>
#include <Ice/ObjectFactory.h>
#include <Ice/BasicStream.h>
#include <Ice/Object.h>
#include <IceUtil/Iterator.h>

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

namespace
{

const ::std::string __MPcp__IPcp__InvokeMethod_name = "InvokeMethod";

}
::IceProxy::Ice::Object* ::IceProxy::MPcp::upCast(::IceProxy::MPcp::IPcp* p) { return p; }

void
::IceProxy::MPcp::__read(::IceInternal::BasicStream* __is, ::IceInternal::ProxyHandle< ::IceProxy::MPcp::IPcp>& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::MPcp::IPcp;
        v->__copyFrom(proxy);
    }
}

::MPcp::VectorString
IceProxy::MPcp::IPcp::InvokeMethod(const ::std::string& strMethodName, const ::MPcp::VectorString& vecParams, const ::Ice::Context* __ctx)
{
    ::IceInternal::InvocationObserver __observer(this, __MPcp__IPcp__InvokeMethod_name, __ctx);
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__MPcp__IPcp__InvokeMethod_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::MPcp::IPcp* __del = dynamic_cast< ::IceDelegate::MPcp::IPcp*>(__delBase.get());
            return __del->InvokeMethod(strMethodName, vecParams, __ctx, __observer);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, __observer);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, true, __cnt, __observer);
        }
    }
}

::Ice::AsyncResultPtr
IceProxy::MPcp::IPcp::begin_InvokeMethod(const ::std::string& strMethodName, const ::MPcp::VectorString& vecParams, const ::Ice::Context* __ctx, const ::IceInternal::CallbackBasePtr& __del, const ::Ice::LocalObjectPtr& __cookie)
{
    __checkAsyncTwowayOnly(__MPcp__IPcp__InvokeMethod_name);
    ::IceInternal::OutgoingAsyncPtr __result = new ::IceInternal::OutgoingAsync(this, __MPcp__IPcp__InvokeMethod_name, __del, __cookie);
    try
    {
        __result->__prepare(__MPcp__IPcp__InvokeMethod_name, ::Ice::Normal, __ctx);
        ::IceInternal::BasicStream* __os = __result->__startWriteParams(::Ice::DefaultFormat);
        __os->write(strMethodName);
        __os->write(vecParams);
        __result->__endWriteParams();
        __result->__send(true);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __result->__exceptionAsync(__ex);
    }
    return __result;
}

::MPcp::VectorString
IceProxy::MPcp::IPcp::end_InvokeMethod(const ::Ice::AsyncResultPtr& __result)
{
    ::Ice::AsyncResult::__check(__result, this, __MPcp__IPcp__InvokeMethod_name);
    ::MPcp::VectorString __ret;
    bool __ok = __result->__wait();
    try
    {
        if(!__ok)
        {
            try
            {
                __result->__throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
            }
        }
        ::IceInternal::BasicStream* __is = __result->__startReadParams();
        __is->read(__ret);
        __result->__endReadParams();
        return __ret;
    }
    catch(const ::Ice::LocalException& ex)
    {
        __result->__getObserver().failed(ex.ice_name());
        throw;
    }
}

const ::std::string&
IceProxy::MPcp::IPcp::ice_staticId()
{
    return ::MPcp::IPcp::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::MPcp::IPcp::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::MPcp::IPcp);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::MPcp::IPcp::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::MPcp::IPcp);
}

::IceProxy::Ice::Object*
IceProxy::MPcp::IPcp::__newInstance() const
{
    return new IPcp;
}

::MPcp::VectorString
IceDelegateM::MPcp::IPcp::InvokeMethod(const ::std::string& strMethodName, const ::MPcp::VectorString& vecParams, const ::Ice::Context* __context, ::IceInternal::InvocationObserver& __observer)
{
    ::IceInternal::Outgoing __og(__handler.get(), __MPcp__IPcp__InvokeMethod_name, ::Ice::Normal, __context, __observer);
    try
    {
        ::IceInternal::BasicStream* __os = __og.startWriteParams(::Ice::DefaultFormat);
        __os->write(strMethodName);
        __os->write(vecParams);
        __og.endWriteParams();
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    ::MPcp::VectorString __ret;
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::IceInternal::BasicStream* __is = __og.startReadParams();
        __is->read(__ret);
        __og.endReadParams();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::MPcp::VectorString
IceDelegateD::MPcp::IPcp::InvokeMethod(const ::std::string& strMethodName, const ::MPcp::VectorString& vecParams, const ::Ice::Context* __context, ::IceInternal::InvocationObserver&)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::MPcp::VectorString& __result, const ::std::string& __p_strMethodName, const ::MPcp::VectorString& __p_vecParams, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_strMethodName(__p_strMethodName),
            _m_vecParams(__p_vecParams)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::MPcp::IPcp* servant = dynamic_cast< ::MPcp::IPcp*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->InvokeMethod(_m_strMethodName, _m_vecParams, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::MPcp::VectorString& _result;
        const ::std::string& _m_strMethodName;
        const ::MPcp::VectorString& _m_vecParams;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __MPcp__IPcp__InvokeMethod_name, ::Ice::Normal, __context);
    ::MPcp::VectorString __result;
    try
    {
        _DirectI __direct(__result, strMethodName, vecParams, __current);
        try
        {
            __direct.getServant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

::Ice::Object* MPcp::upCast(::MPcp::IPcp* p) { return p; }

namespace
{
const ::std::string __MPcp__IPcp_ids[2] =
{
    "::Ice::Object",
    "::MPcp::IPcp"
};

}

bool
MPcp::IPcp::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__MPcp__IPcp_ids, __MPcp__IPcp_ids + 2, _s);
}

::std::vector< ::std::string>
MPcp::IPcp::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__MPcp__IPcp_ids[0], &__MPcp__IPcp_ids[2]);
}

const ::std::string&
MPcp::IPcp::ice_id(const ::Ice::Current&) const
{
    return __MPcp__IPcp_ids[1];
}

const ::std::string&
MPcp::IPcp::ice_staticId()
{
    return __MPcp__IPcp_ids[1];
}

::Ice::DispatchStatus
MPcp::IPcp::___InvokeMethod(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.startReadParams();
    ::std::string strMethodName;
    ::MPcp::VectorString vecParams;
    __is->read(strMethodName);
    __is->read(vecParams);
    __inS.endReadParams();
    ::MPcp::VectorString __ret = InvokeMethod(strMethodName, vecParams, __current);
    ::IceInternal::BasicStream* __os = __inS.__startWriteParams(::Ice::DefaultFormat);
    __os->write(__ret);
    __inS.__endWriteParams(true);
    return ::Ice::DispatchOK;
}

namespace
{
const ::std::string __MPcp__IPcp_all[] =
{
    "InvokeMethod",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping"
};

}

::Ice::DispatchStatus
MPcp::IPcp::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< const ::std::string*, const ::std::string*> r = ::std::equal_range(__MPcp__IPcp_all, __MPcp__IPcp_all + 5, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __MPcp__IPcp_all)
    {
        case 0:
        {
            return ___InvokeMethod(in, current);
        }
        case 1:
        {
            return ___ice_id(in, current);
        }
        case 2:
        {
            return ___ice_ids(in, current);
        }
        case 3:
        {
            return ___ice_isA(in, current);
        }
        case 4:
        {
            return ___ice_ping(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
MPcp::IPcp::__writeImpl(::IceInternal::BasicStream* __os) const
{
    __os->startWriteSlice(ice_staticId(), -1, true);
    __os->endWriteSlice();
}

void
MPcp::IPcp::__readImpl(::IceInternal::BasicStream* __is)
{
    __is->startReadSlice();
    __is->endReadSlice();
}

void 
MPcp::__patch(IPcpPtr& handle, const ::Ice::ObjectPtr& v)
{
    handle = ::MPcp::IPcpPtr::dynamicCast(v);
    if(v && !handle)
    {
        IceInternal::Ex::throwUOE(::MPcp::IPcp::ice_staticId(), v);
    }
}
