object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 552
  ClientWidth = 1009
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 120
  TextHeight = 16
  object Button1: TButton
    Left = 16
    Top = 80
    Width = 75
    Height = 25
    Caption = 'CryString'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Panel1: TPanel
    Left = 128
    Top = 0
    Width = 881
    Height = 552
    Align = alRight
    Caption = 'Panel1'
    TabOrder = 1
    object Memo1: TMemo
      Left = 1
      Top = 1
      Width = 879
      Height = 550
      Align = alClient
      Lines.Strings = (
        '')
      TabOrder = 0
    end
  end
  object Button2: TButton
    Left = 8
    Top = 280
    Width = 99
    Height = 25
    Caption = 'BackProp Test'
    TabOrder = 2
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 16
    Top = 8
    Width = 75
    Height = 25
    Caption = 'CryObject'
    TabOrder = 3
    OnClick = Button3Click
  end
  object Button4: TButton
    Left = 16
    Top = 39
    Width = 75
    Height = 25
    Caption = 'CryArray'
    TabOrder = 4
    OnClick = Button4Click
  end
  object Button5: TButton
    Left = 16
    Top = 128
    Width = 75
    Height = 25
    Caption = 'CrySet'
    TabOrder = 5
    OnClick = Button5Click
  end
  object Button6: TButton
    Left = 16
    Top = 175
    Width = 75
    Height = 25
    Caption = 'CryList'
    TabOrder = 6
    OnClick = Button6Click
  end
  object Button7: TButton
    Left = 16
    Top = 206
    Width = 107
    Height = 25
    Caption = 'Strategy Pattern'
    TabOrder = 7
    OnClick = Button7Click
  end
  object Button8: TButton
    Left = 16
    Top = 237
    Width = 106
    Height = 25
    Caption = 'Observer Pattern'
    TabOrder = 8
    OnClick = Button8Click
  end
end
