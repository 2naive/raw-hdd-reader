object FormMain: TFormMain
  Left = 0
  Top = 0
  Caption = 'sd'
  ClientHeight = 527
  ClientWidth = 794
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
    Left = 8
    Top = 8
    Width = 107
    Height = 16
    Caption = 'Device Information'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 8
    Top = 168
    Width = 182
    Height = 16
    Caption = 'MBR Partition Table Information'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label3: TLabel
    Left = 368
    Top = 8
    Width = 65
    Height = 16
    Caption = 'File Writing'
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object Label4: TLabel
    Left = 368
    Top = 46
    Width = 46
    Height = 13
    Caption = 'Filename:'
  end
  object Label5: TLabel
    Left = 368
    Top = 73
    Width = 75
    Height = 13
    Caption = 'Starting sector:'
  end
  object Label6: TLabel
    Left = 368
    Top = 100
    Width = 57
    Height = 13
    Caption = 'Last sector:'
  end
  object Label7: TLabel
    Left = 368
    Top = 168
    Width = 70
    Height = 16
    Caption = 'File Reading'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label8: TLabel
    Left = 368
    Top = 200
    Width = 79
    Height = 13
    Caption = 'Target filename:'
  end
  object Label9: TLabel
    Left = 368
    Top = 227
    Width = 75
    Height = 13
    Caption = 'Starting sector:'
  end
  object Label10: TLabel
    Left = 368
    Top = 254
    Width = 57
    Height = 13
    Caption = 'Last sector:'
  end
  object Label11: TLabel
    Left = 365
    Top = 320
    Width = 87
    Height = 16
    Alignment = taRightJustify
    Caption = 'Sector Reading'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label12: TLabel
    Left = 512
    Top = 471
    Width = 35
    Height = 13
    Caption = 'Sector:'
  end
  object Bevel1: TBevel
    Left = 361
    Top = 30
    Width = 425
    Height = 126
  end
  object Bevel2: TBevel
    Left = 361
    Top = 188
    Width = 425
    Height = 126
  end
  object Bevel3: TBevel
    Left = 361
    Top = 342
    Width = 425
    Height = 160
  end
  object Label13: TLabel
    Left = 8
    Top = 320
    Width = 181
    Height = 16
    Caption = 'Assumable Unpartitioned Space'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object DeviceInformationTable: TStringGrid
    Left = 8
    Top = 27
    Width = 225
    Height = 129
    ColCount = 2
    DefaultColWidth = 110
    DefaultRowHeight = 20
    FixedCols = 0
    RowCount = 6
    FixedRows = 0
    TabOrder = 0
  end
  object PTTable: TStringGrid
    Left = 8
    Top = 187
    Width = 347
    Height = 127
    ParentCustomHint = False
    ColCount = 4
    DefaultColWidth = 100
    DefaultRowHeight = 20
    FixedCols = 0
    RowCount = 2
    TabOrder = 1
    ColWidths = (
      40
      100
      100
      100)
  end
  object ButtonOpenFile: TButton
    Left = 707
    Top = 41
    Width = 66
    Height = 25
    Caption = 'Open'
    TabOrder = 2
    OnClick = ButtonOpenFileClick
  end
  object EditInputFilename: TEdit
    Left = 448
    Top = 43
    Width = 253
    Height = 21
    Color = cl3DLight
    Enabled = False
    TabOrder = 3
  end
  object EditWriteSector: TEdit
    Left = 448
    Top = 70
    Width = 253
    Height = 21
    TabOrder = 4
    Text = '800000000'
  end
  object MemoSectorData: TMemo
    Left = 368
    Top = 352
    Width = 405
    Height = 108
    TabOrder = 5
  end
  object ButtonReadSector: TButton
    Left = 655
    Top = 466
    Width = 118
    Height = 25
    Caption = 'Read sector'
    TabOrder = 6
    OnClick = ButtonReadSectorClick
  end
  object ButtonWriteFile: TButton
    Left = 655
    Top = 124
    Width = 118
    Height = 25
    Caption = 'Write file'
    TabOrder = 7
    OnClick = ButtonWriteFileClick
  end
  object EditWriteResult: TEdit
    Left = 448
    Top = 97
    Width = 253
    Height = 21
    Color = cl3DLight
    Enabled = False
    TabOrder = 8
  end
  object EditReadSectorId: TEdit
    Left = 552
    Top = 468
    Width = 97
    Height = 21
    TabOrder = 9
    Text = '800000000'
  end
  object ButtonChooseFile: TButton
    Left = 707
    Top = 195
    Width = 66
    Height = 25
    Caption = 'Choose'
    TabOrder = 10
    OnClick = ButtonChooseFileClick
  end
  object EditOutputFilename: TEdit
    Left = 448
    Top = 197
    Width = 253
    Height = 21
    Color = cl3DLight
    Enabled = False
    TabOrder = 11
  end
  object EditGetSector: TEdit
    Left = 448
    Top = 224
    Width = 253
    Height = 21
    TabOrder = 12
    Text = '800000000'
  end
  object ButtonGetFile: TButton
    Left = 655
    Top = 278
    Width = 118
    Height = 25
    Caption = 'Get file'
    TabOrder = 13
    OnClick = ButtonGetFileClick
  end
  object EditGetLastSector: TEdit
    Left = 448
    Top = 251
    Width = 254
    Height = 21
    TabOrder = 14
    Text = '800000000'
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 508
    Width = 794
    Height = 19
    Panels = <>
    SimplePanel = True
    SimpleText = 'Ready'
  end
  object UnpartitionedSpaceTable: TStringGrid
    Left = 8
    Top = 342
    Width = 345
    Height = 160
    ParentCustomHint = False
    ColCount = 4
    DefaultColWidth = 100
    DefaultRowHeight = 20
    FixedCols = 0
    RowCount = 6
    TabOrder = 16
    ColWidths = (
      38
      100
      100
      100)
  end
  object OpenDialog1: TOpenDialog
    Title = 'Choose file to open'
    Left = 384
    Top = 115
  end
  object SaveDialog1: TSaveDialog
    DefaultExt = '*.*'
    Left = 384
    Top = 267
  end
end
