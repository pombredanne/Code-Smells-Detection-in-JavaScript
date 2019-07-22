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

#ifndef FIX_VALUES_H
#define FIX_VALUES_H

#include <string>

namespace FIX
{
  const char BeginString_FIX42[] = "FIX.4.2";
  const char BeginString_FIX41[] = "FIX.4.1";
  const char BeginString_FIX40[] = "FIX.4.0";
  
  const char MsgType_Heartbeat[] = "0";
  const char MsgType_Logon[] = "A";
  const char MsgType_TestRequest[] = "1";
  const char MsgType_ResendRequest[] = "2";
  const char MsgType_Reject[] = "3";
  const char MsgType_SequenceReset[] = "4";
  const char MsgType_Logout[] = "5";
  const char MsgType_Advertisement[] = "7";
  const char MsgType_IndicationofInterest[] = "6";
  const char MsgType_News[] = "B";
  const char MsgType_Email[] = "C";
  const char MsgType_QuoteRequest[] = "R";
  const char MsgType_Quote[] = "S";
  const char MsgType_MassQuote[] = "i";
  const char MsgType_QuoteCancel[] = "Z";
  const char MsgType_QuoteStatusRequest[] = "a";
  const char MsgType_QuoteAcknowledgement[] = "b";
  const char MsgType_MarketDataRequest[] = "V";
  const char MsgType_MarketDataSnapshotFullRefresh[] = "W";
  const char MsgType_MarketDataIncrementalRefresh[] = "X";
  const char MsgType_MarketDataRequestReject[] = "Y";
  const char MsgType_SecurityDefinitionRequest[] = "c";
  const char MsgType_SecurityDefinition[] = "d";
  const char MsgType_SecurityStatusRequest[] = "e";
  const char MsgType_SecurityStatus[] = "f";
  const char MsgType_TradingSessionStatusRequest[] = "g";
  const char MsgType_TradingSessionStatus[] = "h";
  const char MsgType_NewOrderSingle[] = "D";
  const char MsgType_ExecutionReport[] = "8";
  const char MsgType_DontKnowTrade[] = "Q";
  const char MsgType_OrderCancelReplaceRequest[] = "G";
  const char MsgType_OrderCancelRequest[] = "F";
  const char MsgType_OrderCancelReject[] = "9";
  const char MsgType_OrderStatusRequest[] = "H";
  const char MsgType_Allocation[] = "J";
  const char MsgType_AllocationACK[] = "P";
  const char MsgType_SettlementInstructions[] = "T";
  const char MsgType_NewOrderList[] = "E";
  const char MsgType_ListStatus[] = "N";
  const char MsgType_ListExecute[] = "L";
  const char MsgType_ListCancelRequest[] = "K";
  const char MsgType_ListStatusRequest[] = "M";
  const char MsgType_BusinessMessageReject[] = "j";
  const char AdvSide_BUY = 'B';
  const char AdvSide_SELL = 'S';
  const char AdvSide_CROSS = 'X';
  const char AdvSide_TRADE = 'T';
  const char AdvTransType_NEW = 'N';
  const char AdvTransType_CANCEL = 'C';
  const char AdvTransType_REPLACE = 'R';
  const char CommType_PER_SHARE = '1';
  const char CommType_PERCENTAGE = '2';
  const char CommType_ABSOLUTE = '3';
  const char ExecInst_NOT_HELD = '1';
  const char ExecInst_WORK = '2';
  const char ExecInst_GO_ALONG = '3';
  const char ExecInst_OVER_THE_DAY = '4';
  const char ExecInst_HELD = '5';
  const char ExecInst_PARTICIPATE_DONT_INITIATE = '6';
  const char ExecInst_STRICT_SCALE = '7';
  const char ExecInst_TRY_TO_SCALE = '8';
  const char ExecInst_STAY_ON_BIDSIDE = '9';
  const char ExecInst_STAY_ON_OFFERSIDE = '0';
  const char ExecInst_NO_CROSS = 'A';
  const char ExecInst_OK_TO_CROSS = 'B';
  const char ExecInst_CALL_FIRST = 'C';
  const char ExecInst_PERCENT_OF_VOLUME = 'D';
  const char ExecInst_DO_NOT_INCREASE__DNI = 'E';
  const char ExecInst_DO_NOT_REDUCE__DNR = 'F';
  const char ExecInst_ALL_OR_NONE__AON = 'G';
  const char ExecInst_INSTITUTIONS_ONLY = 'I';
  const char ExecInst_LAST_PEG = 'L';
  const char ExecInst_MIDPRICE_PEG = 'M';
  const char ExecInst_NONNEGOTIABLE = 'N';
  const char ExecInst_OPENING_PEG = 'O';
  const char ExecInst_PRIMARY_PEG = 'R';
  const char ExecInst_SUSPEND = 'S';
  const char ExecInst_FIXED_PEG = 'T';
  const char ExecInst_CUSTOMER_DISPLAY_INSTRUCTION = 'U';
  const char ExecInst_NETTING = 'V';
  const char ExecInst_PEG_TO_VWAP = 'W';
  const char ExecTransType_NEW = '0';
  const char ExecTransType_CANCEL = '1';
  const char ExecTransType_CORRECT = '2';
  const char ExecTransType_STATUS = '3';
  const char HandlInst_AUTOMATED_EXECUTION_ORDER_PRIVATE_NO_BROKER_INTERVENTION = '1';
  const char HandlInst_AUTOMATED_EXECUTION_ORDER_PUBLIC_BROKER_INTERVENTION_OK = '2';
  const char HandlInst_MANUAL_ORDER_BEST_EXECUTION = '3';
  const char IDSource_CUSIP = '1';
  const char IDSource_SEDOL = '2';
  const char IDSource_QUIK = '3';
  const char IDSource_ISIN_NUMBER = '4';
  const char IDSource_RIC_CODE = '5';
  const char IDSource_ISO_CURRENCY_CODE = '6';
  const char IDSource_ISO_COUNTRY_CODE = '7';
  const char IDSource_EXCHANGE_SYMBOL = '8';
  const char IDSource_CONSOLIDATED_TAPE_ASSOCIATION = '9';
  const char IOIQltyInd_LOW = 'L';
  const char IOIQltyInd_MEDIUM = 'M';
  const char IOIQltyInd_HIGH = 'H';
  const char IOIShares_SMALL = 'S';
  const char IOIShares_MEDIUM = 'M';
  const char IOIShares_LARGE = 'L';
  const char IOITransType_NEW = 'N';
  const char IOITransType_CANCEL = 'C';
  const char IOITransType_REPLACE = 'R';
  const char LastCapacity_AGENT = '1';
  const char LastCapacity_CROSS_AS_AGENT = '2';
  const char LastCapacity_CROSS_AS_PRINCIPAL = '3';
  const char LastCapacity_PRINCIPAL = '4';
  const char OrdStatus_NEW = '0';
  const char OrdStatus_PARTIALLY_FILLED = '1';
  const char OrdStatus_FILLED = '2';
  const char OrdStatus_DONE_FOR_DAY = '3';
  const char OrdStatus_CANCELED = '4';
  const char OrdStatus_REPLACED = '5';
  const char OrdStatus_PENDING_CANCEL = '6';
  const char OrdStatus_REJECTED = '8';
  const char OrdStatus_SUSPENDED = '9';
  const char OrdStatus_PENDING_NEW = 'A';
  const char OrdStatus_CALCULATED = 'B';
  const char OrdStatus_EXPIRED = 'C';
  const char OrdStatus_ACCEPTED_FOR_BIDDING = 'D';
  const char OrdStatus_PENDING_REPLACE = 'E';
  const char OrdType_MARKET = '1';
  const char OrdType_LIMIT = '2';
  const char OrdType_STOP = '3';
  const char OrdType_STOP_LIMIT = '4';
  const char OrdType_MARKET_ON_CLOSE = '5';
  const char OrdType_WITH_OR_WITHOUT = '6';
  const char OrdType_LIMIT_OR_BETTER = '7';
  const char OrdType_LIMIT_WITH_OR_WITHOUT = '8';
  const char OrdType_ON_BASIS = '9';
  const char OrdType_ON_CLOSE = 'A';
  const char OrdType_LIMIT_ON_CLOSE = 'B';
  const char OrdType_PREVIOUSLY_QUOTED = 'D';
  const char OrdType_PREVIOUSLY_INDICATED = 'E';
  const char OrdType_FOREX_LIMIT = 'F';
  const char OrdType_FOREX_SWAP = 'G';
  const char OrdType_FOREX_PREVIOUSLY_QUOTED = 'H';
  const char OrdType_FUNARI = 'I';
  const char OrdType_PEGGED = 'P';
  const char PossDupFlag_POSSIBLE_DUPLICATE = 'Y';
  const char PossDupFlag_ORIGINAL_TRANSMISSION = 'N';
  const char Rule80A_AGENCY_SINGLE_ORDER = 'A';
  const char Rule80A_PROGRAM_ORDER_NONINDEX_ARB_FOR_MEMBER_FIRMORG = 'C';
  const char Rule80A_PROGRAM_ORDER_INDEX_ARB_FOR_MEMBER_FIRMORG = 'D';
  const char Rule80A_REGISTERED_EQUITY_MARKET_MAKER_TRADES = 'E';
  const char Rule80A_PROGRAM_ORDER_INDEX_ARB_FOR_INDIVIDUAL_CUSTOMER = 'J';
  const char Rule80A_PROGRAM_ORDER_NONINDEX_ARB_FOR_INDIVIDUAL_CUSTOMER = 'K';
  const char Rule80A_SHORT_EXEMPT_AFFILIATED = 'L';
  const char Rule80A_PROGRAM_ORDER_INDEX_ARB_FOR_OTHER_MEMBER = 'M';
  const char Rule80A_PROGRAM_ORDER_NONINDEX_ARB_FOR_OTHER_MEMBER = 'N';
  const char Rule80A_PRINCIPAL = 'P';
  const char Rule80A_SPECIALIST_TRADES = 'S';
  const char Rule80A_PROGRAM_ORDER_INDEX_ARB_FOR_OTHER_AGENCY = 'U';
  const char Rule80A_ALL_OTHER_ORDERS_AS_AGENT_FOR_OTHER_MEMBER = 'W';
  const char Rule80A_SHORT_EXEMPT_NOT_AFFILIATED = 'X';
  const char Rule80A_PROGRAM_ORDER_NONINDEX_ARB_FOR_OTHER_AGENCY = 'Y';
  const char Rule80A_SHORT_EXEMPT_NONMEMBER = 'Z';
  const char Side_BUY = '1';
  const char Side_SELL = '2';
  const char Side_BUY_MINUS = '3';
  const char Side_SELL_PLUS = '4';
  const char Side_SELL_SHORT = '5';
  const char Side_SELL_SHORT_EXEMPT = '6';
  const char Side_CROSS = '8';
  const char Side_CROSS_SHORT = '9';
  const char TimeInForce_DAY = '0';
  const char TimeInForce_GOOD_TILL_CANCEL = '1';
  const char TimeInForce_AT_THE_OPENING = '2';
  const char TimeInForce_IMMEDIATE_OR_CANCEL = '3';
  const char TimeInForce_FILL_OR_KILL = '4';
  const char TimeInForce_GOOD_TILL_CROSSING = '5';
  const char TimeInForce_GOOD_TILL_DATE = '6';
  const char Urgency_NORMAL = '0';
  const char Urgency_FLASH = '1';
  const char Urgency_BACKGROUND = '2';
  const char SettlmntTyp_REGULAR = '0';
  const char SettlmntTyp_CASH = '1';
  const char SettlmntTyp_NEXT_DAY = '2';
  const char SettlmntTyp_TPLUS2 = '3';
  const char SettlmntTyp_TPLUS3 = '4';
  const char SettlmntTyp_TPLUS4 = '5';
  const char SettlmntTyp_FUTURE = '6';
  const char SettlmntTyp_WHEN_ISSUED = '7';
  const char SettlmntTyp_SELLERS_OPTION = '8';
  const char SettlmntTyp_TPLUS5 = '9';
  const char AllocTransType_NEW = '0';
  const char AllocTransType_REPLACE = '1';
  const char AllocTransType_CANCEL = '2';
  const char AllocTransType_PRELIMINARY = '3';
  const char AllocTransType_CALCULATED = '4';
  const char AllocTransType_CALCULATED_WITHOUT_PRELIMINARY = '5';
  const char ProcessCode_REGULAR = '0';
  const char ProcessCode_SOFT_DOLLAR = '1';
  const char ProcessCode_STEPIN = '2';
  const char ProcessCode_STEPOUT = '3';
  const char ProcessCode_SOFTDOLLAR_STEPIN = '4';
  const char ProcessCode_SOFTDOLLAR_STEPOUT = '5';
  const char ProcessCode_PLAN_SPONSOR = '6';
  const char AllocStatus_ACCEPTED = '0';
  const char AllocStatus_REJECTED = '1';
  const char AllocStatus_PARTIAL_ACCEPT = '2';
  const char AllocStatus_RECEIVED = '3';
  const char AllocRejCode_UNKNOWN_ACCOUNT = '0';
  const char AllocRejCode_INCORRECT_QUANTITY = '1';
  const char AllocRejCode_INCORRECT_AVERAGE_PRICE = '2';
  const char AllocRejCode_UNKNOWN_EXECUTING_BROKER_MNEMONIC = '3';
  const char AllocRejCode_COMMISSION_DIFFERENCE = '4';
  const char AllocRejCode_UNKNOWN_ORDERID = '5';
  const char AllocRejCode_UNKNOWN_LISTID = '6';
  const char AllocRejCode_OTHER = '7';
  const char EmailType_NEW = '0';
  const char EmailType_REPLY = '1';
  const char EmailType_ADMIN_REPLY = '2';
  const char EncryptMethod_NONE_OTHER = '0';
  const char EncryptMethod_PKCS = '1';
  const char EncryptMethod_DES = '2';
  const char EncryptMethod_PKCSDES = '3';
  const char EncryptMethod_PGPDES = '4';
  const char EncryptMethod_PGPDESMD5 = '5';
  const char EncryptMethod_PEMDESMD5 = '6';
  const char CxlRejReason_TOO_LATE_TO_CANCEL = '0';
  const char CxlRejReason_UNKNOWN_ORDER = '1';
  const char CxlRejReason_BROKER_OPTION = '2';
  const char CxlRejReason_ALREADY_PENDING = '3';
  const char OrdRejReason_BROKER_OPTION = '0';
  const char OrdRejReason_UNKNOWN_SYMBOL = '1';
  const char OrdRejReason_EXCHANGE_CLOSED = '2';
  const char OrdRejReason_ORDER_EXCEEDS_LIMIT = '3';
  const char OrdRejReason_TOO_LATE_TO_ENTER = '4';
  const char OrdRejReason_UNKNOWN_ORDER = '5';
  const char OrdRejReason_DUPLICATE_ORDER = '6';
  const char OrdRejReason_DUPLICATE_VERBALYES = '7';
  const char OrdRejReason_STALE_ORDER = '8';
  const char IOIQualifier_ALL_OR_NONE = 'A';
  const char IOIQualifier_AT_THE_CLOSE = 'C';
  const char IOIQualifier_IN_TOUCH_WITH = 'I';
  const char IOIQualifier_LIMIT = 'L';
  const char IOIQualifier_MORE_BEHIND = 'M';
  const char IOIQualifier_AT_THE_OPEN = 'O';
  const char IOIQualifier_TAKING_A_POSITION = 'P';
  const char IOIQualifier_AT_THE_MARKET = 'Q';
  const char IOIQualifier_READY_TO_TRADE = 'R';
  const char IOIQualifier_PORTFOLIO_SHOWN = 'S';
  const char IOIQualifier_THROUGH_THE_DAY = 'T';
  const char IOIQualifier_VERSUS = 'V';
  const char IOIQualifier_INDICATION__WORKING_AWAY = 'W';
  const char IOIQualifier_CROSSING_OPPORTUNITY = 'X';
  const char IOIQualifier_AT_THE_MIDPOINT = 'Y';
  const char IOIQualifier_PREOPEN = 'Z';
  const char ReportToExch_YES = 'Y';
  const char ReportToExch_NO = 'N';
  const char LocateReqd_YES = 'Y';
  const char LocateReqd_NO = 'N';
  const char ForexReq_YES = 'Y';
  const char ForexReq_NO = 'N';
  const char GapFillFlag_GAP_FILL_MESSAGE_MSGSEQNUM_FIELD_VALID = 'Y';
  const char GapFillFlag_SEQUENCE_RESET_IGNORE_MSGSEQNUM = 'N';
  const char DKReason_WRONG_SIDE = 'B';
  const char DKReason_QUANTITY_EXCEEDS_ORDER = 'C';
  const char DKReason_NO_MATCHING_ORDER = 'D';
  const char DKReason_PRICE_EXCEEDS_LIMIT = 'E';
  const char IOINaturalFlag_NATURAL = 'Y';
  const char IOINaturalFlag_NOT_NATURAL = 'N';
  const char MiscFeeType_REGULATORY = '1';
  const char MiscFeeType_TAX = '2';
  const char MiscFeeType_LOCAL_COMMISSION = '3';
  const char MiscFeeType_EXCHANGE_FEES = '4';
  const char MiscFeeType_STAMP = '5';
  const char MiscFeeType_LEVY = '6';
  const char MiscFeeType_OTHER = '7';
  const char MiscFeeType_MARKUP = '8';
  const char MiscFeeType_CONSUMPTION_TAX = '9';
  const char ResetSeqNumFlag_YES_RESET_SEQUENCE_NUMBERS = 'Y';
  const char ResetSeqNumFlag_NO = 'N';
  const char ExecType_NEW = '0';
  const char ExecType_PARTIAL_FILL = '1';
  const char ExecType_FILL = '2';
  const char ExecType_DONE_FOR_DAY = '3';
  const char ExecType_CANCELED = '4';
  const char ExecType_REPLACE = '5';
  const char ExecType_PENDING_CANCEL = '6';
  const char ExecType_STOPPED = '7';
  const char ExecType_REJECTED = '8';
  const char ExecType_SUSPENDED = '9';
  const char ExecType_PENDING_NEW = 'A';
  const char ExecType_CALCULATED = 'B';
  const char ExecType_EXPIRED = 'C';
  const char ExecType_RESTATED = 'D';
  const char ExecType_PENDING_REPLACE = 'E';
  const char SettlInstMode_DEFAULT = '0';
  const char SettlInstMode_STANDING_INSTRUCTIONS_PROVIDED = '1';
  const char SettlInstMode_SPECIFIC_ALLOCATION_ACCOUNT_OVERRIDING = '2';
  const char SettlInstMode_SPECIFIC_ALLOCATION_ACCOUNT_STANDING = '3';
  const char SettlInstTransType_NEW = 'N';
  const char SettlInstTransType_CANCEL = 'C';
  const char SettlInstTransType_REPLACE = 'R';
  const char SettlInstSource_BROKER = '1';
  const char SettlInstSource_INSTITUTION = '2';
  const char SecurityType_UNKNOWN = '?';
  const char StandInstDbType_OTHER = '0';
  const char StandInstDbType_DTC_SID = '1';
  const char StandInstDbType_THOMSON_ALERT = '2';
  const char StandInstDbType_A_GLOBAL_CUSTODIAN = '3';
  const char AllocLinkType_FX_NETTING = '0';
  const char AllocLinkType_FX_SWAP = '1';
  const char PutOrCall_PUT = '0';
  const char PutOrCall_CALL = '1';
  const char CoveredOrUncovered_COVERED = '0';
  const char CoveredOrUncovered_UNCOVERED = '1';
  const char CustomerOrFirm_CUSTOMER = '0';
  const char CustomerOrFirm_FIRM = '1';
  const char Benchmark_CURVE = '1';
  const char Benchmark_FIVEYR = '2';
  const char Benchmark_OLD5 = '3';
  const char Benchmark_TENYR = '4';
  const char Benchmark_OLD10 = '5';
  const char Benchmark_THIRTYYR = '6';
  const char Benchmark_OLD30 = '7';
  const char Benchmark_THREEMOLIBOR = '8';
  const char Benchmark_SIXMOLIBOR = '9';
  const char SubscriptionRequestType_SNAPSHOT = '0';
  const char SubscriptionRequestType_SNAPSHOT_PLUS_UPDATES = '1';
  const char SubscriptionRequestType_DISABLE_PREVIOUS = '2';
  const char MarketDepth_FULL_BOOK = '0';
  const char MarketDepth_TOP_OF_BOOK = '1';
  const char MDUpdateType_FULL_REFRESH = '0';
  const char MDUpdateType_INCREMENTAL_REFRESH = '1';
  const char AggregatedBook_ONE_BOOK_ENTRY_PER_SIDE_PER_PRICE = 'Y';
  const char AggregatedBook_MULTIPLE_ENTRIES_PER_SIDE_PER_PRICE_ALLOWED = 'N';
  const char MDEntryType_BID = '0';
  const char MDEntryType_OFFER = '1';
  const char MDEntryType_TRADE = '2';
  const char MDEntryType_INDEX_VALUE = '3';
  const char MDEntryType_OPENING_PRICE = '4';
  const char MDEntryType_CLOSING_PRICE = '5';
  const char MDEntryType_SETTLEMENT_PRICE = '6';
  const char MDEntryType_TRADING_SESSION_HIGH_PRICE = '7';
  const char MDEntryType_TRADING_SESSION_LOW_PRICE = '8';
  const char MDEntryType_TRADING_SESSION_VWAP_PRICE = '9';
  const char TickDirection_PLUS_TICK = '0';
  const char TickDirection_ZEROPLUS_TICK = '1';
  const char TickDirection_MINUS_TICK = '2';
  const char TickDirection_ZEROMINUS_TICK = '3';
  const char QuoteCondition_OPEN__ACTIVE = 'A';
  const char QuoteCondition_CLOSED__INACTIVE = 'B';
  const char QuoteCondition_EXCHANGE_BEST = 'C';
  const char QuoteCondition_CONSOLIDATED_BEST = 'D';
  const char QuoteCondition_LOCKED = 'E';
  const char QuoteCondition_CROSSED = 'F';
  const char QuoteCondition_DEPTH = 'G';
  const char QuoteCondition_FAST_TRADING = 'H';
  const char QuoteCondition_NONFIRM = 'I';
  const char TradeCondition_CASH = 'A';
  const char TradeCondition_AVERAGE_PRICE_TRADE = 'B';
  const char TradeCondition_CASH_TRADE = 'C';
  const char TradeCondition_NEXT_DAY = 'D';
  const char TradeCondition_OPENING__REOPENING_TRADE_DETAIL = 'E';
  const char TradeCondition_INTRADAY_TRADE_DETAIL = 'F';
  const char TradeCondition_RULE_127_TRADE = 'G';
  const char TradeCondition_RULE_155_TRADE = 'H';
  const char TradeCondition_SOLD_LAST = 'I';
  const char TradeCondition_NEXT_DAY_TRADE = 'J';
  const char TradeCondition_OPENED = 'K';
  const char TradeCondition_SELLER = 'L';
  const char TradeCondition_SOLD = 'M';
  const char TradeCondition_STOPPED_STOCK = 'N';
  const char MDUpdateAction_NEW = '0';
  const char MDUpdateAction_CHANGE = '1';
  const char MDUpdateAction_DELETE = '2';
  const char MDReqRejReason_UNKNOWN_SYMBOL = '0';
  const char MDReqRejReason_DUPLICATE_MDREQID = '1';
  const char MDReqRejReason_INSUFFICIENT_BANDWIDTH = '2';
  const char MDReqRejReason_INSUFFICIENT_PERMISSIONS = '3';
  const char MDReqRejReason_UNSUPPORTED_SUBSCRIPTIONREQUESTTYPE = '4';
  const char MDReqRejReason_UNSUPPORTED_MARKETDEPTH = '5';
  const char MDReqRejReason_UNSUPPORTED_MDUPDATETYPE = '6';
  const char MDReqRejReason_UNSUPPORTED_AGGREGATEDBOOK = '7';
  const char MDReqRejReason_UNSUPPORTED_MDENTRYTYPE = '8';
  const char DeleteReason_CANCELATION__TRADE_BUST = '0';
  const char DeleteReason_ERROR = '1';
  const char OpenCloseSettleFlag_DAILY_OPEN__CLOSE__SETTLEMENT_PRICE = '0';
  const char OpenCloseSettleFlag_SESSION_OPEN__CLOSE__SETTLEMENT_PRICE = '1';
  const char OpenCloseSettleFlag_DELIVERY_SETTLEMENT_PRICE = '2';
  const char SessionRejectReason_INVALID_TAG_NUMBER = '0';
  const char SessionRejectReason_REQUIRED_TAG_MISSING = '1';
  const char SessionRejectReason_TAG_NOT_DEFINED_FOR_THIS_MESSAGE_TYPE = '2';
  const char SessionRejectReason_UNDEFINED_TAG = '3';
  const char SessionRejectReason_TAG_SPECIFIED_WITHOUT_A_VALUE = '4';
  const char SessionRejectReason_VALUE_IS_INCORRECT = '5';
  const char SessionRejectReason_INCORRECT_DATA_FORMAT_FOR_VALUE = '6';
  const char SessionRejectReason_DECRYPTION_PROBLEM = '7';
  const char SessionRejectReason_SIGNATURE_PROBLEM = '8';
  const char SessionRejectReason_COMPID_PROBLEM = '9';
  const char BidRequestTransType_NEW = 'N';
  const char BidRequestTransType_CANCEL = 'C';
  const char SolicitedFlag_WAS_SOLCITIED = 'Y';
  const char SolicitedFlag_WAS_NOT_SOLICITED = 'N';
  const char ExecRestatementReason_GT_CORPORATE_ACTION = '0';
  const char ExecRestatementReason_GT_RENEWAL__RESTATEMENT = '1';
  const char ExecRestatementReason_VERBAL_CHANGE = '2';
  const char ExecRestatementReason_REPRICING_OF_ORDER = '3';
  const char ExecRestatementReason_BROKER_OPTION = '4';
  const char ExecRestatementReason_PARTIAL_DECLINE_OF_ORDERQTY = '5';
  const char BusinessRejectReason_OTHER = '0';
  const char BusinessRejectReason_UNKOWN_ID = '1';
  const char BusinessRejectReason_UNKNOWN_SECURITY = '2';
  const char BusinessRejectReason_UNSUPPORTED_MESSAGE_TYPE = '3';
  const char BusinessRejectReason_APPLICATION_NOT_AVAILABLE = '4';
  const char BusinessRejectReason_CONDITIONALLY_REQUIRED_FIELD_MISSING = '5';
  const char MsgDirection_SEND = 'S';
  const char MsgDirection_RECEIVE = 'R';
  const char DiscretionInst_RELATED_TO_DISPLAYED_PRICE = '0';
  const char DiscretionInst_RELATED_TO_MARKET_PRICE = '1';
  const char DiscretionInst_RELATED_TO_PRIMARY_PRICE = '2';
  const char DiscretionInst_RELATED_TO_LOCAL_PRIMARY_PRICE = '3';
  const char DiscretionInst_RELATED_TO_MIDPOINT_PRICE = '4';
  const char DiscretionInst_RELATED_TO_LAST_TRADE_PRICE = '5';
  const char GTBookingInst_BOOK_OUT_ALL_TRADES_ON_DAY_OF_EXECUTION = '0';
  const char GTBookingInst_ACCUMULATE_EXECUTIONS_UNTIL_ORDER_IS_FILLED_OR_EXPIRES = '1';
  const char GTBookingInst_ACCUMULATE_UNTIL_VERBALLY_NOTIFIED_OTHERWISE = '2';
}
#endif //FIX_VALUES_H
 