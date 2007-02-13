object Form3: TForm3
  Left = 119
  Top = 128
  Caption = 'Add New Fuzzy Component'
  ClientHeight = 135
  ClientWidth = 527
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 32
    Top = 0
    Width = 27
    Height = 13
    Caption = 'Name'
  end
  object Label2: TLabel
    Left = 32
    Top = 72
    Width = 53
    Height = 13
    Caption = 'Description'
  end
  object Label3: TLabel
    Left = 32
    Top = 40
    Width = 69
    Height = 13
    Caption = 'Minimum Value'
  end
  object Label4: TLabel
    Left = 181
    Top = 40
    Width = 73
    Height = 13
    Caption = 'Maximum Value'
  end
  object Name: TEdit
    Left = 120
    Top = 0
    Width = 121
    Height = 21
    TabOrder = 0
    Text = 'Name'
  end
  object Description: TEdit
    Left = 120
    Top = 72
    Width = 393
    Height = 21
    TabOrder = 1
    Text = 'Description'
  end
  object BitBtn1: TBitBtn
    Left = 367
    Top = 99
    Width = 75
    Height = 25
    TabOrder = 2
    Kind = bkOK
  end
  object BitBtn2: TBitBtn
    Left = 448
    Top = 99
    Width = 75
    Height = 25
    TabOrder = 3
    Kind = bkCancel
  end
  object Min: TEdit
    Left = 120
    Top = 37
    Width = 49
    Height = 21
    TabOrder = 4
    Text = '0'
  end
  object Max: TEdit
    Left = 285
    Top = 37
    Width = 49
    Height = 21
    TabOrder = 5
    Text = '500'
  end
end
