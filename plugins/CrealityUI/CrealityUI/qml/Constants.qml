pragma Singleton
import QtQuick 2.10

QtObject {
    property var screenScaleFactor: 1
    property  bool mainPreviewShow: false
    property  int rightPanelWidth: 280
    property  int rightPanelHeight: 670
    property int rightPanelMaxWidth: 280
    //lisugui 2021-1-21
    property bool bLeftToolBarEnabled: true
    property bool bRightPanelEnabled: true
    property bool bMenuBarEnabled: true
    property bool bGLViewEnabled: true
    //
    property bool bModelAlwaysShow: true

    property bool bIsWizardShowing: false

    property bool bIsLaserSizeLoced: true

    property var languageType: 0

    property int leftShowType: 0  //0 :leftbar,1: models,2 : list  //控制左边栏的 互斥显示

    readonly property FontLoader mySystemFont: FontLoader {name:"Source Han Sans CN Normal"}

    property alias fontDirectory: directoryFontLoader.fontDirectory
    property alias relativeFontDirectory: directoryFontLoader.relativeFontDirectory

    /* Edit this comment to add your custom font */
    readonly property font font: Qt.font({
                                             family: mySystemFont.name,
                                             pointSize: Qt.application.font.pointSize
                                         })
    readonly property font largeFont: Qt.font({
                                                  family: mySystemFont.name,
                                                  pointSize: Qt.application.font.pointSize * 1.6
                                              })

    readonly property string labelFontFamily: "Source Han Sans CN Normal"
    readonly property string panelFontFamily: "Microsoft YaHei UI"
    readonly property int labelFontWeight: Font.Normal
    readonly property int labelFontPixelSize: 12
    readonly property int labelFontPointSize: 9
    readonly property int labelLargeFontPixelSize: 14
    readonly property int panelLargeFontPixelSize: 16
    readonly property int labelLargeFontPointSize: 12
    readonly property int imageButtomPointSize: 9
    //    property color backgroundColor: "#000000"
    //切片 功能模块界面 用到的字体颜色
    readonly property color disabledTextColor: "#a0a1a2"

    //backgroundColor
    property color itemBackgroundColor: "#061F3B"//"#29292c"
    property color textRectBgHoveredColor: "#1E9BE2"  //输入框的hovercolor


    //常规字体颜色
    property color textColor: "#E3EBEE"
    property color textBackgroundColor: "#4B4B4B"
    property color unitColor : "#BCBBBB"
    //按钮的默认颜色
    property color itemChildBackgroundColor:"#383C3E"  //框内框的背景颜色
    property color buttonColor:"#535353" //"#061F3B" //"#5C6163"
    property color hoveredColor: "#3A3A3A"//"#1b9ebb"
    property color selectionColor:  "#214076" // "#42bdd8"
    property color rectBorderColor: "#909090" //"#333333"
    property color dialogBtnHoveredColor: "#5D5D5D"
    property color secondBtnHoverdColor: "#666666"
    property color secondBtnSelectionColor: "#36B785"
    //slider color
    property color grooveColor: "#5E5E64"
    property color handleColor: "#42BDD8"
    property color handleBorderColor: "#5E5E64"

    //MAIN background
    property color mainBackgroundColor : "#363638"

    property color dropShadowColor: "#333333"
    //cmb
    property color cmbPopupColor: "#E1E1E1"
    property color cmbPopupColor_pressed : "#9CE8F4"//"#3AC2D7"

    property color cmbIndicatorRectColor : "#B0B0B0"
    property color cmbIndicatorRectColor_pressed : "#D7D7D7"

    property color cmbIndicatorRectColor_pressed_basic : "transparent"
    property color cmbIndicatorRectColor_basic: "transparent"
    //tree
    property color treeBackgroundColor: "#424242"
    property color treeItemColor:"#424242"
    property color treeItemColor_pressed: "#666666"
    //lefttoolbar
    property color leftBtnBgColor_normal:  "#4b4b4d"
    property color leftBtnBgColor_hovered: "#68686b"
    property color leftBtnBgColor_selected: "#1e9be2"
    property color leftTextColor: "#C3C3C3"
    property color leftTextColor_d: "white"
    //topbar
    property color topBtnBgColor_normal:  "#2f2f30"
    property color topBtnBgColor_hovered: "#1e9be2"
    property color topBtnBorderColor_normal: "transparent"
    property color topBtnBordeColor_hovered: "transparent"
    property color topBtnTextColor_normal : "#8D8D91"
    property color topBtnTextColor_hovered : "#FFFFFF"
    //dialog
    property color dialogTitleColor : "#373737"
    property color dialogTitleTextColor : "white"
    property color dialogItemRectBgColor : "#4B4B4B"
    property color dialogItemRectBgBorderColor : "#6E6E72"
    property color aboutDiaItemRectBgColor : "#535353"
    property color dialogContentBgColor : "#535353"
    property color dialogBorderColor : "#262626"
    //tabBtn
    property color tabButtonSelectColor: "#1e9be2"
    property color tabButtonNormalColor: "#535353"

    //menuBar title
    property color headerBackgroundColor: "#333333"
    property color menuBarBtnColor: "#333333"
    property color menuBarBtnColor_hovered: "#3E3E3E"
    property color menuBarBtnColor_border: "#4D4D4D"
    property color menuStyleBgColor: "white"
    property color menuStyleBgColor_hovered: "#9CE8F4"
    property color menuTextColor: "white"
    property color menuTextColor_hovered: "white"
    property color menuTextColor_normal: "white"

    //search btn
    property color searchBtnDisableColor : "#ECECEC"
    property color searchBtnNormalColor : "#1E9BE2"
    property color searchBtnHoveredColor : "#1EB6E2"

    property color typeBtnSelectedColor : "#1E9BE2"
    property color typeBtnHoveredColor : "#666666"
    property color typeModelBtnListHoveredColor: "#666666"
    property color splitLineColor : "#666666"
    property color splitLineColorDark : "#444444"
    property color radioCheckLabelColor: "white"
    property color radioCheckRadiusColor: "white"
    property color radioInfillBgColor: "#424242"
    property color radioBorderColor: "#333333"
    property color profileBtnColor: "#6e6e73"
    property color profileBtnHoverColor: "#8a8a8a"//"#C2C2C5"
    property color tooltipBgColor: "#454545"
    property color checkBoxBgColor: "#424242"
    property color checkBoxBorderColor: "#6e6e72"
    property color menuSplitLineColor: "#0D0D0D"
    property color menuSplitLineColorleft: "#303030"
    property color mainViewSplitLineColor: "#0D0D0D"
    property color printerCombRectColor: "#D7D7D7"
    property color sliderTopColor1: "#4A4A4A"
    property color sliderTopColor2: "#767676"
    property color sliderBottomColor: "#535353"
    property color progressBarBgColor: "#303030"
    property color previewPanelRecgColor: "black"
    property color previewPanelTextgColor: "#CDD2D7"

    property color switchModelBgColor: "#181818"
    property color switchModeSelectedlBgColor: "#535353"
    property color modleItemBorderColor: "#D7D7D7"
    property color userInfoPrintItem: "#D7D7D7"
    property color userinfoBasicItemRecColor : "#404040"
    property color modelAlwaysMoreBtnBgColor: "#D7D7D7"
    property color modelAlwaysItemBorderColor: "#262626"

    property color cube3DAmbientColor: "#828282"
    property color cube3DAmbientColor_Entered:"#E5E0D0"

    property color textColor_disabled:"#AFAFAF"
    property color printerCommboxBtnDefaultColor: "#B7B7B7"
    property color printerCommboxPopBgColor: "#E1E1E1"
    property color printerCommboxIndicatorBgColor: "#666666"
    property color printerCommboxIndicatorPopShowBgColor: "#E1E1E1"
    property color printerCommboxBgColor: "#181818"
    property color printerCommboxBgBorderColor: "#333333"

    property color tittleBarBtnColor: "#3E3E3E"
    property color laserFoldTittleColor: "#414143"
    

    property color enabledBtnShadowColor:"black"

    property color laserLineBorderColor:"#999999"

    //spinbox
    property string upBtnImgSource: "qrc:/UI/photo/upBtn.png"
    property string upBtnImgSource_d: "qrc:/UI/photo/upBtn_d.png"

    property string downBtnImgSource: "qrc:/UI/photo/downBtn.png"
    property string downBtnImgSource_d: "qrc:/UI/photo/downBtn_d.png"


    property var clearBtnImg: "qrc:/UI/photo/clearBtn.png"
    property var clearBtnImg_d: "qrc:/UI/photo/clearBtn_d.png"

    property var sourchBtnImg: "qrc:/UI/photo/search.png"
    property var sourchBtnImg_d: "qrc:/UI/photo/search_d.png"

    property var downBtnImg: "qrc:/UI/photo/downBtn.png"
    property var checkBtnImg: "qrc:/UI/images/check2.png"

    property var showPWHoveredImg: "qrc:/UI/photo/showPW_d.png"
    property var hidePWHoveredImg: "qrc:/UI/photo/hidePW_d.png"
    property var areaCodeComboboxImg: "qrc:/UI/photo/comboboxDown.png"

    property var sliderBtnimg: "qrc:/UI/photo/rangSlider.png"

    property var configTabBtnImg: "qrc:/UI/photo/configTabBtn.png"
    property var configTabBtnImg_Hovered: "qrc:/UI/photo/configTabBtn_d.png"

    property var supportTabBtnImg:"qrc:/UI/photo/supportTabBtn.png"
    property var supportTabBtnImg_Hovered:"qrc:/UI/photo/supportTabBtn_d.png"

    property var homeImg: "qrc:/UI/images/home.png"
    property var homeImg_Hovered: "qrc:/UI/images/home_s.png"

    property var laserPickImg: "qrc:/UI/images/laser_pick.png"
    property var laserImageImg: "qrc:/UI/images/laser_img.png"
    property var laserFontImg: "qrc:/UI/images/laser_font.png"
    property var laserRectImg: "qrc:/UI/images/laser_rect.png"
    property var laserArcImg: "qrc:/UI/images/laser_arc.png"

    property var switchModelFDMImg: "qrc:/UI/images/fdmMode.png"
    property var switchModelFDMImg_H: "qrc:/UI/images/fdmMode_h.png"
    property var switchModelLaserImg: "qrc:/UI/images/laserMode.png"
    property var switchModelLaserImg_H: "qrc:/UI/images/laserMode_h.png"
    property var switchModelLaserImgEn: "qrc:/UI/images/laserMode_en.png"
    property var switchModelLaserImgEn_H: "qrc:/UI/images/laserMode_h_en.png"
    property var switchModelCNCImg: "qrc:/UI/images/CNCMode.png"
    property var switchModelCNCImg_H: "qrc:/UI/images/CNCMode_h.png"

    property var printFanOffImg : "qrc:/UI/photo/print_fan_off.png"
    property var printFanOnImg: "qrc:/UI/photo/print_fan_on.png"
    property var printMoveAxisYUpImg: "qrc:/UI/photo/print_move_axis_y+.png"
    property var printMoveAxisYUp_HImg: "qrc:/UI/photo/print_move_axis_y+_h.png"
    property var printMoveAxisYUp_CImg: "qrc:/UI/photo/print_move_axis_y+_c.png"
    property var printMoveAxisZUpImg: "qrc:/UI/photo/print_move_axis_z+.png"
    property var printMoveAxisZUp_HImg: "qrc:/UI/photo/print_move_axis_z+_h.png"
    property var printMoveAxisZUp_CImg: "qrc:/UI/photo/print_move_axis_z+_c.png"
    property var printMoveAxisXDownImg: "qrc:/UI/photo/print_move_axis_x-.png"
    property var printMoveAxisXDown_HImg: "qrc:/UI/photo/print_move_axis_x-_h.png"
    property var printMoveAxisXDown_CImg: "qrc:/UI/photo/print_move_axis_x-_c.png"
    property var printMoveAxisZeroImg: "qrc:/UI/photo/print_move_axis_zero.png"
    property var printMoveAxisZero_HImg: "qrc:/UI/photo/print_move_axis_zero_h.png"
    property var printMoveAxisZero_CImg: "qrc:/UI/photo/print_move_axis_zero_c.png"
    property var printMoveAxisXUpImg: "qrc:/UI/photo/print_move_axis_x+.png"
    property var printMoveAxisXUp_HImg: "qrc:/UI/photo/print_move_axis_x+_h.png"
    property var printMoveAxisXUp_CImg: "qrc:/UI/photo/print_move_axis_x+_c.png"
    property var printMoveAxisYDownImg: "qrc:/UI/photo/print_move_axis_y-.png"
    property var printMoveAxisYDown_HImg: "qrc:/UI/photo/print_move_axis_y-_h.png"
    property var printMoveAxisYDown_CImg: "qrc:/UI/photo/print_move_axis_y-_c.png"
    property var printMoveAxisZDownImg: "qrc:/UI/photo/print_move_axis_z-.png"
    property var printMoveAxisZDown_HImg: "qrc:/UI/photo/print_move_axis_z-_h.png"
    property var printMoveAxisZDown_CImg: "qrc:/UI/photo/print_move_axis_z-_c.png"

    property var userinfoDel_HImg: "qrc:/UI/photo/userinfo_delete_h.png"
    property var userinfoPrint_HImg: "qrc:/UI/photo/userinfo_print_h.png"
    property var userinfoShare_HImg: "qrc:/UI/photo/userinfo_share_h.png"
    property var userinfoExport_HImg: "qrc:/UI/photo/userinfo_export_h.png"

    property var modelAlwaysPopBg: "qrc:/UI/photo/model_always_pop_bg.png"
    property var modelAlwaysBtnIcon: "qrc:/UI/photo/model_always_show.png"
    property var uploadModelImg: "qrc:/UI/photo/upload_model_img.png"
    property var uploadLocalModelImg: "qrc:/UI/photo/upload_model_local_img.png"
    property var deleteModelImg: "qrc:/UI/photo/delete_model_img.png"

    property var editProfileImg: "qrc:/UI/photo/editProfile.png"
    property var uploadProfileImg: "qrc:/UI/photo/uploadPro.png"
    property var deleteProfileImg: "qrc:/UI/photo/deleteProfile.png"

    property var onSrcImg: "qrc:/UI/photo/on.png"
    property var offSrcImg: "qrc:/UI/photo/off.png"
    property var checkedSrcImg: "qrc:/UI/photo/radio_1.png"
    property var uncheckSrcImg: "qrc:/UI/photo/radio_2.png"

    property var cube3DTopImg : "qrc:/UI/images/top.png"
    property var cube3DTopImg_C: "qrc:/UI/images/top_C.png"
    property var cube3DFrontImg: "qrc:/UI/images/front.png"
    property var cube3DFrontImg_C: "qrc:/UI/images/front_C.png"
    property var cube3DBottomImg: "qrc:/UI/images/bottom.png"
    property var cube3DBottomImg_C: "qrc:/UI/images/bottom_C.png"
    property var cube3DBackImg: "qrc:/UI/images/back.png"
    property var cube3DBackImg_C: "qrc:/UI/images/back_C.png"
    property var cube3DLeftkImg: "qrc:/UI/images/left.png"
    property var cube3DLeftkImg_C: "qrc:/UI/images/left_C.png"
    property var cube3DLeftkRight: "qrc:/UI/images/right.png"
    property var cube3DLeftkRight_C: "qrc:/UI/images/right_C.png"

    property var laserFoldTitleUpImg: "qrc:/UI/photo/laser_fold_item_up.png"
    property var laserFoldTitleDownImg: "qrc:/UI/photo/laser_fold_item_down.png"

    property var wifiRefreshImg: "qrc:/UI/photo/refresh.png"

    // ---------- right panel [beg] ----------

    property color right_panel_text_default_color         : "#FFFFFF"
    property color right_panel_text_disable_color         : "#FFFFFF"
    property color right_panel_text_hovered_color         : "#FFFFFF"
    property color right_panel_text_checked_color         : "#FFFFFF"

    property color right_panel_button_default_color       : "#4B4B4D"
    property color right_panel_button_disable_color       : "#4B4B4D"
    property color right_panel_button_hovered_color       : "#414143"
    property color right_panel_button_checked_color       : "#414143"

    property color right_panel_border_default_color       : "#6C6C70"
    property color right_panel_border_disable_color       : "#6C6C70"
    property color right_panel_border_hovered_color       : "#1E9BE2"
    property color right_panel_border_checked_color       : "#1E9BE2"

    property color right_panel_item_default_color         : "#414143"
    property color right_panel_item_disable_color         : "#414143"
    property color right_panel_item_hovered_color         : "#5F5F5F"
    property color right_panel_item_checked_color         : "#739AB0"
    property color right_panel_item_text_default_color    : "#CBCBCB"
    property color right_panel_item_text_disable_color    : "#CBCBCB"
    property color right_panel_item_text_hovered_color    : "#CBCBCB"
    property color right_panel_item_text_checked_color    : "#FFFFFF"

    property color right_panel_combobox_background_color  : "#414143"

    property color right_panel_tab_text_default_color     : "#8D8D91"
    property color right_panel_tab_text_disable_color     : "#8D8D91"
    property color right_panel_tab_text_hovered_color     : "#8D8D91"
    property color right_panel_tab_text_checked_color     : "#FFFFFF"
    property color right_panel_tab_button_default_color   : "#1C1C1D"
    property color right_panel_tab_button_disable_color   : "#1C1C1D"
    property color right_panel_tab_button_hovered_color   : "#1C1C1D"
    property color right_panel_tab_button_checked_color   : "#1E9BE2"
    property color right_panel_tab_background_color       : "#1C1C1D"

    property color right_panel_menu_border_color          : "#1C1C1D"
    property color right_panel_menu_background_color      : "#4B4B4D"
    property color right_panel_menu_split_line_color      : "#3B3B3D"

    property color right_panel_slice_button_default_color : "#1E9BE2"
    property color right_panel_slice_button_disable_color : "#626265"
    property color right_panel_slice_button_hovered_color : "#1E9BE2"
    property color right_panel_slice_button_checked_color : "#1E9BE2"
    property color right_panel_slice_text_default_color   : "#B1B1B7"
    property color right_panel_slice_text_disable_color   : "#B1B1B7"
    property color right_panel_slice_text_hovered_color   : "#B1B1B7"
    property color right_panel_slice_text_checked_color   : "#FFFFFF"

    property string right_panel_quality_custom_default_image  : "qrc:/UI/photo/config_quality_custom.png"
    property string right_panel_quality_custom_checked_image  : "qrc:/UI/photo/config_quality_custom_checked.png"
    property string right_panel_quality_high_default_image    : "qrc:/UI/photo/config_quality_high.png"
    property string right_panel_quality_high_checked_image    : "qrc:/UI/photo/config_quality_high_checked.png"
    property string right_panel_quality_middle_default_image  : "qrc:/UI/photo/config_quality_middle.png"
    property string right_panel_quality_middle_checked_image  : "qrc:/UI/photo/config_quality_middle_checked.png"
    property string right_panel_quality_low_default_image     : "qrc:/UI/photo/config_quality_low.png"
    property string right_panel_quality_low_checked_image     : "qrc:/UI/photo/config_quality_low_checked.png"
    property string right_panel_quality_verylow_default_image : "qrc:/UI/photo/config_quality_verylow.png"
    property string right_panel_quality_verylow_checked_image : "qrc:/UI/photo/config_quality_verylow_checked.png"

    // ---------- right panel [end] ----------

    //lanPrinter Panel
    property color lanPrinter_panel_border: "transparent"
    property color lanPrinter_panel_crossline: "#0F0F10"
    property color lanPrinter_panel_light_txt: "#FFFFFF"
    property color lanPrinter_panel_weight_txt: "#FFFFFF"
    property color lanPrinter_panel_background: "#242426"
    property color lanPrinter_panel_btn_default: "#2E2E30"
    property color lanPrinter_panel_btn_hovered: "#414143"

    //leftPopWin
    property color darkThemeColor_primary: "#4b4b4d"
    property color darkThemeColor_secondary: "#6e6e73"
    property color darkThemeColor_third: "#6e6e73"

    property color lightThemeColor_primary: "#ffffff"
    property color lightThemeColor_secondary: "#dddde1"
    property color lightThemeColor_third: "#999999"

    property color themeTextColor: "#ffffff"
    property color themeColor_primary: darkThemeColor_primary
    property color themeColor_secondary: darkThemeColor_secondary
    property color themeColor_third: darkThemeColor_third

    property color lpw_bgColor: themeColor_primary
    property color lpw_titleColor: themeColor_third
    property color lpw_textColor: themeTextColor

    property color lpw_spinColor: themeColor_secondary
    property color lpw_spinHoverColor: themeColor_primary
    property color lpw_spinClickedColor: themeColor_primary

    property color lpw_spinIndectorBgColor: themeColor_secondary
    property color lpw_spinIndectorBgHoverColor: themeColor_primary

    property color lpw_spinBorderColor: themeColor_primary
    property color lpw_spinBorderHoverColor: themeColor_primary

    property color lpw_BtnColor: themeColor_primary
    property color lpw_BtnHoverColor: themeColor_secondary

    property color lpw_BtnBorderColor: themeColor_secondary
    property color lpw_BtnBorderHoverColor: themeColor_secondary

    property string addModel: "qrc:/UI/photo/addModel.png"
    property string delModel: "qrc:/UI/photo/deleteModel.png"
    //_leftPopWin

    property DirectoryFontLoader directoryFontLoader: DirectoryFontLoader {
        id: directoryFontLoader
    }


    property string imagePathPrefix: "file:///./photo/"

    Component.onCompleted: {
        console.log("imagePathPrefix =" + imagePathPrefix)
        console.log("onCompleted-currentTheme:",currentTheme)
        currentTheme = 0
    }
    //new color
    property color themeColor
    property color controlColor
    property color tipBackgroundColor
    property color tipBorderColor
    property color infoPanelColor
    property color infoPanelBgColor

    property color controlBorderColor
    property int currentTheme: -1
    onCurrentThemeChanged: {
        console.log("onCurrentThemeChanged-currentTheme:",currentTheme)
        var t = themes.get(currentTheme)
        mainBackgroundColor = t.mainBackgroundColor
        dropShadowColor = t.dropShadowColor
        headerBackgroundColor = t.headerBackgroundColor;
        menuBarBtnColor = t.menuBarBtnColor;
        menuBarBtnColor_hovered = t.menuBarBtnColor_hovered
        menuTextColor = t.menuTextColor;
        menuTextColor_hovered = t.menuTextColor_hovered
        menuTextColor_normal = t.menuTextColor_normal
        menuStyleBgColor = t.menuStyleBgColor;
        menuStyleBgColor_hovered = t.menuStyleBgColor_hovered;
        infoPanelColor = t.infoPanelColor;
        aboutDiaItemRectBgColor = t.aboutDiaItemRectBgColor;
        
        //LeftToolBar
        leftBtnBgColor_normal       = t.leftBtnBgColor_normal
        leftBtnBgColor_hovered      = t.leftBtnBgColor_hovered
        leftBtnBgColor_selected     = t.leftBtnBgColor_selected
        leftTextColor               = t.leftTextColor
        leftTextColor_d             = t.leftTextColor_d
        topBtnBgColor_normal        = t.topBtnBgColor_normal
        topBtnBgColor_hovered       = t.topBtnBgColor_hovered
        topBtnBorderColor_normal    = t.topBtnBorderColor_normal
        topBtnBordeColor_hovered    = t.topBtnBordeColor_hovered
        topBtnTextColor_normal      = t.topBtnTextColor_normal
        topBtnTextColor_hovered     = t.topBtnTextColor_hovered

        themeColor = t.themeColor
        tipBackgroundColor = t.tipBackgroundColor
        tipBorderColor = t.tipBorderColor
        controlColor = t.controlColor
        controlBorderColor = t.controlBorderColor
        itemBackgroundColor = t.itemBackgroundColor

        buttonColor = t.buttonColor
        dialogBtnHoveredColor = t.dialogBtnHoveredColor
        secondBtnHoverdColor = t.secondBtnHoverdColor
        secondBtnSelectionColor = t.secondBtnSelectionColor
        hoveredColor = t.hoveredColor
        selectionColor = t.selectionColor
        textBackgroundColor = t.textBackgroundColor
        rectBorderColor = t.rectBorderColor
        dialogTitleColor = t.dialogTitleColor
        dialogContentBgColor = t.dialogContentBgColor
        dialogTitleTextColor = t.dialogTitleTextColor
        dialogBorderColor = t.dialogBorderColor

        searchBtnDisableColor = t.searchBtnDisableColor
        searchBtnNormalColor = t.searchBtnNormalColor
        searchBtnHoveredColor = t.searchBtnHoveredColor
        typeBtnHoveredColor = t.typeBtnHoveredColor
        typeModelBtnListHoveredColor = t.typeModelBtnListHoveredColor

        textColor = t.textColor
        dialogItemRectBgColor = t.dialogItemRectBgColor
        dialogItemRectBgBorderColor = t.dialogItemRectBgBorderColor

        splitLineColor = t.splitLineColor
        splitLineColorDark = t.splitLineColorDark
        radioCheckLabelColor = t.radioCheckLabelColor
        radioCheckRadiusColor = t.radioCheckRadiusColor
        radioInfillBgColor = t.radioInfillBgColor
        radioBorderColor = t.radioBorderColor
        profileBtnColor = t.profileBtnColor
        profileBtnHoverColor = t.profileBtnHoverColor
        tooltipBgColor = t.tooltipBgColor
        checkBoxBgColor = t.checkBoxBgColor
        checkBoxBorderColor = t.checkBoxBorderColor
        cmbIndicatorRectColor_basic = t.cmbIndicatorRectColor_basic
        cmbIndicatorRectColor_pressed_basic = t.cmbIndicatorRectColor_pressed_basic
        menuSplitLineColor = t.menuSplitLineColor
        menuSplitLineColorleft = t.menuSplitLineColorleft
        mainViewSplitLineColor = t.mainViewSplitLineColor
        printerCombRectColor = t.printerCombRectColor
        sliderTopColor1 = t.sliderTopColor1
        sliderTopColor2 = t.sliderTopColor2
        sliderBottomColor = t.sliderBottomColor
        sliderBtnimg = t.sliderBtnimg

        previewPanelRecgColor = t.previewPanelRecgColor
        tittleBarBtnColor = t.tittleBarBtnColor
        laserFoldTittleColor = t.laserFoldTittleColor

        treeBackgroundColor = t.treeBackgroundColor
        treeItemColor = t.treeItemColor
        treeItemColor_pressed = t.treeItemColor_pressed
        tabButtonSelectColor = t.tabButtonSelectColor
        tabButtonNormalColor = t.tabButtonNormalColor
        switchModelBgColor = t.switchModelBgColor
        switchModeSelectedlBgColor = t.switchModeSelectedlBgColor
        userInfoPrintItem = t.userInfoPrintItem
        userinfoBasicItemRecColor = t.userinfoBasicItemRecColor
        clearBtnImg = t.clearBtnImg
        clearBtnImg_d = t.clearBtnImg_d
        sourchBtnImg = t.sourchBtnImg
        sourchBtnImg_d = t.sourchBtnImg_d
        downBtnImg = t.downBtnImg
        checkBtnImg = t.checkBtnImg
        showPWHoveredImg = t.showPWHoveredImg
        hidePWHoveredImg = t.hidePWHoveredImg
        areaCodeComboboxImg = t.areaCodeComboboxImg
        configTabBtnImg = t.configTabBtnImg
        supportTabBtnImg = t.supportTabBtnImg
        configTabBtnImg_Hovered = t.configTabBtnImg_Hovered
        supportTabBtnImg_Hovered = t.supportTabBtnImg_Hovered
        progressBarBgColor = t.progressBarBgColor
        previewPanelTextgColor = t.previewPanelTextgColor
        modelAlwaysMoreBtnBgColor = t.modelAlwaysMoreBtnBgColor
        modelAlwaysItemBorderColor = t.modelAlwaysItemBorderColor
        homeImg = t.homeImg
        homeImg_Hovered = t.homeImg_Hovered
        laserPickImg = t.laserPickImg
        laserImageImg = t.laserImageImg
        laserFontImg = t.laserFontImg
        laserRectImg = t.laserRectImg
        laserArcImg = t.laserArcImg
        switchModelFDMImg = t.switchModelFDMImg
        switchModelLaserImg = t.switchModelLaserImg
        switchModelFDMImg_H = t.switchModelFDMImg_H
        switchModelLaserImg_H = t.switchModelLaserImg_H
        switchModelLaserImgEn = t.switchModelLaserImgEn
        switchModelLaserImgEn_H = t.switchModelLaserImgEn_H
        switchModelCNCImg = t.switchModelCNCImg
        switchModelCNCImg_H = t.switchModelCNCImg_H

        modleItemBorderColor = t.modleItemBorderColor
        printFanOffImg = t.printFanOffImg
        printFanOnImg = t.printFanOnImg
        printMoveAxisYUpImg = t.printMoveAxisYUpImg
        printMoveAxisYUp_HImg = t.printMoveAxisYUp_HImg
        printMoveAxisYUp_CImg = t.printMoveAxisYUp_CImg
        printMoveAxisZUpImg = t.printMoveAxisZUpImg
        printMoveAxisZUp_HImg = t.printMoveAxisZUp_HImg
        printMoveAxisZUp_CImg = t.printMoveAxisZUp_CImg
        printMoveAxisXDownImg = t.printMoveAxisXDownImg
        printMoveAxisXDown_HImg = t.printMoveAxisXDown_HImg
        printMoveAxisXDown_CImg = t.printMoveAxisXDown_CImg
        printMoveAxisZeroImg = t.printMoveAxisZeroImg
        printMoveAxisZero_HImg = t.printMoveAxisZero_HImg
        printMoveAxisZero_CImg = t.printMoveAxisZero_CImg
        printMoveAxisXUpImg = t.printMoveAxisXUpImg
        printMoveAxisXUp_HImg = t.printMoveAxisXUp_HImg
        printMoveAxisXUp_CImg = t.printMoveAxisXUp_CImg
        printMoveAxisYDownImg = t.printMoveAxisYDownImg
        printMoveAxisYDown_HImg = t.printMoveAxisYDown_HImg
        printMoveAxisYDown_CImg = t.printMoveAxisYDown_CImg
        printMoveAxisZDownImg = t.printMoveAxisZDownImg
        printMoveAxisZDown_HImg = t.printMoveAxisZDown_HImg
        printMoveAxisZDown_CImg = t.printMoveAxisZDown_CImg
        userinfoDel_HImg = t.userinfoDel_HImg
        userinfoPrint_HImg = t.userinfoPrint_HImg
        userinfoShare_HImg = t.userinfoShare_HImg
        userinfoExport_HImg = t.userinfoExport_HImg
        modelAlwaysPopBg = t.modelAlwaysPopBg
        modelAlwaysBtnIcon = t.modelAlwaysBtnIcon
        uploadModelImg = t.uploadModelImg
        uploadLocalModelImg = t.uploadLocalModelImg
        deleteModelImg = t.deleteModelImg

        editProfileImg = t.editProfileImg
        uploadProfileImg = t.uploadProfileImg
        deleteProfileImg = t.deleteProfileImg

        onSrcImg = t.onSrcImg
        offSrcImg = t.offSrcImg
        checkedSrcImg = t.checkedSrcImg
        uncheckSrcImg = t.uncheckSrcImg

        cube3DAmbientColor = t.cube3DAmbientColor
        textColor_disabled = t.textColor_disabled
        printerCommboxBtnDefaultColor = t.printerCommboxBtnDefaultColor
        printerCommboxPopBgColor = t.printerCommboxPopBgColor
        printerCommboxIndicatorBgColor = t.printerCommboxIndicatorBgColor
        printerCommboxIndicatorPopShowBgColor = t.printerCommboxIndicatorPopShowBgColor
        printerCommboxBgColor = t.printerCommboxBgColor
        printerCommboxBgBorderColor = t.printerCommboxBgBorderColor

        cube3DTopImg = t.cube3DTopImg
        cube3DTopImg_C = t.cube3DTopImg_C
        cube3DFrontImg = t.cube3DFrontImg
        cube3DFrontImg_C = t.cube3DFrontImg_C
        cube3DBottomImg = t.cube3DBottomImg
        cube3DBottomImg_C = t.cube3DBottomImg_C
        cube3DBackImg = t.cube3DBackImg
        cube3DBackImg_C = t.cube3DBackImg_C
        cube3DLeftkImg = t.cube3DLeftkImg
        cube3DLeftkImg_C = t.cube3DLeftkImg_C
        cube3DLeftkRight = t.cube3DLeftkRight
        cube3DLeftkRight_C = t.cube3DLeftkRight_C
        laserFoldTitleUpImg = t.laserFoldTitleUpImg
        laserFoldTitleDownImg = t.laserFoldTitleDownImg
        enabledBtnShadowColor = t.enabledBtnShadowColor
        laserLineBorderColor = t.laserLineBorderColor
        wifiRefreshImg = t.wifiRefreshImg
        upBtnImgSource = t.upBtnImgSource
        upBtnImgSource_d = t.upBtnImgSource_d
        downBtnImgSource = t.downBtnImgSource
        downBtnImgSource_d = t.downBtnImgSource_d

        // ---------- right panel [beg] ----------

        right_panel_text_default_color         = t.right_panel_text_default_color
        right_panel_text_disable_color         = t.right_panel_text_disable_color
        right_panel_text_hovered_color         = t.right_panel_text_hovered_color
        right_panel_text_checked_color         = t.right_panel_text_checked_color

        right_panel_button_default_color       = t.right_panel_button_default_color
        right_panel_button_disable_color       = t.right_panel_button_disable_color
        right_panel_button_hovered_color       = t.right_panel_button_hovered_color
        right_panel_button_checked_color       = t.right_panel_button_checked_color

        right_panel_border_default_color       = t.right_panel_border_default_color
        right_panel_border_disable_color       = t.right_panel_border_disable_color
        right_panel_border_hovered_color       = t.right_panel_border_hovered_color
        right_panel_border_checked_color       = t.right_panel_border_checked_color

        right_panel_item_default_color         = t.right_panel_item_default_color
        right_panel_item_disable_color         = t.right_panel_item_disable_color
        right_panel_item_hovered_color         = t.right_panel_item_hovered_color
        right_panel_item_checked_color         = t.right_panel_item_checked_color
        right_panel_item_text_default_color    = t.right_panel_item_text_default_color
        right_panel_item_text_disable_color    = t.right_panel_item_text_disable_color
        right_panel_item_text_hovered_color    = t.right_panel_item_text_hovered_color
        right_panel_item_text_checked_color    = t.right_panel_item_text_checked_color

        right_panel_combobox_background_color  = t.right_panel_combobox_background_color

        right_panel_tab_text_default_color     = t.right_panel_tab_text_default_color
        right_panel_tab_text_disable_color     = t.right_panel_tab_text_disable_color
        right_panel_tab_text_hovered_color     = t.right_panel_tab_text_hovered_color
        right_panel_tab_text_checked_color     = t.right_panel_tab_text_checked_color
        right_panel_tab_button_default_color   = t.right_panel_tab_button_default_color
        right_panel_tab_button_disable_color   = t.right_panel_tab_button_disable_color
        right_panel_tab_button_hovered_color   = t.right_panel_tab_button_hovered_color
        right_panel_tab_button_checked_color   = t.right_panel_tab_button_checked_color
        right_panel_tab_background_color       = t.right_panel_tab_background_color

        right_panel_menu_border_color          = t.right_panel_menu_border_color
        right_panel_menu_background_color      = t.right_panel_menu_background_color
        right_panel_menu_split_line_color      = t.right_panel_menu_split_line_color

        right_panel_slice_button_default_color = t.right_panel_slice_button_default_color
        right_panel_slice_button_disable_color = t.right_panel_slice_button_disable_color
        right_panel_slice_button_hovered_color = t.right_panel_slice_button_hovered_color
        right_panel_slice_button_checked_color = t.right_panel_slice_button_checked_color
        right_panel_slice_text_default_color   = t.right_panel_slice_text_default_color
        right_panel_slice_text_disable_color   = t.right_panel_slice_text_disable_color
        right_panel_slice_text_hovered_color   = t.right_panel_slice_text_hovered_color
        right_panel_slice_text_checked_color   = t.right_panel_slice_text_checked_color

        right_panel_quality_custom_default_image  = t.right_panel_quality_custom_default_image
        right_panel_quality_custom_checked_image  = t.right_panel_quality_custom_checked_image
        right_panel_quality_high_default_image    = t.right_panel_quality_high_default_image
        right_panel_quality_high_checked_image    = t.right_panel_quality_high_checked_image
        right_panel_quality_middle_default_image  = t.right_panel_quality_middle_default_image
        right_panel_quality_middle_checked_image  = t.right_panel_quality_middle_checked_image
        right_panel_quality_low_default_image     = t.right_panel_quality_low_default_image
        right_panel_quality_low_checked_image     = t.right_panel_quality_low_checked_image
        right_panel_quality_verylow_default_image = t.right_panel_quality_verylow_default_image
        right_panel_quality_verylow_checked_image = t.right_panel_quality_verylow_checked_image

        // ---------- right panel [end] ----------

        //lanPrinter Panel
        lanPrinter_panel_border = t.lanPrinter_panel_border
        lanPrinter_panel_crossline = t.lanPrinter_panel_crossline
        lanPrinter_panel_light_txt = t.lanPrinter_panel_light_txt
        lanPrinter_panel_weight_txt = t.lanPrinter_panel_weight_txt
        lanPrinter_panel_background = t.lanPrinter_panel_background
        lanPrinter_panel_btn_default = t.lanPrinter_panel_btn_default
        lanPrinter_panel_btn_hovered = t.lanPrinter_panel_btn_hovered

        //leftPopWin
        themeTextColor = t.themeTextColor
        themeColor_primary = t.themeColor_primary
        themeColor_secondary = t.themeColor_secondary
        themeColor_third = t.themeColor_third
        addModel = t.addModel
        delModel = t.delModel
        //_leftPopWin
    }
    property ListModel themes: ListModel {
        ListElement {
            name: "Dark Theme"
            mainBackgroundColor : "#363638"
           dropShadowColor : "#333333"
            headerBackgroundColor:"#1C1C1D"
            menuBarBtnColor: "#1C1C1D"
            menuBarBtnColor_hovered: "#3E3E3E"
            menuTextColor: "#000000"
            menuTextColor_hovered: "white"
            menuTextColor_normal: "#999999"
            menuStyleBgColor: "#FFFFFF"
            menuStyleBgColor_hovered: "#74C9FF"
            themeColor: "#4B4B4D"
            textColor: "#ffffff"
            invalidColor: "#782c2c"
            alterColor: "#383838"
            tipBackgroundColor: "#ffffff"
            tipBorderColor: "#767676"
            buttonColor : "#6E6E73"
            dialogBtnHoveredColor: "#86868A"
            secondBtnHoverdColor: "#666666"
            secondBtnSelectionColor: "#36B785"
            rectBorderColor : "#6C6C70"
            hoveredColor: "#333333"
            selectionColor : "#3A3A3A"
            textBackgroundColor : "#4B4B4B"
            textColor_disabled:"#AFAFAF"
            printerCommboxBtnDefaultColor: "#B7B7B7"
            printerCommboxPopBgColor: "#E1E1E1"
            printerCommboxIndicatorBgColor: "#666666"
            printerCommboxIndicatorPopShowBgColor: "#E1E1E1"
            printerCommboxBgColor: "#181818"
            printerCommboxBgBorderColor: "#333333"
            textColor_hovered: "#ffffff"
            textColor_pressed: "#d2d2d2"
            controlColor: "#333333"
            controlBorderColor: "#555555"
            imageColor: "#adafb2"
            imageColor_disabled: "#989898"
            scrollBarBackgroundColor: "#3f3f3f"
            scrollBarBackgroundColor_hovered: "#4a4a4a"
            aboutDiaItemRectBgColor: "#333333"

            //lsg
            //LeftToolBar Btn
            leftBtnBgColor_normal:  "#4b4b4d"
            leftBtnBgColor_hovered: "#68686b"
            leftBtnBgColor_selected: "#1e9be2"
            leftTextColor: "#C3C3C3"
            leftTextColor_d: "white"
            topBtnBgColor_normal:  "#2f2f30"
            topBtnBgColor_hovered: "#1e9be2"
            topBtnBorderColor_normal: "transparent"
            topBtnBordeColor_hovered: "transparent"
            topBtnTextColor_normal : "#8D8D91"
            topBtnTextColor_hovered : "#FFFFFF"

           itemBackgroundColor : "#535353"
           infoPanelColor: "#DBDADA"

           dialogTitleColor: "#6E6E73"
           dialogContentBgColor: "#535353"
           dialogTitleTextColor:"#ffffff"
           dialogBorderColor : "#262626"

            searchBtnDisableColor: "#999999"
            searchBtnNormalColor: "#1EB6E2"
            searchBtnHoveredColor: "#1E9BE2"
            typeBtnHoveredColor : "#1EB6E2"//"#686868"
            typeModelBtnListHoveredColor: "#666666"

            dialogItemRectBgColor: "#4B4B4D"
            dialogItemRectBgBorderColor : "#6E6E72"

            splitLineColor : "#68686B"
            splitLineColorDark : "#444444"
            radioCheckLabelColor: "white"
            radioCheckRadiusColor: "white"
            radioInfillBgColor: "#424242"
            radioBorderColor: "#333333"
            profileBtnColor: "#6E6E73"
            profileBtnHoverColor: "#86868A"
            tooltipBgColor: "#454545"
            checkBoxBgColor: "#424242"
            checkBoxBorderColor: "#333333"
            cmbIndicatorRectColor_pressed_basic : "transparent"
            cmbIndicatorRectColor_basic: "transparent"

            menuSplitLineColor: "#37373A"
            menuSplitLineColorleft: "#303030"
            mainViewSplitLineColor: "#0D0D0D"
            printerCombRectColor: "#D7D7D7"

            treeBackgroundColor: "#424242"
            treeItemColor:"#424242"
            treeItemColor_pressed: "#777777"

            progressBarBgColor: "#303030"
            previewPanelRecgColor: "black"
            previewPanelTextgColor: "#CDD2D7"
            tabButtonSelectColor: "#454545"
            tabButtonNormalColor: "#535353"

            userInfoPrintItem: "#D7D7D7"
            userinfoBasicItemRecColor : "#404040"

            switchModelBgColor: "#181818"
            switchModeSelectedlBgColor: "#535353"
            modleItemBorderColor: "transparent"
            sliderTopColor1: "#4A4A4A"
            sliderTopColor2: "#767676"
            sliderBottomColor: "#535353"
            modelAlwaysMoreBtnBgColor: "#D7D7D7"
            modelAlwaysItemBorderColor: "#262626"

            cube3DAmbientColor: "#828282"
            cube3DAmbientColor_Entered:"#E5E0D0"

            enabledBtnShadowColor:"black"
            tittleBarBtnColor: "#3E3E3E"
            laserFoldTittleColor: "#424242"

            laserLineBorderColor:"#999999"

            upBtnImgSource: "qrc:/UI/photo/upBtn.png"
            upBtnImgSource_d: "qrc:/UI/photo/upBtn_d.png"

            downBtnImgSource: "qrc:/UI/photo/downBtn.png"
            downBtnImgSource_d: "qrc:/UI/photo/downBtn_d.png"

            sliderBtnimg: "qrc:/UI/photo/rangSlider.png"

            clearBtnImg: "qrc:/UI/photo/clearBtn.png"
            clearBtnImg_d: "qrc:/UI/photo/clearBtn_d.png"

            sourchBtnImg: "qrc:/UI/photo/search.png"
            sourchBtnImg_d: "qrc:/UI/photo/search_d.png"
            downBtnImg: "qrc:/UI/photo/downBtn.png"
            checkBtnImg: "qrc:/UI/images/check2.png"
            showPWHoveredImg: "qrc:/UI/photo/showPW_d.png"
            hidePWHoveredImg: "qrc:/UI/photo/hidePW_d.png"
            areaCodeComboboxImg: "qrc:/UI/photo/comboboxDown.png"
            configTabBtnImg: "qrc:/UI/photo/configTabBtn.png"
            configTabBtnImg_Hovered: "qrc:/UI/photo/configTabBtn_d.png"
            supportTabBtnImg:"qrc:/UI/photo/supportTabBtn.png"
            supportTabBtnImg_Hovered:"qrc:/UI/photo/supportTabBtn_d.png"
            homeImg: "qrc:/UI/images/home.png"
            homeImg_Hovered: "qrc:/UI/images/home_s.png"
            laserPickImg: "qrc:/UI/images/laser_pick.png"
            laserImageImg: "qrc:/UI/images/laser_img.png"
            laserFontImg: "qrc:/UI/images/laser_font.png"
            laserRectImg: "qrc:/UI/images/laser_rect.png"
            laserArcImg: "qrc:/UI/images/laser_arc.png"
            switchModelFDMImg: "qrc:/UI/images/fdmMode.png"
            switchModelLaserImg: "qrc:/UI/images/laserMode.png"
            switchModelFDMImg_H: "qrc:/UI/images/fdmMode_h.png"
            switchModelLaserImg_H: "qrc:/UI/images/laserMode_h.png"
            switchModelLaserImgEn: "qrc:/UI/images/laserMode_en.png"
            switchModelLaserImgEn_H: "qrc:/UI/images/laserMode_h_en.png"
            switchModelCNCImg: "qrc:/UI/images/CNCMode.png"
            switchModelCNCImg_H: "qrc:/UI/images/CNCMode_h.png"

            printFanOffImg : "qrc:/UI/photo/print_fan_off.png"
            printFanOnImg: "qrc:/UI/photo/print_fan_on.png"
            printMoveAxisYUpImg: "qrc:/UI/photo/print_move_axis_y+.png"
            printMoveAxisYUp_HImg: "qrc:/UI/photo/print_move_axis_y+_h.png"
            printMoveAxisYUp_CImg: "qrc:/UI/photo/print_move_axis_y+_c.png"
            printMoveAxisZUpImg: "qrc:/UI/photo/print_move_axis_z+.png"
            printMoveAxisZUp_HImg: "qrc:/UI/photo/print_move_axis_z+_h.png"
            printMoveAxisZUp_CImg: "qrc:/UI/photo/print_move_axis_z+_c.png"
            printMoveAxisXDownImg: "qrc:/UI/photo/print_move_axis_x-.png"
            printMoveAxisXDown_HImg: "qrc:/UI/photo/print_move_axis_x-_h.png"
            printMoveAxisXDown_CImg: "qrc:/UI/photo/print_move_axis_x-_c.png"
            printMoveAxisZeroImg: "qrc:/UI/photo/print_move_axis_zero.png"
            printMoveAxisZero_HImg: "qrc:/UI/photo/print_move_axis_zero_h.png"
            printMoveAxisZero_CImg: "qrc:/UI/photo/print_move_axis_zero_c.png"
            printMoveAxisXUpImg: "qrc:/UI/photo/print_move_axis_x+.png"
            printMoveAxisXUp_HImg: "qrc:/UI/photo/print_move_axis_x+_h.png"
            printMoveAxisXUp_CImg: "qrc:/UI/photo/print_move_axis_x+_c.png"
            printMoveAxisYDownImg: "qrc:/UI/photo/print_move_axis_y-.png"
            printMoveAxisYDown_HImg: "qrc:/UI/photo/print_move_axis_y-_h.png"
            printMoveAxisYDown_CImg: "qrc:/UI/photo/print_move_axis_y-_c.png"
            printMoveAxisZDownImg: "qrc:/UI/photo/print_move_axis_z-.png"
            printMoveAxisZDown_HImg: "qrc:/UI/photo/print_move_axis_z-_h.png"
            printMoveAxisZDown_CImg: "qrc:/UI/photo/print_move_axis_z-_c.png"
            userinfoDel_HImg: "qrc:/UI/photo/userinfo_delete_h.png"
            userinfoPrint_HImg: "qrc:/UI/photo/userinfo_print_h.png"
            userinfoShare_HImg: "qrc:/UI/photo/userinfo_share_h.png"
            userinfoExport_HImg: "qrc:/UI/photo/userinfo_export_h.png"
            modelAlwaysPopBg: "qrc:/UI/photo/model_always_pop_bg.png"
            modelAlwaysBtnIcon: "qrc:/UI/photo/model_always_show.png"
            uploadModelImg: "qrc:/UI/photo/upload_model_img.png"
            uploadLocalModelImg: "qrc:/UI/photo/upload_model_local_img.png"
            deleteModelImg: "qrc:/UI/photo/delete_model_img.png"

            editProfileImg: "qrc:/UI/photo/editProfile.png"
            uploadProfileImg: "qrc:/UI/photo/uploadPro.png"
            deleteProfileImg: "qrc:/UI/photo/deleteProfile.png"

            onSrcImg: "qrc:/UI/photo/on.png"
            offSrcImg: "qrc:/UI/photo/off.png"
            checkedSrcImg: "qrc:/UI/photo/radio_1.png"
            uncheckSrcImg: "qrc:/UI/photo/radio_2.png"

            cube3DTopImg: "qrc:/UI/images/top.png"
            cube3DTopImg_C: "qrc:/UI/images/top_C.png"
            cube3DFrontImg: "qrc:/UI/images/front.png"
            cube3DFrontImg_C: "qrc:/UI/images/front_C.png"
            cube3DBottomImg: "qrc:/UI/images/bottom.png"
            cube3DBottomImg_C: "qrc:/UI/images/bottom_C.png"
            cube3DBackImg: "qrc:/UI/images/back.png"
            cube3DBackImg_C: "qrc:/UI/images/back_C.png"
            cube3DLeftkImg: "qrc:/UI/images/left.png"
            cube3DLeftkImg_C: "qrc:/UI/images/left_C.png"
            cube3DLeftkRight: "qrc:/UI/images/right.png"
            cube3DLeftkRight_C: "qrc:/UI/images/right_C.png"
            laserFoldTitleUpImg: "qrc:/UI/photo/laser_fold_item_up.png"
            laserFoldTitleDownImg: "qrc:/UI/photo/laser_fold_item_down.png"
            wifiRefreshImg: "qrc:/UI/photo/refresh.png"

            // ---------- right panel [beg] ----------

            right_panel_text_default_color         : "#FFFFFF"
            right_panel_text_disable_color         : "#FFFFFF"
            right_panel_text_hovered_color         : "#FFFFFF"
            right_panel_text_checked_color         : "#FFFFFF"

            right_panel_button_default_color       : "#4B4B4D"
            right_panel_button_disable_color       : "#4B4B4D"
            right_panel_button_hovered_color       : "#414143"
            right_panel_button_checked_color       : "#414143"

            right_panel_border_default_color       : "#6C6C70"
            right_panel_border_disable_color       : "#6C6C70"
            right_panel_border_hovered_color       : "#6C6C70"
            right_panel_border_checked_color       : "#6C6C70"

            right_panel_item_default_color         : "#414143"
            right_panel_item_disable_color         : "#414143"
            right_panel_item_hovered_color         : "#5F5F5F"
            right_panel_item_checked_color         : "#739AB0"
            right_panel_item_text_default_color    : "#CBCBCB"
            right_panel_item_text_disable_color    : "#CBCBCB"
            right_panel_item_text_hovered_color    : "#CBCBCB"
            right_panel_item_text_checked_color    : "#FFFFFF"

            right_panel_combobox_background_color  : "#414143"

            right_panel_tab_text_default_color     : "#8D8D91"
            right_panel_tab_text_disable_color     : "#8D8D91"
            right_panel_tab_text_hovered_color     : "#8D8D91"
            right_panel_tab_text_checked_color     : "#FFFFFF"
            right_panel_tab_button_default_color   : "#1C1C1D"
            right_panel_tab_button_disable_color   : "#1C1C1D"
            right_panel_tab_button_hovered_color   : "#1C1C1D"
            right_panel_tab_button_checked_color   : "#1E9BE2"
            right_panel_tab_background_color       : "#1C1C1D"

            right_panel_menu_border_color          : "#1C1C1D"
            right_panel_menu_background_color      : "#4B4B4D"
            right_panel_menu_split_line_color      : "#3B3B3D"

            right_panel_slice_button_default_color : "#1E9BE2"
            right_panel_slice_button_disable_color : "#626265"
            right_panel_slice_button_hovered_color : "#57B4E9"
            right_panel_slice_button_checked_color : "#57B4E9"
            right_panel_slice_text_default_color   : "#FFFFFF"
            right_panel_slice_text_disable_color   : "#B1B1B7"
            right_panel_slice_text_hovered_color   : "#FFFFFF"
            right_panel_slice_text_checked_color   : "#FFFFFF"

            right_panel_quality_custom_default_image  : "qrc:/UI/photo/config_quality_custom.png"
            right_panel_quality_custom_checked_image  : "qrc:/UI/photo/config_quality_custom_checked.png"
            right_panel_quality_high_default_image    : "qrc:/UI/photo/config_quality_high.png"
            right_panel_quality_high_checked_image    : "qrc:/UI/photo/config_quality_high_checked.png"
            right_panel_quality_middle_default_image  : "qrc:/UI/photo/config_quality_middle.png"
            right_panel_quality_middle_checked_image  : "qrc:/UI/photo/config_quality_middle_checked.png"
            right_panel_quality_low_default_image     : "qrc:/UI/photo/config_quality_low.png"
            right_panel_quality_low_checked_image     : "qrc:/UI/photo/config_quality_low_checked.png"
            right_panel_quality_verylow_default_image : "qrc:/UI/photo/config_quality_verylow.png"
            right_panel_quality_verylow_checked_image : "qrc:/UI/photo/config_quality_verylow_checked.png"

            // ---------- right panel [end] ----------

            //lanPrinter Panel
            lanPrinter_panel_border: "transparent"
            lanPrinter_panel_crossline: "#0F0F10"
            lanPrinter_panel_light_txt: "#FFFFFF"
            lanPrinter_panel_weight_txt: "#FFFFFF"
            lanPrinter_panel_background: "#242426"
            lanPrinter_panel_btn_default: "#2E2E30"
            lanPrinter_panel_btn_hovered: "#414143"

            //leftPopWin
            themeTextColor: "#ffffff"
            themeColor_primary: "#4b4b4d"
            themeColor_secondary: "#6e6e73"
            themeColor_third: "#6e6e73"
            addModel: "qrc:/UI/photo/addModel.png"
            delModel: "qrc:/UI/photo/deleteModel.png"
        }
        ListElement {
            name: "Light Theme"
            mainBackgroundColor:"#F2F2F5"
            dropShadowColor : "#BBBBBB"
            headerBackgroundColor:"#FFFFFF"//"#333333"
            menuBarBtnColor: "#FFFFFF"
            menuBarBtnColor_hovered: "#E9E9E9"
            menuTextColor: "#333333"
            menuTextColor_hovered: "black"
            menuTextColor_normal: "black"
            menuStyleBgColor: "#FFFFFF"
            menuStyleBgColor_hovered: "#74C9FF"
            themeColor: "#FFFFFF"//"#485359"
            textColor: "#333333"//"#373737"
            buttonColor : "#FFFFFF"
            dialogBtnHoveredColor: "#ADADAD"
            secondBtnHoverdColor: "#ECECEC"
            secondBtnSelectionColor: "#58CD9F"
            hoveredColor : "#1E9BE2"
            selectionColor : "#3A3A3A"
            rectBorderColor : "#CBCBCC"
            textBackgroundColor : "#F9F9F9"
            tipBackgroundColor: "#ffffff"
            tipBorderColor: "#767676"
            controlColor: "#f5f5f6"
            controlBorderColor: "#cbcbcb"
            aboutDiaItemRectBgColor: "#535353"

            //lsg
            //LeftToolBar Btn
            leftBtnBgColor_normal:  "#FFFFFF"
            leftBtnBgColor_hovered: "#DAF2FF"
            leftBtnBgColor_selected: "#1E9BE2"
            leftTextColor: "#333333"
            leftTextColor_d: "white"
            topBtnBgColor_normal:  "#FFFFFF"
            topBtnBgColor_hovered: "#1e9be2"
            topBtnBorderColor_normal: "#D6D6DC"
            topBtnBordeColor_hovered: "transparent"
            topBtnTextColor_normal : "#999999"
            topBtnTextColor_hovered : "#FFFFFF"

            itemBackgroundColor : "#FFFFFF"//"#485359"
            infoPanelColor: "#333333"

            dialogTitleColor : "#ffffff"
            dialogContentBgColor: "#FFFFFF"
            dialogTitleTextColor:"#333333"
            dialogBorderColor : "transparent"

            searchBtnDisableColor: "#E5E5E9"
            searchBtnNormalColor: "#1E9BE2"
            searchBtnHoveredColor: "#1EB6E2"
            typeBtnHoveredColor : "#ECECEC" //"#1EB6E2"//"#E1E1E1"
            typeModelBtnListHoveredColor: "#ECECEC"

            dialogItemRectBgColor: "#FFFFFF"
            dialogItemRectBgBorderColor: "#CBCBCC"

            splitLineColor : "#DFDFE3"
            splitLineColorDark : "#FFFFFF"
            radioCheckLabelColor: "#333333"
            radioCheckRadiusColor: "#333333"
            radioInfillBgColor: "#F9F9F9"
            radioBorderColor: "#828790"
            profileBtnColor: "#ECECEC"
            profileBtnHoverColor: "#C2C2C5"
            tooltipBgColor: "#F9F9F9"
            checkBoxBgColor: "#F9F9F9"
            checkBoxBorderColor: "#828790"
            cmbIndicatorRectColor_pressed_basic : "transparent"
            cmbIndicatorRectColor_basic: "transparent"

            menuSplitLineColor: "#DCDCDC"
            menuSplitLineColorleft: "#E5E5E9"
            mainViewSplitLineColor: "#cacaca"
            printerCombRectColor: "#C5C5CA"

            treeBackgroundColor: "#F9F9F9"
            treeItemColor:"#F9F9F9"
            treeItemColor_pressed: "#D8D8DA"

            userInfoPrintItem: "#FFFFFF"
            userinfoBasicItemRecColor : "#FFFFFF"

            progressBarBgColor: "#DFDFE4"
            previewPanelRecgColor: "#959596"
            previewPanelTextgColor: "#333333"
            tabButtonSelectColor: "#F9F9F9"
            tabButtonNormalColor: "#C5C5CA"

            switchModelBgColor: "#C5C5CA"
            switchModeSelectedlBgColor: "#FFFFFF"
            modleItemBorderColor: "#E9E9EC"
            sliderTopColor1: "#20ACFB"
            sliderTopColor2: "#20ACFB"
            sliderBottomColor: "#D8D8DC"
            modelAlwaysMoreBtnBgColor: "#FFFFFF"
            modelAlwaysItemBorderColor: "#727273"

            cube3DAmbientColor: "#B8B8B8"
            cube3DAmbientColor_Entered:"#A8A8A8"
            textColor_disabled:"#AFAFAF"
            printerCommboxBtnDefaultColor: "#D8D8DA"
            printerCommboxPopBgColor: "#F2F2F5"
            printerCommboxIndicatorBgColor: "#C5C5CA"
            printerCommboxIndicatorPopShowBgColor: "#C5C5CA"
            printerCommboxBgColor:"#DCDCDF" 
            printerCommboxBgBorderColor: "#C5C5CA"

            enabledBtnShadowColor:"#C7C7CA"
            tittleBarBtnColor: "#ceced0"
            laserFoldTittleColor: "#D8D8DA"

            laserLineBorderColor:"#333333"

            upBtnImgSource: "qrc:/UI/photo/upBtn_white.png"
            upBtnImgSource_d: "qrc:/UI/photo/upBtn_white_d.png"
            downBtnImgSource: "qrc:/UI/photo/downBtn_white.png"
            downBtnImgSource_d: "qrc:/UI/photo/downBtn_white_d.png"

            sliderBtnimg: "qrc:/UI/photo/rangSlider2.png"

            clearBtnImg: "qrc:/UI/photo/clearBtn2.png"
            clearBtnImg_d: "qrc:/UI/photo/clearBtn2_white_d.png"
            sourchBtnImg: "qrc:/UI/photo/search.png"
            sourchBtnImg_d: "qrc:/UI/photo/search_white_d.png"
            downBtnImg: "qrc:/UI/photo/down.png"
            checkBtnImg: "qrc:/UI/images/check3.png"
            showPWHoveredImg: "qrc:/UI/photo/showPW2_d.png"
            hidePWHoveredImg: "qrc:/UI/photo/hidePW2_d.png"
            areaCodeComboboxImg: "qrc:/UI/photo/comboboxDown2.png"
            configTabBtnImg: "qrc:/UI/photo/configTabBtn2.png"
            configTabBtnImg_Hovered: "qrc:/UI/photo/configTabBtn2_d.png"
            supportTabBtnImg:"qrc:/UI/photo/supportTabBtn2.png"
            supportTabBtnImg_Hovered:"qrc:/UI/photo/supportTabBtn2_d.png"
            homeImg: "qrc:/UI/images/home2.png"
            homeImg_Hovered: "qrc:/UI/images/home2_s.png"
            laserPickImg: "qrc:/UI/images/laser_pick2.png"
            laserImageImg: "qrc:/UI/images/laser_img2.png"
            laserFontImg: "qrc:/UI/images/laser_font2.png"
            laserRectImg: "qrc:/UI/images/laser_rect2.png"
            laserArcImg: "qrc:/UI/images/laser_arc2.png"
            switchModelFDMImg: "qrc:/UI/images/fdmMode2.png"
            switchModelLaserImg: "qrc:/UI/images/laserMode2.png"
            switchModelFDMImg_H: "qrc:/UI/images/fdmMode2_h.png"
            switchModelLaserImg_H: "qrc:/UI/images/laserMode2_h.png"
            switchModelLaserImgEn: "qrc:/UI/images/laserMode2_en.png"
            switchModelLaserImgEn_H: "qrc:/UI/images/laserMode2_h_en.png"
            switchModelCNCImg: "qrc:/UI/images/CNCMode2.png"
            switchModelCNCImg_H: "qrc:/UI/images/CNCMode2_h.png"

            printFanOffImg : "qrc:/UI/photo/print_fan_off2.png"
            printFanOnImg: "qrc:/UI/photo/print_fan_on2.png"
            printMoveAxisYUpImg: "qrc:/UI/photo/print_move_axis_y+2.png"
            printMoveAxisYUp_HImg: "qrc:/UI/photo/print_move_axis_y+2_h.png"
            printMoveAxisYUp_CImg: "qrc:/UI/photo/print_move_axis_y+2_c.png"
            printMoveAxisZUpImg: "qrc:/UI/photo/print_move_axis_z+2.png"
            printMoveAxisZUp_HImg: "qrc:/UI/photo/print_move_axis_z+2_h.png"
            printMoveAxisZUp_CImg: "qrc:/UI/photo/print_move_axis_z+2_c.png"
            printMoveAxisXDownImg: "qrc:/UI/photo/print_move_axis_x-2.png"
            printMoveAxisXDown_HImg: "qrc:/UI/photo/print_move_axis_x-2_h.png"
            printMoveAxisXDown_CImg: "qrc:/UI/photo/print_move_axis_x-2_c.png"
            printMoveAxisZeroImg: "qrc:/UI/photo/print_move_axis_zero2.png"
            printMoveAxisZero_HImg: "qrc:/UI/photo/print_move_axis_zero2_h.png"
            printMoveAxisZero_CImg: "qrc:/UI/photo/print_move_axis_zero2_c.png"
            printMoveAxisXUpImg: "qrc:/UI/photo/print_move_axis_x+2.png"
            printMoveAxisXUp_HImg: "qrc:/UI/photo/print_move_axis_x+2_h.png"
            printMoveAxisXUp_CImg: "qrc:/UI/photo/print_move_axis_x+2_c.png"
            printMoveAxisYDownImg: "qrc:/UI/photo/print_move_axis_y-2.png"
            printMoveAxisYDown_HImg: "qrc:/UI/photo/print_move_axis_y-2_h.png"
            printMoveAxisYDown_CImg: "qrc:/UI/photo/print_move_axis_y-2_c.png"
            printMoveAxisZDownImg: "qrc:/UI/photo/print_move_axis_z-2.png"
            printMoveAxisZDown_HImg: "qrc:/UI/photo/print_move_axis_z-2_h.png"
            printMoveAxisZDown_CImg: "qrc:/UI/photo/print_move_axis_z-2_c.png"
            userinfoDel_HImg: "qrc:/UI/photo/userinfo_delete2_h.png"
            userinfoPrint_HImg: "qrc:/UI/photo/userinfo_print2_h.png"
            userinfoShare_HImg: "qrc:/UI/photo/userinfo_share2_h.png"
            userinfoExport_HImg: "qrc:/UI/photo/userinfo_export2_h.png"
            modelAlwaysPopBg: "qrc:/UI/photo/model_always_pop_bg2.png"
            modelAlwaysBtnIcon: "qrc:/UI/photo/model_always_show2.png"
            uploadModelImg: "qrc:/UI/photo/upload_model_img2.png"
            uploadLocalModelImg: "qrc:/UI/photo/upload_model_local_img2.png"
            deleteModelImg: "qrc:/UI/photo/delete_model_img2.png"

            editProfileImg: "qrc:/UI/photo/editProfile2.png"
            uploadProfileImg: "qrc:/UI/photo/uploadPro.png"
            deleteProfileImg: "qrc:/UI/photo/deleteProfile.png"

            onSrcImg: "qrc:/UI/photo/on2.png"
            offSrcImg: "qrc:/UI/photo/off2.png"
            checkedSrcImg: "qrc:/UI/photo/radio2_1.png"
            uncheckSrcImg: "qrc:/UI/photo/radio2_2.png"

            cube3DTopImg: "qrc:/UI/images/top2.png"
            cube3DTopImg_C: "qrc:/UI/images/top2_C.png"
            cube3DFrontImg: "qrc:/UI/images/front2.png"
            cube3DFrontImg_C: "qrc:/UI/images/front2_C.png"
            cube3DBottomImg: "qrc:/UI/images/bottom2.png"
            cube3DBottomImg_C: "qrc:/UI/images/bottom2_C.png"
            cube3DBackImg: "qrc:/UI/images/back2.png"
            cube3DBackImg_C: "qrc:/UI/images/back2_C.png"
            cube3DLeftkImg: "qrc:/UI/images/left2.png"
            cube3DLeftkImg_C: "qrc:/UI/images/left2_C.png"
            cube3DLeftkRight: "qrc:/UI/images/right2.png"
            cube3DLeftkRight_C: "qrc:/UI/images/right2_C.png"
            laserFoldTitleUpImg: "qrc:/UI/photo/laser_fold_item_up2.png"
            laserFoldTitleDownImg: "qrc:/UI/photo/laser_fold_item_down2.png"
            wifiRefreshImg: "qrc:/UI/photo/refresh2.png"

            // ---------- right panel [beg] ----------

            right_panel_text_default_color         : "#333333"
            right_panel_text_disable_color         : "#333333"
            right_panel_text_hovered_color         : "#333333"
            right_panel_text_checked_color         : "#333333"

            right_panel_button_default_color       : "#FFFFFF"
            right_panel_button_disable_color       : "#FFFFFF"
            right_panel_button_hovered_color       : "#DCDCE4"
            right_panel_button_checked_color       : "#DCDCE4"

            right_panel_border_default_color       : "#D6D6DC"
            right_panel_border_disable_color       : "#D6D6DC"
            right_panel_border_hovered_color       : "#D6D6DC"
            right_panel_border_checked_color       : "#D6D6DC"

            right_panel_item_default_color         : "#FFFFFF"
            right_panel_item_disable_color         : "#FFFFFF"
            right_panel_item_hovered_color         : "#D6D6DC"
            right_panel_item_checked_color         : "#B7E5FF"
            right_panel_item_text_default_color    : "#333333"
            right_panel_item_text_disable_color    : "#333333"
            right_panel_item_text_hovered_color    : "#333333"
            right_panel_item_text_checked_color    : "#333333"

            right_panel_combobox_background_color  : "#FFFFFF"

            right_panel_tab_text_default_color     : "#8D8D90"
            right_panel_tab_text_disable_color     : "#8D8D90"
            right_panel_tab_text_hovered_color     : "#FFFFFF"
            right_panel_tab_text_checked_color     : "#FFFFFF"
            right_panel_tab_button_default_color   : "#FFFFFF"
            right_panel_tab_button_disable_color   : "#FFFFFF"
            right_panel_tab_button_hovered_color   : "#1E9BE2"
            right_panel_tab_button_checked_color   : "#1E9BE2"
            right_panel_tab_background_color       : "#FFFFFF"

            right_panel_menu_border_color          : "#D6D6DC"
            right_panel_menu_background_color      : "#FFFFFF"
            right_panel_menu_split_line_color      : "#D6D6DC"

            right_panel_slice_button_default_color : "#1E9BE2"
            right_panel_slice_button_disable_color : "#DCDCE4"
            right_panel_slice_button_hovered_color : "#57B4E9"
            right_panel_slice_button_checked_color : "#57B4E9"
            right_panel_slice_text_default_color   : "#FFFFFF"
            right_panel_slice_text_disable_color   : "#FFFFFF"
            right_panel_slice_text_hovered_color   : "#FFFFFF"
            right_panel_slice_text_checked_color   : "#FFFFFF"

            right_panel_quality_custom_default_image  : "qrc:/UI/photo/config_quality_custom.png"
            right_panel_quality_custom_checked_image  : "qrc:/UI/photo/config_quality_custom.png"
            right_panel_quality_high_default_image    : "qrc:/UI/photo/config_quality_high.png"
            right_panel_quality_high_checked_image    : "qrc:/UI/photo/config_quality_high.png"
            right_panel_quality_middle_default_image  : "qrc:/UI/photo/config_quality_middle.png"
            right_panel_quality_middle_checked_image  : "qrc:/UI/photo/config_quality_middle.png"
            right_panel_quality_low_default_image     : "qrc:/UI/photo/config_quality_low.png"
            right_panel_quality_low_checked_image     : "qrc:/UI/photo/config_quality_low.png"
            right_panel_quality_verylow_default_image : "qrc:/UI/photo/config_quality_verylow.png"
            right_panel_quality_verylow_checked_image : "qrc:/UI/photo/config_quality_verylow.png"

            // ---------- right panel [end] ----------

            //lanPrinter Panel
            lanPrinter_panel_border: "#D6D6DC"
            lanPrinter_panel_crossline: "#D6D6DC"
            lanPrinter_panel_light_txt: "#666666"
            lanPrinter_panel_weight_txt: "#333333"
            lanPrinter_panel_background: "#F2F2F5"
            lanPrinter_panel_btn_default: "#E8E8ED"
            lanPrinter_panel_btn_hovered: "#FFFFFF"

            //leftPopWin
            themeTextColor: "#333333"
            themeColor_primary: "#ffffff"
            themeColor_secondary: "#dddde1"
            themeColor_third: "#ffffff"
            addModel: "qrc:/UI/photo/addModel_light.png"
            delModel: "qrc:/UI/photo/delModel_light.png"
        }
    }
}
