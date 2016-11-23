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

#ifndef FIX_MESSAGE
#define FIX_MESSAGE

#ifdef _MSC_VER
#pragma warning( disable: 4786 )
#endif

#include "FieldMap.h"
#include "Fields.h"
#include "Group.h"
#include "SessionID.h"
#include <vector>

namespace FIX
{
  typedef FieldMap Header;
  typedef FieldMap Trailer;

  /*! \addtogroup user
   *  @{
   */
  /**
   * Base class for all %FIX messages.
   *
   * A message consists of a three field maps.  On for the header, the body,
   * and the trailer.
   */
  class Message : public FieldMap
  {
    friend class MessageValidator;
    friend class Session;

    enum field_type { header, body, trailer };
    typedef std::map<int, std::vector<Group> > Groups;

  public:
    Message()
      : m_header(message_order(message_order::header)),
        m_trailer(message_order(message_order::trailer)),
        m_validStructure(true) {}

    /// Construct a message from a string
    Message( const std::string& string ) throw( InvalidMessage& )
      : m_header(message_order(message_order::header)),
        m_trailer(message_order(message_order::trailer)),
        m_validStructure(true)
    {
      if(!setString(string))
        throw InvalidMessage();
    }

    void addGroup( Group& group )
    {
      m_groups[group.field()].push_back(group);
    }

    Group& getGroup( unsigned num, Group& group ) throw(FieldNotFound&)
    {
      Groups::iterator i = m_groups.find(num);
      if(i == m_groups.end()) throw FieldNotFound(group.field());
      if(i->second.size() < num || num < 0) throw FieldNotFound(group.field());
      group = *(i->second.begin() + (num - 1));
      return group;
    }

  protected:
    // Constructor for derived classes
    Message( const BeginString& beginString, const MsgType& msgType )
      : m_header(message_order(message_order::header)),
        m_trailer(message_order(message_order::trailer)),
        m_validStructure(true)
    {
      m_header.setField(beginString);
      m_header.setField(msgType);
    }

  public:
    /// Get a string representation of the message
    std::string getString() const
    {
      m_header.setField( bodyLength() );
      m_trailer.setField( checkSum() );

      return
        m_header.calculateString() +
        FieldMap::calculateString() +
        m_trailer.calculateString();
    }

    /**
     * Set a message based on a string representation
     * This will fill in the fields on the message by parsing out the string
     * that is passed in.  It will return true on success and false
     * on failure.
     */
    bool setString( const std::string& string )
    {
      clear();

      std::string::size_type pos = 0;
      int count = 0;

      static FIELD::Field const headerOrder[] = {
        FIELD::BeginString,
        FIELD::BodyLength,
        FIELD::MsgType
      };

      field_type type = header;

      while(pos < string.size())
        {
          FieldBase field = extractField(string,pos);
          if (count < 3 && headerOrder[count++] != field.getField())
            return false;

          if( isHeaderField(field) )
            { if(type != header) m_validStructure = false;
            m_header.setField(field); }
          else
            if( isTrailerField(field) )
              { type = trailer;
              m_trailer.setField(field); }
            else
              { if(type == trailer) m_validStructure = false;
              type = body;
              setField(field); }
        }
      return validate();
    }

    /// Getter for the message header
    const Header& getHeader() const { return m_header; }
    /// Mutable getter for the message header
    Header& getHeader() { return m_header; }
    /// Getter for the message trailer
    const Header& getTrailer() const { return m_trailer; }
    /// Mutable getter for the message trailer
    Header& getTrailer() { return m_trailer; }

    bool hasValidStructure() const { return m_validStructure; }

    BodyLength bodyLength() const
    {
      return BodyLength
        (m_header.calculateLength()
         + calculateLength()
         + m_trailer.calculateLength());
    }

    CheckSum checkSum() const
    {
      return CheckSum
        ((m_header.calculateTotal()
          + calculateTotal()
          + m_trailer.calculateTotal()) % 256);
    }

    static bool isAdminMsgType( const MsgType& msgType )
    {
      if( !msgType.getValue().length() ) return false;
      return strchr
        ("0A12345",
         msgType.getValue().c_str()[0]) != 0;
    }

