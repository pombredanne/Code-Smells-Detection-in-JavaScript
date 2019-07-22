<!--
  ====================================================================
  The QuickFix Software License, Version 1.0
 
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
 
  4. The names "QuickFix" and "ThoughtWorks, Inc." must
     not be used to endorse or promote products derived from this
     software without prior written permission. For written
     permission, please contact quickfix-users@lists.sourceforge.net.
 
  5. Products derived from this software may not be called "QuickFix",
     nor may "QuickFix" appear in their name, without prior written
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
using System;

namespace QuickFix<xsl:value-of select="//fix/@major"/><xsl:value-of select="//fix/@minor"/>
{

  public class MessageFactory : QuickFix.MessageFactory
  {
    public QuickFix.Message create( String beginString, String msgType ) {
    <xsl:call-template name="if-statement"/>
    return new QuickFix<xsl:value-of select="//fix/@major"/><xsl:value-of select="//fix/@minor"/>.Message();
    }
  };
}
  </xsl:template>

  <xsl:template name="if-statement">
   <xsl:for-each select="//fix/messages/message">
     if(msgType.Equals("<xsl:value-of select="@msgtype"/>")) {
       return new QuickFix<xsl:value-of select="//fix/@major"/><xsl:value-of select="//fix/@minor"/>.<xsl:value-of select="@name"/>();
     }
   </xsl:for-each>
  </xsl:template>
</xsl:stylesheet>