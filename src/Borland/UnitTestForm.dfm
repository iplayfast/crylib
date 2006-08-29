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
      ExplicitLeft = 216
      ExplicitTop = 8
      ExplicitWidth = 409
      ExplicitHeight = 273
    end
  end
  object Button2: TButton
    Left = 8
    Top = 168
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
end
