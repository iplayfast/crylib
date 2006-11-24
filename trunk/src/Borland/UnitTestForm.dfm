object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 414
  ClientWidth = 757
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -10
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 12
  object Button1: TButton
    Left = 12
    Top = 60
    Width = 56
    Height = 19
    Caption = 'CryString'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Panel1: TPanel
    Left = 96
    Top = 0
    Width = 661
    Height = 414
    Align = alRight
    Caption = 'Panel1'
    TabOrder = 1
    object Memo1: TMemo
      Left = 1
      Top = 1
      Width = 659
      Height = 412
      Align = alClient
      Lines.Strings = (
        '')
      TabOrder = 0
    end
  end
  object Button2: TButton
    Left = 6
    Top = 210
    Width = 74
    Height = 19
    Caption = 'BackProp Test'
    TabOrder = 2
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 12
    Top = 6
    Width = 56
    Height = 19
    Caption = 'CryObject'
    TabOrder = 3
    OnClick = Button3Click
  end
  object Button4: TButton
    Left = 12
    Top = 29
    Width = 56
    Height = 19
    Caption = 'CryArray'
    TabOrder = 4
    OnClick = Button4Click
  end
  object Button5: TButton
    Left = 12
    Top = 96
    Width = 56
    Height = 19
    Caption = 'CrySet'
    TabOrder = 5
    OnClick = Button5Click
  end
  object Button6: TButton
    Left = 12
    Top = 131
    Width = 56
    Height = 19
    Caption = 'CryList'
    TabOrder = 6
    OnClick = Button6Click
  end
  object Button7: TButton
    Left = 12
    Top = 155
    Width = 80
    Height = 18
    Caption = 'Strategy Pattern'
    TabOrder = 7
    OnClick = Button7Click
  end
  object Button8: TButton
    Left = 12
    Top = 178
    Width = 80
    Height = 19
    Caption = 'Observer Pattern'
    TabOrder = 8
    OnClick = Button8Click
  end
end
