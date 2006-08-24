object ClassBuilderFrm: TClassBuilderFrm
  Left = 0
  Top = 0
  Caption = 'ClassBuilderFrm'
  ClientHeight = 933
  ClientWidth = 1134
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -14
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 120
  TextHeight = 17
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 1134
    Height = 203
    ActivePage = SetBaseClass
    Align = alTop
    TabOrder = 0
    object SetBaseClass: TTabSheet
      Caption = 'Set Base Class'
      object SetBaseClassBT: TButton
        Left = 38
        Top = 18
        Width = 141
        Height = 33
        Caption = 'Set Base Class'
        TabOrder = 0
        OnClick = SetBaseClassBTClick
      end
      object IncludeStubs: TCheckBox
        Left = 52
        Top = 73
        Width = 326
        Height = 22
        Caption = 'Include Stubs for non-virtual functions'
        TabOrder = 1
      end
      inline NameED: TLabelEdit
        Left = 460
        Top = 4
        Width = 270
        Height = 55
        TabOrder = 2
        TabStop = True
        ExplicitLeft = 460
        ExplicitTop = 4
        ExplicitWidth = 270
        ExplicitHeight = 55
        inherited Label1: TLabel
          Left = 31
          Top = 21
          Width = 35
          Height = 17
          Caption = 'Name'
          ExplicitLeft = 31
          ExplicitTop = 21
          ExplicitWidth = 35
          ExplicitHeight = 17
        end
        inherited Edit1: TEdit
          Left = 94
          Top = 17
          Width = 158
          Height = 25
          Text = 'MyNewClass'
          ExplicitLeft = 94
          ExplicitTop = 17
          ExplicitWidth = 158
          ExplicitHeight = 25
        end
      end
      object ClassTypeCB: TComboBox
        Left = 230
        Top = 21
        Width = 190
        Height = 25
        Style = csDropDownList
        ItemHeight = 17
        TabOrder = 3
      end
      object Button1: TButton
        Left = 256
        Top = 136
        Width = 75
        Height = 25
        Caption = 'Button1'
        TabOrder = 4
        OnClick = Button1Click
      end
    end
    object AddVariables: TTabSheet
      Caption = 'Add Variable'
      Enabled = False
      ImageIndex = 1
      object ArrayAmountLb: TLabel
        Left = 600
        Top = 101
        Width = 50
        Height = 17
        Alignment = taCenter
        AutoSize = False
        Caption = '0'
      end
      inline VariableName: TLabelEdit
        Left = 460
        Top = 4
        Width = 270
        Height = 55
        TabOrder = 0
        TabStop = True
        ExplicitLeft = 460
        ExplicitTop = 4
        ExplicitWidth = 270
        ExplicitHeight = 55
        inherited Label1: TLabel
          Left = 4
          Top = 21
          Width = 85
          Height = 17
          Caption = 'Variable Name'
          ExplicitLeft = 4
          ExplicitTop = 21
          ExplicitWidth = 85
          ExplicitHeight = 17
        end
        inherited Edit1: TEdit
          Left = 101
          Top = 17
          Width = 158
          Height = 25
          Text = 'MyVariableName'
          OnExit = UpdateStatus
          OnKeyDown = VariableNameEdit1KeyDown
          ExplicitLeft = 101
          ExplicitTop = 17
          ExplicitWidth = 158
          ExplicitHeight = 25
        end
      end
      object RadioGroup1: TRadioGroup
        Left = 0
        Top = 4
        Width = 195
        Height = 89
        Caption = 'Variable Type'
        ItemIndex = 0
        Items.Strings = (
          'Class Instance'
          'Primitive')
        TabOrder = 1
        OnClick = UpdateStatus
      end
      object PropertyCB: TCheckBox
        Left = 774
        Top = 12
        Width = 127
        Height = 22
        Caption = 'Property'
        TabOrder = 2
        OnClick = UpdateStatus
      end
      object TypePointer: TCheckBox
        Left = 774
        Top = 42
        Width = 127
        Height = 22
        Caption = 'Type Pointer'
        TabOrder = 3
        OnClick = UpdateStatus
      end
      object PointerToArray: TCheckBox
        Left = 774
        Top = 73
        Width = 127
        Height = 22
        Caption = 'Pointer To Array'
        Enabled = False
        TabOrder = 4
        OnClick = UpdateStatus
      end
      object ClassType2CB: TComboBox
        Left = 230
        Top = 21
        Width = 190
        Height = 25
        Style = csDropDownList
        DropDownCount = 20
        ItemHeight = 17
        TabOrder = 5
        OnChange = UpdateStatus
      end
      object ArrayCount: TTrackBar
        Left = 527
        Top = 56
        Width = 196
        Height = 37
        Hint = 'Array of Variables'
        Max = 100
        Min = 1
        PageSize = 10
        Frequency = 10
        Position = 1
        TabOrder = 6
        OnChange = ArrayCountChange
      end
      object AddVariable: TButton
        Left = 7
        Top = 101
        Width = 188
        Height = 32
        Caption = 'Add Variable'
        TabOrder = 8
        OnClick = AddVariableClick
      end
      object VariableTypeCB: TComboBox
        Left = 230
        Top = 52
        Width = 190
        Height = 25
        Style = csDropDownList
        ItemHeight = 17
        TabOrder = 7
        OnChange = UpdateStatus
      end
      inline DefaultValue: TCheckedLabel
        Left = 492
        Top = 128
        Width = 388
        Height = 34
        TabOrder = 9
        TabStop = True
        ExplicitLeft = 492
        ExplicitTop = 128
        ExplicitWidth = 388
        ExplicitHeight = 34
        inherited CheckBox1: TCheckBox
          Left = 31
          Top = -3
          Width = 127
          Height = 23
          Caption = 'Default Value'
          ExplicitLeft = 31
          ExplicitTop = -3
          ExplicitWidth = 127
          ExplicitHeight = 23
        end
        inherited Edit1: TEdit
          Left = 166
          Top = -3
          Width = 158
          Height = 25
          Text = ''
          ExplicitLeft = 166
          ExplicitTop = -3
          ExplicitWidth = 158
          ExplicitHeight = 25
        end
      end
    end
    object RemoveVariable: TTabSheet
      Caption = 'Remove Variable'
      Enabled = False
      ImageIndex = 2
      object Label1: TLabel
        Left = 31
        Top = 42
        Width = 169
        Height = 17
        Caption = 'Name of variable to remove'
      end
      object RemoveVariableCB: TComboBox
        Left = 272
        Top = 38
        Width = 190
        Height = 25
        ItemHeight = 17
        TabOrder = 0
        Text = 'RemoveVariableCB'
      end
    end
  end
  object TreeView1: TTreeView
    Left = 0
    Top = 203
    Width = 1134
    Height = 711
    Align = alClient
    AutoExpand = True
    Indent = 19
    TabOrder = 1
    Items.NodeData = {
      0101000000210000000000000000000000FFFFFFFFFFFFFFFF00000000000000
      00047400650073007400}
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 914
    Width = 1134
    Height = 19
    Panels = <
      item
        Width = 50
      end>
  end
  object MainMenu1: TMainMenu
    Left = 712
    object File1: TMenuItem
      Caption = '&File'
      object New1: TMenuItem
        Caption = '&New'
        OnClick = MenuItem_NewClick
      end
      object Open1: TMenuItem
        Caption = '&Open...'
        OnClick = MenuItem_OpenClick
      end
      object Save1: TMenuItem
        Caption = '&Save'
        OnClick = MenuItem_SaveClick
      end
      object SaveAs1: TMenuItem
        Caption = 'Save &As...'
        OnClick = MenuItem_SaveAsClick
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object Print1: TMenuItem
        Caption = '&Print...'
      end
      object PrintSetup1: TMenuItem
        Caption = 'P&rint Setup...'
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Exit1: TMenuItem
        Caption = 'E&xit'
        OnClick = Exit1Click
      end
    end
    object testUseDebugtoverifyresults1: TMenuItem
      Caption = '&test (Use Debug to verify results)'
      object testcode11: TMenuItem
        Caption = 'test code 1'
      end
      object testcode21: TMenuItem
        Caption = 'test code 2'
      end
    end
  end
  object OpenDialog1: TOpenDialog
    DefaultExt = 'xml'
    Filter = 'XML Classbuilder|*.xml'
    Options = [ofHideReadOnly, ofFileMustExist, ofEnableSizing]
    Left = 616
    Top = 8
  end
  object SaveDialog1: TSaveDialog
    DefaultExt = 'XML'
    Filter = 'XML,CPP,H|*.XML'
    Left = 672
    Top = 8
  end
end
