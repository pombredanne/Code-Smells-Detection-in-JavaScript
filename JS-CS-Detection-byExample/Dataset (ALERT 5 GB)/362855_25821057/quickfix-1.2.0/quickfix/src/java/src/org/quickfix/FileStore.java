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

package org.quickfix;

import java.util.Collection;
import java.util.Date;

public class FileStore implements MessageStore {
    private int cppPointer;

    private FileStore() {
        create();
    }

    private FileStore(int cppPointer) {
        this.cppPointer = cppPointer;
    }

    protected void finalize() {
        destroy();
    }

    public boolean set(Message message ){
        return set0(message);
    }

    public boolean get(int sequence, Message message ) {
        return get0(sequence, message);
    }

    public boolean get(int startSequence, int endSequence, 
                       Collection messages ) {
        return get0(startSequence, endSequence, messages);
    }

    public int getNextSenderMsgSeqNum(){
        return getNextSenderMsgSeqNum0();
    }

    public int getNextTargetMsgSeqNum(){
        return getNextTargetMsgSeqNum0();
    }

    public void setNextSenderMsgSeqNum(int next){
        setNextSenderMsgSeqNum0(next);
    }

    public void setNextTargetMsgSeqNum(int next){
        setNextTargetMsgSeqNum0(next);
    }

    public void incrNextSenderMsgSeqNum(){
        incrNextSenderMsgSeqNum0();
    }

    public void incrNextTargetMsgSeqNum(){
        incrNextTargetMsgSeqNum0();
    }

    public Date getCreationTime(){
        return getCreationTime0();
    }

    public void reset(){
        reset0();
    }

    private native void create();
    private native void destroy();
    private native boolean set0(Message message);
    private native boolean get0(int sequence, Message message);
    private native boolean get0(int startSequence, int endSequence, 
                                Collection messages);
    private native int getNextSenderMsgSeqNum0();
    public native int getNextTargetMsgSeqNum0();
    private native void setNextSenderMsgSeqNum0(int next);
    private native void setNextTargetMsgSeqNum0(int next);
    private native void incrNextSenderMsgSeqNum0();
    private native void incrNextTargetMsgSeqNum0();
    private native Date getCreationTime0();
    private native void reset0();
}
