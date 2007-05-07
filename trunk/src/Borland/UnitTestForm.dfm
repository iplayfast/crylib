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
  object Panel1: TPanel
    Left = 144
    Top = 0
    Width = 613
    Height = 414
    Align = alRight
    Caption = 'Panel1'
    TabOrder = 0
    object Memo1: TMemo
      Left = 1
      Top = 1
      Width = 611
      Height = 412
      Align = alClient
      Lines.Strings = (
        '')
      TabOrder = 0
    end
  end
  object CheckListBox1: TCheckListBox
    Left = 0
    Top = 66
    Width = 121
    Height = 286
    ItemHeight = 12
    Items.Strings = (
      'Object'
      'Array<int>'
      'Array<float>'
      'String'
      'CrySet'
      'List'
      'Strategy Pattern'
      'Observer Pattern'
      'BackProp Nural Net'
      'BitArray'
      'Fuzzy Logic'
      'Test')
    TabOrder = 1
    OnClick = CheckListBox1Click
  end
  object Button10: TButton
    Left = 24
    Top = 368
    Width = 75
    Height = 25
    Caption = 'Test All'
    TabOrder = 2
    OnClick = Button10Click
  end
end
