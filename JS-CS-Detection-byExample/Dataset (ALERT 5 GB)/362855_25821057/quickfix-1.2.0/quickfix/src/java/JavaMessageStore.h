/* -*- C++ -*- */
/* ====================================================================
 * The QuickFIX Software License, Version 1.0
 *
 * Copyright (c) 2001 ThoughtWorks, Inc.  All rights
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

#ifndef JAVA_MESSAGE_STORE
#define JAVA_MESSAGE_STORE

#include "quickfix/include/MessageStore.h"
#include "JVM.h"

class JavaMessageStore : public FIX::MessageStore
{
public:
  JavaMessageStore(JVMObject object);
  virtual ~JavaMessageStore();

  virtual bool set( const FIX::Message& message );
  virtual bool get( int seq, FIX::Message& message ) const;
  virtual bool get( int start, int end, std::vector<FIX::Message>& ) const;
  virtual int getNextSenderMsgSeqNum() const;
  virtual int getNextTargetMsgSeqNum() const;
  virtual void setNextSenderMsgSeqNum(int seq);
  virtual void setNextTargetMsgSeqNum(int seq);
  virtual void incrNextSenderMsgSeqNum();
  virtual void incrNextTargetMsgSeqNum();
  virtual FIX::UtcTimeStamp getCreationTime() const;
  virtual void reset();

private:
  jobject messageToJavaMessage( const FIX::Message& message );
  jobject createJavaMessage() const;
  jobject createCollection() const;

  mutable JVMObject messageStore;

  jmethodID setId;
  jmethodID getId;
  jmethodID getRangeId;
  jmethodID getNextSenderMsgSeqNumId;
  jmethodID getNextTargetMsgSeqNumId;
  jmethodID setNextSenderMsgSeqNumId;
  jmethodID setNextTargetMsgSeqNumId;
  jmethodID incrNextSenderMsgSeqNumId;
  jmethodID incrNextTargetMsgSeqNumId;
  jmethodID getCreationTimeId;
  jmethodID resetId;
};

#endif
