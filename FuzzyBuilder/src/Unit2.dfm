object Form2: TForm2
  Left = 0
  Top = 0
  Caption = 'Add New Logic Operation'
  ClientHeight = 116
  ClientWidth = 564
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 16
    Top = 8
    Width = 51
    Height = 13
    Caption = 'Logic Type'
  end
  object Label2: TLabel
    Left = 16
    Top = 40
    Width = 53
    Height = 13
    Caption = 'Description'
  end
  object LogicSelection: TComboBox
    Left = 117
    Top = 8
    Width = 159
    Height = 21
    ItemHeight = 13
    ItemIndex = 0
    TabOrder = 0
    Text = 'In'
    Items.Strings = (
      'In'
      'Out'
      'Not'
      'Or'
      'Xor '
      'And')
  end
  object BitBtn1: TBitBtn
    Left = 407
    Top = 83
    Width = 75
    Height = 25
    TabOrder = 1
    Kind = bkOK
  end
  object BitBtn2: TBitBtn
    Left = 488
    Top = 83
    Width = 75
    Height = 25
    TabOrder = 2
    Kind = bkCancel
  end
  object Description: TEdit
    Left = 117
    Top = 35
    Width = 393
    Height = 21
    TabOrder = 3
    Text = 'Description'
  end
end