    static bool isHeaderField( int field )
    {
      switch( field )
        {
        case FIELD::BeginString:
        case FIELD::BodyLength:
        case FIELD::MsgType:
        case FIELD::SenderCompID:
        case FIELD::TargetCompID:
        case FIELD::OnBehalfOfCompID:
        case FIELD::DeliverToCompID:
        case FIELD::SecureDataLen:
        case FIELD::MsgSeqNum:
        case FIELD::SenderSubID:
        case FIELD::SenderLocationID:
        case FIELD::TargetSubID:
        case FIELD::TargetLocationID:
        case FIELD::OnBehalfOfSubID:
        case FIELD::OnBehalfOfLocationID:
        case FIELD::DeliverToSubID:
        case FIELD::DeliverToLocationID:
        case FIELD::PossDupFlag:
        case FIELD::PossResend:
        case FIELD::SendingTime:
        case FIELD::OrigSendingTime:
        case FIELD::XmlDataLen:
        case FIELD::XmlData:
        case FIELD::MessageEncoding:
        case FIELD::LastMsgSeqNumProcessed:
        case FIELD::OnBehalfOfSendingTime:
          return true;
        default:
          return false;
        };
    }
    static bool isHeaderField( const FieldBase& field )
    {
      return isHeaderField(field.getField());
    }

    static bool isTrailerField( int field )
    {
      switch( field )
        {
        case FIELD::SignatureLength:
        case FIELD::Signature:
        case FIELD::CheckSum:
          return true;
        default:
          return false;
        };
    }
    static bool isTrailerField( const FieldBase& field )
    {
      return isTrailerField(field.getField());
    }

    /// Returns the session ID of the intended recipient
    SessionID getSessionID() throw(FieldNotFound&)
    {
      BeginString beginString;
      SenderCompID senderCompID;
      TargetCompID targetCompID;

      getHeader().getField(beginString);
      getHeader().getField(senderCompID);
      getHeader().getField(targetCompID);

      return SessionID(beginString, senderCompID, targetCompID);
    }

    /// Sets the session ID of the intended recipient
    void setSessionID( const SessionID& sessionID )
    {
      getHeader().setField(sessionID.getBeginString());
      getHeader().setField(sessionID.getSenderCompID());
      getHeader().setField(sessionID.getTargetCompID());
    }

  private:
    FieldBase extractField
    (const std::string string, std::string::size_type& pos)
    {
      FIELD::Field field;
      std::string fieldString;
      std::string valueString;

      std::string::size_type equalSign = string.find_first_of('=', pos);
      fieldString = string.substr(pos, equalSign - pos);

      std::string::size_type soh =
        string.find_first_of('\001', equalSign+1);
      valueString = string.substr(equalSign+1, soh - (equalSign+1));

      std::stringstream stream(fieldString);
      stream >> field;

      pos = soh+1;
      return FieldBase( field, valueString );
    }

    void clear()
    {
      m_header.clear();
      FieldMap::clear();
      m_trailer.clear();
    }

    bool validate()
    {
      try
        {
          CheckSum aCheckSum;
          m_trailer.getField(aCheckSum);
          return aCheckSum == checkSum();
        }
      catch( FieldNotFound& ) { return false; }
    }

  protected:
    mutable FieldMap m_header;
    mutable FieldMap m_trailer;
    Groups m_groups;
    bool m_validStructure;
  };
  /*! @} */

  inline std::ostream& operator <<
  (std::ostream& stream, const Message& message)
  {
    stream << message.getString();
    return stream;
  }

  /// Parse the type of a message from a string.
  inline MsgType identifyType( const std::string& message )
    throw( MessageParseError& )
  {
    std::string::size_type pos = message.find("\00135=");
    if( pos == std::string::npos ) throw MessageParseError();

    std::string::size_type startValue = pos + 4;
    std::string::size_type soh = message.find_first_of('\001', startValue);
    if( soh == std::string::npos ) throw MessageParseError();

    std::string value = message.substr(startValue, soh - startValue);
    return MsgType(value);
  }
}

#endif //FIX_MESSAGE
