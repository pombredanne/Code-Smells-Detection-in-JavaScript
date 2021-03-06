/* ====================================================================
* The QuickFIX Software License, Version 1.0
*
* Copyright (c) 2002 ThoughtWorks, Inc.  All rights
* reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in
*    the documentation and/or other materials provided with the
*    distribution.
*
* 3. The end-user documentation included with the redistribution,
*    if any, must include the following acknowledgment:
*       "This product includes software developed by
*        ThoughtWorks, Inc. (http://www.thoughtworks.com/)."
*    Alternately, this acknowledgment may appear in the software itself,
*    if and wherever such third-party acknowledgments normally appear.
*
* 4. The names "QuickFIX" and "ThoughtWorks, Inc." must
*    not be used to endorse or promote products derived from this
*    software without prior written permission. For written
*    permission, please contact quickfix-users@lists.sourceforge.net.
*
* 5. Products derived from this software may not be called "QuickFIX",
*    nor may "QuickFIX" appear in their name, without prior written
*    permission of ThoughtWorks, Inc.
*
* THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED.  IN NO EVENT SHALL THOUGHTWORKS INC OR
* ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
* USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
* ====================================================================
*/

#ifdef _MSC_VER
#include "stdafx.h"
#else
#include "config.h"
#endif

#include "JVM.h"
#include "Conversions.h"
#include "org_quickfix_ScreenLogFactory.h"
#include "quickfix/include/Log.h"

JNIEXPORT void JNICALL Java_org_quickfix_ScreenLogFactory_create__ZZZ
( JNIEnv *pEnv, jobject obj, jboolean incoming, jboolean outgoing, jboolean event )
{
  JVM::set( pEnv );
  JVMObject object( obj );

  FIX::LogFactory* pFactory = new FIX::ScreenLogFactory
                              ( incoming, outgoing, event );
  object.setInt( "cppPointer", ( int ) pFactory );
}

JNIEXPORT void JNICALL Java_org_quickfix_ScreenLogFactory_destroy
( JNIEnv *pEnv, jobject obj )
{
  JVM::set( pEnv );
  JVMObject jobject( obj );
  FIX::ScreenLogFactory* pFactory
  = ( FIX::ScreenLogFactory* ) jobject.getInt( "cppPointer" );
  delete pFactory;
}

JNIEXPORT jobject JNICALL Java_org_quickfix_ScreenLogFactory_create__Lorg_quickfix_SessionID_2
( JNIEnv * pEnv, jobject obj, jobject sessionID )
{
  JVM::set( pEnv );
  JVMObject jobject( obj );
  JVMObject jsession( sessionID );

  FIX::ScreenLogFactory* pFactory
  = ( FIX::ScreenLogFactory* ) jobject.getInt( "cppPointer" );
  FIX::SessionID* pSessionID
  = ( FIX::SessionID* ) jsession.getInt( "cppPointer" );

  try
  {
    FIX::Log* pLog = pFactory->create( *pSessionID );
    JVMClass type( "Lorg/quickfix/ScreenLog;" );
    jmethodID method = pEnv->GetMethodID( type, "<init>", "(I)V" );
    return pEnv->NewObject( type, method, ( jint ) pLog );
  }
  catch ( FIX::ConfigError & e )
  {
    throwNew( "Lorg/quickfix/ConfigError;", e.what() );
  }
  return 0;
}
