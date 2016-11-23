<!--
  ====================================================================
  The QuickFIX Software License, Version 1.0
 
  Copyright (c) 2001 ThoughtWorks, Inc.  All rights
  reserved.
 
  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:
 
  1. Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
 
  2. Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.
 
  3. The end-user documentation included with the redistribution,
     if any, must include the following acknowledgment:
        "This product includes software developed by
         ThoughtWorks, Inc. (http://www.thoughtworks.com/)."
     Alternately, this acknowledgment may appear in the software itself,
     if and wherever such third-party acknowledgments normally appear.
 
  4. The names "QuickFIX" and "ThoughtWorks, Inc." must
     not be used to endorse or promote products derived from this
     software without prior written permission. For written
     permission, please contact quickfix-users@lists.sourceforge.net.
 
  5. Products derived from this software may not be called "QuickFIX",
     nor may "QuickFIX" appear in their name, without prior written
     permission of ThoughtWorks, Inc.
 
  THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED.  IN NO EVENT SHALL THOUGHTWORKS INC OR
  ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
  OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
  SUCH DAMAGE.
  ====================================================================
 -->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
 <xsl:output  method="text" encoding="UTF-8"/>

 <xsl:template match="text()"/>

 <xsl:template match="/">/* -*- C++ -*- */<xsl:copy-of select=
   "document('../LICENSE.xml')"/>
package org.quickfix.fix<xsl:value-of select="//fix/@major"/><xsl:value-of select="//fix/@minor"/>;

import org.quickfix.*;
import org.quickfix.field.*;

public class MessageCracker <xsl:call-template name="base-class"/>
{
public void onMessage( org.quickfix.Message message, SessionID sessionID ) throws FieldNotFound, UnsupportedMessageType, IncorrectTagValue
  { throw new UnsupportedMessageType(); }
<xsl:call-template name="virtual-functions"/>
<xsl:call-template name="switch-statement"/>
  };


</xsl:template>

<xsl:template name="virtual-functions">
 <xsl:for-each select="//fix/messages/message[@msgcat='app']"> public void onMessage( <xsl:value-of select="@name"/> message, SessionID sessionID ) throws FieldNotFound, UnsupportedMessageType, IncorrectTagValue
 <xsl:if test="@name!='BusinessMessageReject'">   { throw new UnsupportedMessageType(); }
 </xsl:if>
 <xsl:if test="@name='BusinessMessageReject'">   {}
 </xsl:if>
</xsl:for-each>
</xsl:template>

<xsl:template name="switch-statement">
  public void crack( org.quickfix.Message message, SessionID sessionID )
    throws UnsupportedMessageType, FieldNotFound, IncorrectTagValue
  { crack<xsl:value-of select="//fix/@major"/><xsl:value-of select="//fix/@minor"/>((Message)message, sessionID); }

  public void crack<xsl:value-of select="//fix/@major"/><xsl:value-of select="//fix/@minor"/>( Message message, SessionID sessionID ) 
    throws UnsupportedMessageType, FieldNotFound, IncorrectTagValue
  {
    MsgType msgType = new MsgType();
    message.getHeader().getField(msgType);
    String msgTypeString = msgType.getValue();
    if(msgTypeString.length() > 1)
    {
      onMessage( message, sessionID );
      return;
    }

    switch(msgTypeString.charAt(0))
    {<xsl:for-each select="//fix/messages/message">
      case '<xsl:value-of select="@msgtype"/>':
        onMessage( (<xsl:value-of select="@name"/>)message, sessionID );
	      break;</xsl:for-each>
      default:
        onMessage( message, sessionID );
    }
  }
</xsl:template>

<xsl:template name="base-class">
 <xsl:if test="//fix/@major='4'">
   <xsl:if test="//fix/@minor='1'">extends org.quickfix.fix40.MessageCracker</xsl:if>
   <xsl:if test="//fix/@minor='2'">extends org.quickfix.fix41.MessageCracker</xsl:if>
 </xsl:if>
</xsl:template>

</xsl:stylesheet>
