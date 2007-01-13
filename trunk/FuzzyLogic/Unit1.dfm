object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 655
  ClientWidth = 709
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnShow = Refresh
  PixelsPerInch = 96
  TextHeight = 13
  object Fuzzy1: TPanel
    Left = 0
    Top = 167
    Width = 709
    Height = 128
    Align = alTop
    Color = clTeal
    TabOrder = 0
    object Label2: TLabel
      Left = 20
      Top = 6
      Width = 26
      Height = 13
      Caption = 'Input'
    end
    object Label1: TLabel
      Left = 12
      Top = 70
      Width = 34
      Height = 13
      Caption = 'Output'
    end
    object Label6: TLabel
      Left = 256
      Top = 10
      Width = 27
      Height = 13
      Caption = 'Name'
    end
    object Input1: TTrackBar
      Left = 12
      Top = 24
      Width = 468
      Height = 30
      Max = 1000
      Frequency = 200
      TabOrder = 0
      OnChange = Input1Change
      OnEnter = Input1Enter
      OnExit = Input1Exit
    end
    object Output1: TTrackBar
      Left = 12
      Top = 89
      Width = 468
      Height = 30
      Max = 1000
      Frequency = 200
      TabOrder = 1
      OnChange = Output1Change
      OnEnter = Output1Enter
    end
    object Button3: TButton
      Left = 486
      Top = 84
      Width = 75
      Height = 25
      Caption = 'Clear Fuzzy 1'
      TabOrder = 2
      OnClick = Button3Click
    end
    object Button2: TButton
      Left = 485
      Top = 30
      Width = 140
      Height = 25
      Caption = 'Add Values to Fuzzy 1'
      TabOrder = 3
      OnClick = Button2Click
    end
    object Button1: TButton
      Left = 567
      Top = 84
      Width = 75
      Height = 25
      Caption = 'Optimize'
      TabOrder = 4
      OnClick = Button1Click
    end
    object Fuzzy1Name: TEdit
      Left = 289
      Top = 6
      Width = 121
      Height = 21
      TabOrder = 5
      Text = 'Fuzzy1'
    end
  end
  object Logic: TPanel
    Left = 0
    Top = 477
    Width = 709
    Height = 178
    Align = alBottom
    Color = clRed
    TabOrder = 1
    ExplicitTop = 458
    object Label3: TLabel
      Left = 11
      Top = 37
      Width = 56
      Height = 13
      Caption = 'Fuzzy 1 out'
    end
    object Label8: TLabel
      Left = 11
      Top = 87
      Width = 56
      Height = 13
      Caption = 'Fuzzy 2 out'
    end
    object Label9: TLabel
      Left = 12
      Top = 137
      Width = 45
      Height = 13
      Caption = 'Logic Out'
    end
    object LogicInText: TLabel
      Left = 488
      Top = 8
      Width = 6
      Height = 13
      Caption = '0'
    end
    object FuzzyOut2: TProgressBar
      Left = 12
      Top = 106
      Width = 453
      Height = 16
      Max = 1000
      Smooth = True
      Step = 1
      TabOrder = 0
    end
    object LogicOut: TProgressBar
      Left = 11
      Top = 156
      Width = 453
      Height = 16
      Max = 1000
      Smooth = True
      TabOrder = 1
    end
    object FuzzyOut1: TProgressBar
      Left = 12
      Top = 56
      Width = 453
      Height = 16
      Max = 1000
      Smooth = True
      TabOrder = 2
    end
    object LogicIn: TTrackBar
      Left = 0
      Top = 6
      Width = 468
      Height = 30
      Max = 1000
      Frequency = 200
      TabOrder = 3
      TickStyle = tsNone
      OnChange = LogicInChange
    end
    object LogicSelection: TComboBox
      Left = 544
      Top = 6
      Width = 145
      Height = 21
      ItemHeight = 13
      TabOrder = 4
      Text = 'Fuzzy 1 And Fuzzy 2'
      OnChange = Refresh
      Items.Strings = (
        'Fuzzy 1 And Fuzzy 2'
        'Fuzzy 1 Or Fuzzy 2'
        'Not Fuzzy 1'
        'Not Fuzzy 2'
        'Fuzzy 1 Xor Fuzzy 2'
        'Inhibite Fuzzy 1 based on Fuzzy 2')
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 709
    Height = 167
    Align = alTop
    Caption = 'Panel2'
    TabOrder = 2
    object Bevel1: TBevel
      Left = 496
      Top = 8
      Width = 201
      Height = 66
    end
    object MaxValue: TLabel
      Left = 538
      Top = 37
      Width = 24
      Height = 13
      Caption = '1000'
    end
    object Label10: TLabel
      Left = 506
      Top = 18
      Width = 84
      Height = 13
      Caption = 'Maximum Allowed'
    end
    object grid: TStringGrid
      Left = -32
      Top = 8
      Width = 409
      Height = 169
      DefaultColWidth = 80
      TabOrder = 0
    end
    object UpDown1: TUpDown
      Left = 605
      Top = 18
      Width = 16
      Height = 48
      Max = 10000
      Increment = 1000
      Position = 1000
      TabOrder = 1
      Wrap = True
      OnClick = UpDown1Click
    end
    object UpDown2: TUpDown
      Left = 627
      Top = 18
      Width = 16
      Height = 48
      Max = 900
      Increment = 100
      TabOrder = 2
      Wrap = True
      OnClick = UpDown1Click
    end
    object UpDown3: TUpDown
      Left = 651
      Top = 18
      Width = 16
      Height = 48
      Max = 90
      Increment = 10
      TabOrder = 3
      Wrap = True
      OnClick = UpDown1Click
    end
    object UpDown4: TUpDown
      Left = 673
      Top = 18
      Width = 16
      Height = 48
      Max = 9
      TabOrder = 4
      Wrap = True
      OnClick = UpDown1Click
    end
    object Chart1: TChart
      Left = 0
      Top = 2
      Width = 490
      Height = 165
      AnimatedZoom = True
      BackWall.Brush.Color = clWhite
      BackWall.Brush.Style = bsClear
      Title.Text.Strings = (
        'Fuzzy Values')
      View3D = False
      TabOrder = 5
      Visible = False
      object Series1: TLineSeries
        Marks.ArrowLength = 8
        Marks.Visible = False
        SeriesColor = clTeal
        Title = 'Fuzzy 1'
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        Pointer.Visible = False
        XValues.DateTime = False
        XValues.Name = 'X'
        XValues.Multiplier = 1.000000000000000000
        XValues.Order = loAscending
        YValues.DateTime = False
        YValues.Name = 'Y'
        YValues.Multiplier = 1.000000000000000000
        YValues.Order = loNone
      end
      object Series2: TLineSeries
        Marks.ArrowLength = 8
        Marks.Visible = False
        SeriesColor = clLime
        Title = 'Fuzzy 2'
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        Pointer.Visible = False
        XValues.DateTime = False
        XValues.Name = 'X'
        XValues.Multiplier = 1.000000000000000000
        XValues.Order = loAscending
        YValues.DateTime = False
        YValues.Name = 'Y'
        YValues.Multiplier = 1.000000000000000000
        YValues.Order = loNone
      end
      object Series3: TLineSeries
        Marks.ArrowLength = 8
        Marks.Visible = False
        SeriesColor = clRed
        Title = 'Logic'
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        Pointer.Visible = False
        XValues.DateTime = False
        XValues.Name = 'X'
        XValues.Multiplier = 1.000000000000000000
        XValues.Order = loAscending
        YValues.DateTime = False
        YValues.Name = 'Y'
        YValues.Multiplier = 1.000000000000000000
        YValues.Order = loNone
      end
    end
    object RadioGroup1: TRadioGroup
      Left = 496
      Top = 80
      Width = 65
      Height = 57
      Caption = 'Data View'
      ItemIndex = 0
      Items.Strings = (
        'Text'
        'Chart')
      TabOrder = 6
      OnClick = RadioGroup1Click
    end
    object CheckBox3d: TCheckBox
      Left = 591
      Top = 88
      Width = 34
      Height = 17
      Caption = '3d'
      TabOrder = 7
      Visible = False
      OnClick = CheckBox3dClick
    end
    object UpDown5: TUpDown
      Left = 591
      Top = 111
      Width = 34
      Height = 16
      Min = 1
      Orientation = udHorizontal
      Position = 40
      TabOrder = 8
      Visible = False
      OnClick = UpDown5Click
    end
  end
  object Fuzzy2: TPanel
    Left = 0
    Top = 295
    Width = 709
    Height = 117
    Align = alTop
    Color = clLime
    TabOrder = 3
    object Label4: TLabel
      Left = 20
      Top = 6
      Width = 26
      Height = 13
      Caption = 'Input'
    end
    object Label5: TLabel
      Left = 12
      Top = 61
      Width = 34
      Height = 13
      Caption = 'Output'
    end
    object Label7: TLabel
      Left = 256
      Top = 10
      Width = 27
      Height = 13
      Caption = 'Name'
    end
    object Input2: TTrackBar
      Left = 12
      Top = 25
      Width = 468
      Height = 30
      Max = 1000
      Frequency = 200
      TabOrder = 0
      OnChange = Input2Change
      OnEnter = Input2Enter
      OnExit = Input2Exit
    end
    object Output2: TTrackBar
      Left = 12
      Top = 80
      Width = 468
      Height = 30
      Max = 1000
      Frequency = 200
      TabOrder = 1
      OnChange = Output2Change
      OnEnter = Output1Enter
    end
    object Button4: TButton
      Tag = 1
      Left = 486
      Top = 76
      Width = 75
      Height = 25
      Caption = 'Clear Fuzzy 2'
      TabOrder = 2
      OnClick = Button4Click
    end
    object Button5: TButton
      Tag = 1
      Left = 485
      Top = 22
      Width = 140
      Height = 25
      Caption = 'Add Values to Fuzzy 2'
      TabOrder = 3
      OnClick = Button5Click
    end
    object Button6: TButton
      Left = 567
      Top = 76
      Width = 75
      Height = 25
      Caption = 'Optimize'
      TabOrder = 4
      OnClick = Button6Click
    end
    object Fuzzy2Name: TEdit
      Left = 289
      Top = 6
      Width = 121
      Height = 21
      TabOrder = 5
      Text = 'Fuzzy2'
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 412
    Width = 709
    Height = 65
    Align = alClient
    TabOrder = 4
    ExplicitHeight = 46
  end
  object MainMenu1: TMainMenu
    Left = 608
    Top = 8
    object File1: TMenuItem
      Caption = '&File'
      object New1: TMenuItem
        Caption = '&New'
        OnClick = New1Click
      end
      object Open1: TMenuItem
        Caption = '&Open...'
        OnClick = Open1Click
      end
      object Save1: TMenuItem
        Caption = '&Save'
        OnClick = Save1Click
      end
      object SaveAs1: TMenuItem
        Caption = 'Save &As...'
        OnClick = SaveAs1Click
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object Exit1: TMenuItem
        Caption = 'E&xit'
        OnClick = Exit1Click
      end
    end
    object Complexity1: TMenuItem
      Caption = 'Complexity'
      object OneFuzzyLogic1: TMenuItem
        Caption = 'One Fuzzy Logic'
        Checked = True
        OnClick = OneFuzzyLogic1Click
      end
      object TwoFuzzyLogics1: TMenuItem
        Caption = 'Two Fuzzy Logics'
        OnClick = TwoFuzzyLogics1Click
      end
      object FuzzyLogicOperations1: TMenuItem
        Caption = 'Fuzzy Logic Operations'
        OnClick = FuzzyLogicOperations1Click
      end
    end
  end
  object OpenTextFileDialog1: TOpenTextFileDialog
    FileName = 'FuzzyValues.h'
    Filter = 'Include|*.h|Text|*.txt'
    Left = 640
    Top = 56
  end
  object SaveTextFileDialog1: TSaveTextFileDialog
    FileName = 'FuzzyValues.h'
    Filter = 'Include|*.h|Text|*.txt'
    Left = 648
    Top = 88
  end
end
