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

package quickfix.fix41;

import quickfix.Message;

public class MessageFactory implements quickfix.MessageFactory
{
  public Message create( String beginString, String msgType ) {
  
   if("0".equals(msgType)) {
     return new quickfix.fix41.Heartbeat();
   }
 
   if("A".equals(msgType)) {
     return new quickfix.fix41.Logon();
   }
 
   if("1".equals(msgType)) {
     return new quickfix.fix41.TestRequest();
   }
 
   if("2".equals(msgType)) {
     return new quickfix.fix41.ResendRequest();
   }
 
   if("3".equals(msgType)) {
     return new quickfix.fix41.Reject();
   }
 
   if("4".equals(msgType)) {
     return new quickfix.fix41.SequenceReset();
   }
 
   if("5".equals(msgType)) {
     return new quickfix.fix41.Logout();
   }
 
   if("7".equals(msgType)) {
     return new quickfix.fix41.Advertisement();
   }
 
   if("6".equals(msgType)) {
     return new quickfix.fix41.IndicationofInterest();
   }
 
   if("B".equals(msgType)) {
     return new quickfix.fix41.News();
   }
 
   if("C".equals(msgType)) {
     return new quickfix.fix41.Email();
   }
 
   if("R".equals(msgType)) {
     return new quickfix.fix41.QuoteRequest();
   }
 
   if("S".equals(msgType)) {
     return new quickfix.fix41.Quote();
   }
 
   if("D".equals(msgType)) {
     return new quickfix.fix41.NewOrderSingle();
   }
 
   if("8".equals(msgType)) {
     return new quickfix.fix41.ExecutionReport();
   }
 
   if("Q".equals(msgType)) {
     return new quickfix.fix41.DontKnowTrade();
   }
 
   if("G".equals(msgType)) {
     return new quickfix.fix41.OrderCancelReplaceRequest();
   }
 
   if("F".equals(msgType)) {
     return new quickfix.fix41.OrderCancelRequest();
   }
 
   if("9".equals(msgType)) {
     return new quickfix.fix41.OrderCancelReject();
   }
 
   if("H".equals(msgType)) {
     return new quickfix.fix41.OrderStatusRequest();
   }
 
   if("J".equals(msgType)) {
     return new quickfix.fix41.Allocation();
   }
 
   if("P".equals(msgType)) {
     return new quickfix.fix41.AllocationACK();
   }
 
   if("T".equals(msgType)) {
     return new quickfix.fix41.SettlementInstructions();
   }
 
   if("E".equals(msgType)) {
     return new quickfix.fix41.NewOrderList();
   }
 
   if("N".equals(msgType)) {
     return new quickfix.fix41.ListStatus();
   }
 
   if("L".equals(msgType)) {
     return new quickfix.fix41.ListExecute();
   }
 
   if("K".equals(msgType)) {
     return new quickfix.fix41.ListCancelRequest();
   }
 
   if("M".equals(msgType)) {
     return new quickfix.fix41.ListStatusRequest();
   }
 
  return new quickfix.fix41.Message();
  }
}

