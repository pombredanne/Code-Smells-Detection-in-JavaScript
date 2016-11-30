object ToolEditForm: TToolEditForm
  Left = 778
  Top = 377
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Edit tool'
  ClientHeight = 304
  ClientWidth = 484
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  DesignSize = (
    484
    304)
  PixelsPerInch = 96
  TextHeight = 15
  object lblTitle: TLabel
    Left = 8
    Top = 12
    Width = 23
    Height = 15
    Caption = 'Title'
  end
  object lblProg: TLabel
    Left = 8
    Top = 40
    Width = 46
    Height = 15
    Caption = 'Program'
  end
  object lblWorkDir: TLabel
    Left = 8
    Top = 69
    Width = 96
    Height = 15
    Caption = 'Working Directory'
  end
  object lblParam: TLabel
    Left = 8
    Top = 98
    Width = 59
    Height = 15
    Caption = 'Parameters'
  end
  object lblMacros: TLabel
    Left = 7
    Top = 157
    Width = 93
    Height = 15
    Caption = 'Available Macros:'
  end
  object btnProg: TSpeedButton
    Left = 451
    Top = 36
    Width = 23
    Height = 22
    Glyph.Data = {
      36030000424D3603000000000000360000002800000010000000100000000100
      18000000000000030000120B0000120B00000000000000000000BFBFBFBFBFBF
      BFBFBFBFBFBFBFBFBF000000BFBFBFBFBFBFBFBFBFBFBFBF0000000000000000
      00000000000000BFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBF000000BF
      BFBF000000BFBFBF0000005DCCFF5DCCFF5DCCFF000000BFBFBFBFBFBFBFBFBF
      BFBFBFBFBFBFBFBFBF000000BFBFBFBFBFBFBFBFBFBFBFBF6868680000000000
      00000000000000BFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBF
      BFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBF
      BFBFBFBFBFBFBFBFBF000000BFBFBFBFBFBFBFBFBFBFBFBF0000000000000000
      00000000000000BFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBF000000BF
      BFBF000000BFBFBF0000005DCCFF5DCCFF5DCCFF000000BFBFBFBFBFBFBFBFBF
      BFBFBFBFBFBFBFBFBF000000BFBFBFBFBFBFBFBFBFBFBFBF6868680000000000
      00000000000000BFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBF
      BFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBF000000000000
      000000000000000000000000000000000000000000000000000000BFBFBFBFBF
      BFBFBFBFBFBFBFBFBFBF00000000AEFF0096DB0096DB0096DB0096DB0096DB00
      96DB0096DB0082BE000000BFBFBFBFBFBFBFBFBFBFBFBFBFBFBF0000005DCCFF
      00AEFF00AEFF00AEFF00AEFF00AEFF00AEFF00AEFF0096DB000000BFBFBFBFBF
      BFBFBFBFBFBFBFBFBFBF0000005DCCFF00AEFF00AEFF00AEFF00AEFF00AEFF00
      AEFF00AEFF0096DB000000BFBFBFBFBFBFBFBFBFBFBFBFBFBFBF0000005DCCFF
      00AEFF00AEFF00AEFF00AEFF00AEFF00AEFF00AEFF0096DB000000BFBFBFBFBF
      BFBFBFBFBFBFBFBFBFBF0000005DCCFF00AEFF00AEFF5DCCFF5DCCFF5DCCFF5D
      CCFF5DCCFF00AEFF000000BFBFBFBFBFBFBFBFBFBFBFBFBFBFBF686868BDEBFF
      5DCCFF5DCCFF000000000000000000000000000000000000BFBFBFBFBFBFBFBF
      BFBFBFBFBFBFBFBFBFBFBFBFBF000000000000000000BFBFBFBFBFBFBFBFBFBF
      BFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBF}
    OnClick = btnProgClick
  end
  object btnWorkDir: TSpeedButton
    Left = 451
    Top = 65
    Width = 23
    Height = 22
    Glyph.Data = {
      36030000424D3603000000000000360000002800000010000000100000000100
      18000000000000030000120B0000120B00000000000000000000BFBFBFBFBFBF
      BFBFBFBFBFBFBFBFBF000000BFBFBFBFBFBFBFBFBFBFBFBF0000000000000000
      00000000000000BFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBF000000BF
      BFBF000000BFBFBF0000005DCCFF5DCCFF5DCCFF000000BFBFBFBFBFBFBFBFBF
      BFBFBFBFBFBFBFBFBF000000BFBFBFBFBFBFBFBFBFBFBFBF6868680000000000
      00000000000000BFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBF
      BFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBF
      BFBFBFBFBFBFBFBFBF000000BFBFBFBFBFBFBFBFBFBFBFBF0000000000000000
      00000000000000BFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBF000000BF
      BFBF000000BFBFBF0000005DCCFF5DCCFF5DCCFF000000BFBFBFBFBFBFBFBFBF
      BFBFBFBFBFBFBFBFBF000000BFBFBFBFBFBFBFBFBFBFBFBF6868680000000000
      00000000000000BFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBF
      BFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBF000000000000
      000000000000000000000000000000000000000000000000000000BFBFBFBFBF
      BFBFBFBFBFBFBFBFBFBF00000000AEFF0096DB0096DB0096DB0096DB0096DB00
      96DB0096DB0082BE000000BFBFBFBFBFBFBFBFBFBFBFBFBFBFBF0000005DCCFF
      00AEFF00AEFF00AEFF00AEFF00AEFF00AEFF00AEFF0096DB000000BFBFBFBFBF
      BFBFBFBFBFBFBFBFBFBF0000005DCCFF00AEFF00AEFF00AEFF00AEFF00AEFF00
      AEFF00AEFF0096DB000000BFBFBFBFBFBFBFBFBFBFBFBFBFBFBF0000005DCCFF
      00AEFF00AEFF00AEFF00AEFF00AEFF00AEFF00AEFF0096DB000000BFBFBFBFBF
      BFBFBFBFBFBFBFBFBFBF0000005DCCFF00AEFF00AEFF5DCCFF5DCCFF5DCCFF5D
      CCFF5DCCFF00AEFF000000BFBFBFBFBFBFBFBFBFBFBFBFBFBFBF686868BDEBFF
      5DCCFF5DCCFF000000000000000000000000000000000000BFBFBFBFBFBFBFBF
      BFBFBFBFBFBFBFBFBFBFBFBFBF000000000000000000BFBFBFBFBFBFBFBFBFBF
      BFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBF}
    OnClick = btnWorkDirClick
  end
  object lblDesc: TMemo
    Left = 168
    Top = 180
    Width = 305
    Height = 51
    Color = clBtnFace
    Enabled = False
    ReadOnly = True
    TabOrder = 10
  end
  object edTitle: TEdit
    Left = 120
    Top = 8
    Width = 353
    Height = 23
    TabOrder = 0
  end
  object edProgram: TEdit
    Left = 120
    Top = 36
    Width = 321
    Height = 23
    TabOrder = 1
    OnChange = edProgramChange
    OnEnter = EditEnter
  end
  object edWorkDir: TEdit
    Left = 120
    Top = 65
    Width = 321
    Height = 23
    TabOrder = 2
    OnEnter = EditEnter
  end
  object edParams: TEdit
    Left = 120
    Top = 94
    Width = 353
    Height = 23
    TabOrder = 3
    OnChange = edParamsChange
    OnEnter = EditEnter
  end
  object btnCancel: TBitBtn
    Left = 304
    Top = 272
    Width = 85
    Height = 24
    Anchors = [akLeft, akBottom]
    Caption = '&Cancel'
    TabOrder = 8
    OnClick = btnCancelClick
    Kind = bkCancel
  end
  object btnOk: TBitBtn
    Left = 214
    Top = 272
    Width = 81
    Height = 24
    Anchors = [akLeft, akBottom]
    Caption = '&OK'
    TabOrder = 7
    Kind = bkOK
  end
  object lstMacro: TListBox
    Left = 8
    Top = 180
    Width = 153
    Height = 79
    Color = 14548990
    IntegralHeight = True
    ItemHeight = 15
    Items.Strings = (
      '<DEFAULT>'
      '<EXECPATH>'
      '<EXENAME>'
      '<PROJECTPATH>'
      '<PROJECTFILE>'
      '<PROJECTNAME>'
      '<SOURCENAME>'
      '<SOURCEPATH>'
      '<SOURCESPCLIST>'
      '<WORDXY>'
      '<DEVCPPVERSION>')
    TabOrder = 4
    OnClick = lstMacroClick
    OnDblClick = btnInsertClick
  end
  object btnInsert: TBitBtn
    Left = 170
    Top = 236
    Width = 103
    Height = 24
    Caption = '&Insert Macro'
    TabOrder = 5
    OnClick = btnInsertClick
    Glyph.Data = {
      76010000424D7601000000000000760000002800000020000000100000000100
      04000000000000010000130B0000130B00001000000000000000000000000000
      800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
      33333333FF33333333FF333993333333300033377F3333333777333993333333
      300033F77FFF3333377739999993333333333777777F3333333F399999933333
      33003777777333333377333993333333330033377F3333333377333993333333
      3333333773333333333F333333333333330033333333F33333773333333C3333
      330033333337FF3333773333333CC333333333FFFFF77FFF3FF33CCCCCCCCCC3
      993337777777777F77F33CCCCCCCCCC3993337777777777377333333333CC333
      333333333337733333FF3333333C333330003333333733333777333333333333
      3000333333333333377733333333333333333333333333333333}
    NumGlyphs = 2
  end
  object btnHelp: TBitBtn
    Left = 399
    Top = 272
    Width = 75
    Height = 24
    Anchors = [akLeft, akBottom]
    TabOrder = 6
    OnClick = HelpClick
    Kind = bkHelp
  end
  object ParamText: TEdit
    Left = 8
    Top = 128
    Width = 465
    Height = 23
    AutoSize = False
    Color = clBtnFace
    ReadOnly = True
    TabOrder = 9
  end
end