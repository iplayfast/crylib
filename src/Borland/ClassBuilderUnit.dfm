object ClassBuilderFrm: TClassBuilderFrm
  Left = 0
  Top = 0
  Caption = 'ClassBuilderFrm'
  ClientHeight = 713
  ClientWidth = 867
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 867
    Height = 155
    ActivePage = SetBaseClass
    Align = alTop
    TabOrder = 0
    object SetBaseClass: TTabSheet
      Caption = 'Set Base Class'
      object SetBaseClassBT: TButton
        Left = 29
        Top = 14
        Width = 108
        Height = 25
        Caption = 'Set Base Class'
        TabOrder = 0
        OnClick = SetBaseClassBTClick
      end
      object IncludeStubs: TCheckBox
        Left = 40
        Top = 56
        Width = 249
        Height = 17
        Caption = 'Include Stubs for non-virtual functions'
        TabOrder = 1
      end
      inline NameED: TLabelEdit
        Left = 352
        Top = 3
        Width = 206
        Height = 42
        TabOrder = 2
        TabStop = True
        ExplicitLeft = 352
        ExplicitTop = 3
        inherited Label1: TLabel
          Width = 27
          Caption = 'Name'
          ExplicitWidth = 27
        end
        inherited Edit1: TEdit
          Text = 'MyNewClass'
        end
      end
      object ClassTypeCB: TComboBox
        Left = 176
        Top = 16
        Width = 145
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 3
      end
      object Button1: TButton
        Left = 196
        Top = 104
        Width = 57
        Height = 19
        Caption = 'Button1'
        TabOrder = 4
        OnClick = Button1Click
      end
    end
    object AddVariables: TTabSheet
      Caption = 'Add Variable'
      Enabled = False
      ImageIndex = 1
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object ArrayAmountLb: TLabel
        Left = 459
        Top = 77
        Width = 38
        Height = 13
        Alignment = taCenter
        AutoSize = False
        Caption = '0'
      end
      inline VariableName: TLabelEdit
        Left = 352
        Top = 3
        Width = 206
        Height = 42
        TabOrder = 0
        TabStop = True
        ExplicitLeft = 352
        ExplicitTop = 3
        inherited Label1: TLabel
          Left = 3
          Width = 68
          Caption = 'Variable Name'
          ExplicitLeft = 3
          ExplicitWidth = 68
        end
        inherited Edit1: TEdit
          Left = 77
          Height = 25
          Text = 'MyVariableName'
          OnExit = UpdateStatus
          OnKeyDown = VariableNameEdit1KeyDown
          ExplicitLeft = 77
          ExplicitHeight = 25
        end
      end
      object RadioGroup1: TRadioGroup
        Left = 0
        Top = 3
        Width = 149
        Height = 68
        Caption = 'Variable Type'
        ItemIndex = 0
        Items.Strings = (
          'Class Instance'
          'Primitive')
        TabOrder = 1
        OnClick = UpdateStatus
      end
      object PropertyCB: TCheckBox
        Left = 592
        Top = 9
        Width = 97
        Height = 17
        Caption = 'Property'
        TabOrder = 2
        OnClick = UpdateStatus
      end
      object TypePointer: TCheckBox
        Left = 592
        Top = 32
        Width = 97
        Height = 17
        Caption = 'Type Pointer'
        TabOrder = 3
        OnClick = UpdateStatus
      end
      object PointerToArray: TCheckBox
        Left = 592
        Top = 56
        Width = 97
        Height = 17
        Caption = 'Pointer To Array'
        Enabled = False
        TabOrder = 4
        OnClick = UpdateStatus
      end
      object ClassType2CB: TComboBox
        Left = 176
        Top = 16
        Width = 145
        Height = 25
        Style = csDropDownList
        DropDownCount = 20
        ItemHeight = 0
        TabOrder = 5
        OnChange = UpdateStatus
      end
      object ArrayCount: TTrackBar
        Left = 403
        Top = 43
        Width = 150
        Height = 28
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
        Left = 5
        Top = 77
        Width = 144
        Height = 25
        Caption = 'Add Variable'
        TabOrder = 8
        OnClick = AddVariableClick
      end
      object VariableTypeCB: TComboBox
        Left = 176
        Top = 40
        Width = 145
        Height = 25
        Style = csDropDownList
        ItemHeight = 0
        TabOrder = 7
        OnChange = UpdateStatus
      end
      inline DefaultValue: TCheckedLabel
        Left = 376
        Top = 98
        Width = 297
        Height = 26
        TabOrder = 9
        TabStop = True
        ExplicitLeft = 376
        ExplicitTop = 98
        ExplicitWidth = 297
        ExplicitHeight = 26
        inherited CheckBox1: TCheckBox
          Left = 24
          Top = -2
          Caption = 'Default Value'
          ExplicitLeft = 24
          ExplicitTop = -2
        end
        inherited Edit1: TEdit
          Left = 127
          Top = -2
          Height = 25
          Text = ''
          ExplicitLeft = 127
          ExplicitTop = -2
          ExplicitHeight = 25
        end
      end
    end
    object RemoveVariable: TTabSheet
      Caption = 'Remove Variable'
      Enabled = False
      ImageIndex = 2
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label1: TLabel
        Left = 24
        Top = 32
        Width = 133
        Height = 13
        Caption = 'Name of variable to remove'
      end
      object RemoveVariableCB: TComboBox
        Left = 208
        Top = 29
        Width = 145
        Height = 25
        ItemHeight = 0
        TabOrder = 0
        Text = 'RemoveVariableCB'
      end
    end
  end
  object TreeView1: TTreeView
    Left = 0
    Top = 155
    Width = 867
    Height = 539
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
    Top = 694
    Width = 867
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
