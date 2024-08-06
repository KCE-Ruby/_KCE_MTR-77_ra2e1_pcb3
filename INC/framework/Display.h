/**
 ******************************************************************************
 * @file    Display.h
 * @author  KCE R&D Team
 * @date    2024-06-09
 * @author  Ruby
 *
 * @copyright Copyright © 2024 KCE. All rights reserved.
 ******************************************************************************
 */

#define INC_DISPLAY_H_ 
#ifdef INC_DISPLAY_H_

#define SMG_1_OPEN R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_00, BSP_IO_LEVEL_LOW)
#define SMG_2_OPEN R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_01, BSP_IO_LEVEL_LOW)
#define SMG_3_OPEN R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_02, BSP_IO_LEVEL_LOW)
#define SMG_4_OPEN R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_03, BSP_IO_LEVEL_LOW)
#define SMG_5_OPEN R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_09_PIN_13, BSP_IO_LEVEL_LOW)
#define SMG_6_OPEN R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_04_PIN_07, BSP_IO_LEVEL_LOW)

#define SMG_1_CLOSE R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_00, BSP_IO_LEVEL_HIGH)
#define SMG_2_CLOSE R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_01, BSP_IO_LEVEL_HIGH)
#define SMG_3_CLOSE R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_02, BSP_IO_LEVEL_HIGH)
#define SMG_4_CLOSE R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_03, BSP_IO_LEVEL_HIGH)
#define SMG_5_CLOSE R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_09_PIN_13, BSP_IO_LEVEL_HIGH)
#define SMG_6_CLOSE R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_04_PIN_07, BSP_IO_LEVEL_HIGH)

#define SMG_A_OPEN  R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_00_PIN_15, BSP_IO_LEVEL_HIGH)
#define SMG_B_OPEN  R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_04_PIN_09, BSP_IO_LEVEL_HIGH)
#define SMG_C_OPEN  R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_05_PIN_00, BSP_IO_LEVEL_HIGH)
#define SMG_D_OPEN  R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_12, BSP_IO_LEVEL_HIGH)
#define SMG_E_OPEN  R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_11, BSP_IO_LEVEL_HIGH)
#define SMG_F_OPEN  R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_04_PIN_08, BSP_IO_LEVEL_HIGH)
#define SMG_G_OPEN  R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_00_PIN_14, BSP_IO_LEVEL_HIGH)
#define SMG_DP_OPEN R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_04, BSP_IO_LEVEL_HIGH)

#define SMG_A_CLOSE R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_00_PIN_15, BSP_IO_LEVEL_LOW)
#define SMG_B_CLOSE R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_04_PIN_09, BSP_IO_LEVEL_LOW)
#define SMG_C_CLOSE R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_05_PIN_00, BSP_IO_LEVEL_LOW)
#define SMG_D_CLOSE R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_12, BSP_IO_LEVEL_LOW)
#define SMG_E_CLOSE R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_11, BSP_IO_LEVEL_LOW)
#define SMG_F_CLOSE R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_04_PIN_08, BSP_IO_LEVEL_LOW)
#define SMG_G_CLOSE R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_00_PIN_14, BSP_IO_LEVEL_LOW)
#define SMG_DP_CLOSE R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_04, BSP_IO_LEVEL_LOW)

/* Enum type definitions -----------------------------------------------------*/
enum table_location
{
  table_2 = 0, //高位
  table_1 = 1, //中位
  table_0 = 2, //低位
};

enum scan_location
{
  Scan_3 = 3, //負號位
  Scan_2 = 2, //百位
  Scan_1 = 1, //十位
  Scan_0 = 0, //個位
};

enum UserDefineByteTables
{
  xxx,    // 0: all digits off
  //7.1 調節參數
    Hy,     //當量測溫度>=SET+Hy時, 壓縮機運行, 當溫度小於SET時, 壓縮機停止
    LS,     //設定點最小允許值, 範圍:攝氏-50 ~ SET度
    US,     //設定點最大允許值, 範圍:攝氏SET ~ 110度
    Ot,     //蒸發器探頭1(P1)校正調整值, 範圍:攝氏-12.0 ~ 12.0度
    P2P,    //蒸發器探頭2(P2)是否存在, n=不存在; y=存在, 將影響融霜終止時間的依據
    OE,     //蒸發器探頭2(P2)校正調整值, 範圍:攝氏-12.0 ~ 12.0度
    P3P,    //蒸發器探頭3(P3)是否存在, n=不存在(輸入類型=數位輸入); y=存在(輸入類型=探頭3)
    O3,     //蒸發器探頭3(P3)校正調整值, 範圍:攝氏-12.0 ~ 12.0度
    P4P,    //蒸發器探頭4(P4)是否存在, n=不存在; y=存在
    O4,     //蒸發器探頭4(P4)校正調整值, 範圍:攝氏-12.0 ~ 12.0度
    OdS,    //上電輸出延時(0 ~ 255分鐘), 上電後輸出保持未通電的狀態時間
    AC,     //防頻繁啟動延時(0 ~ 50分鐘), 壓縮機快速開關的延遲時間
    rtr,    //規定用於控制調節時, P1與P2的比例, 範圍:0~100, [公式:(P1-P2)/100+P2), 100=P1; 0=P2]
    CCt,    //強冷凍循環時, 設定強制運行壓縮機時間, 範圍:0.0~24.0小時; 分辨率:10分鐘
    CCS,    //強冷凍循環溫度設定值, 範圍:攝氏-50 ~ 150度
    COn,    //探頭失靈時, 壓縮機開機運轉時間, 範圍:0 ~ 255分鐘, COn=0、COF!=0時, 壓縮機總是停機
    COF,    //探頭失靈時, 壓縮機停機運轉時間, 範圍:0 ~ 255分鐘, COn!=0、COF=0時, 壓縮機總是開機

  //7.2 顯示參數
    CF,     //量測單位 (攝氏度/華氏度), 調整後須確認Hy, LS, US, Ot, ALU, ALL等參數, 必要時須同時修改
    rES,    //resolution 分辨率(僅限攝氏模式下使用), in=整數; dE=小數
    Lod,    //預設的面板顯示選擇, (P1~P4, SET, dtr=顯示按照參數dtr設定的探頭所占百分比讀數)
    rEd,    //ATC遠程顯示選擇 (此功能為加購選項, 需訂貨時指明), 就是指定在遠程要顯示的面板畫面
    dLY,    //顯示刷新時間間隔 (0~20.0分鐘, 分辨率: 10秒), 當溫度上升, 溫度顯示延遲時間
    dtr,    //設定當Lod = dtr用於顯示的P1與P2所佔的百分比, 範圍:0~100, [公式:(P1-P2)/100+P2), 100=P1; 0=P2]

  //7.3 融霜參數
    dFP,    //融霜終止檢測探頭選擇, nP=無探頭, P1~P4
    tdF,    //融霜類型: EL=電熱(融霜時壓縮機關閉), in=熱氣(融霜時壓縮機運轉)
    dtE,    //融霜終止溫度, 範圍:攝氏-50 ~ 50度, 只有P2P=y時可用, 當P1溫度超過此設定值, 融霜終止
    IdF,    //融霜間隔: 範圍:0 ~ 120小時, 設定融霜起始點之間的時間間隔
    // did,    //數字輸入報警延遲, 範圍:0~255分鐘, 當i1F=EAL或bAL時, 從檢測到報警到發出警報信號之間的延遲時間
    // nPS,    //壓力開關中斷次數, 範圍:0~15次, 當開關中斷次數在did設定時間內達到此設定值時報警, 只能斷電再開才能復位警報
    // odc,    //門開時壓縮機和風扇的狀態, no=維持原來狀態; Fan=僅風扇關閉; CPr=僅壓縮機關閉; F_C=風扇壓縮機都關閉
    // rrd,    //開門報警延時did後, 輸出是否重啟, 當i1F=dor時, no=輸出不變, yES=輸出重啟
    // HES,    //節能設置週期溫度升高度數, 範圍:攝氏-30 ~ 30度
    MdF,    //融霜允許最大時間, 範圍:0 ~ 255分鐘(=0時不融霜), 當P2P=n基於此參數時間融霜; P2P=y基於溫度融霜但時間不能超過此參數設定值
    dSd,    //啟動融霜延時, 範圍:0 ~ 99分鐘, 當有多個溫控器時, 用這個參數來錯開融霜啟動時間避免系統負載過大跳電
    dFd,    //融霜期間顯示, rt=實際庫溫; it=融霜啟動前的庫溫; SEt=設定點; dEF=代表融霜中
    dAd,    //融霜結束後的庫溫顯示最大延時, 範圍:0 ~255分鐘, 融霜結束到恢復庫溫顯示之間的最大延時時間
    Fdt,    //滴水時間, 範圍:0 ~ 120分鐘, 到達融霜終止溫度到正常製冷運行的間隔時間
    dPo,    //上電融霜是否允許, y=上電啟動融霜; n=上電不啟動融霜
    dAF,    //強冷凍循環後的融霜延時, 範圍:0 ~ 23.5小時, 設定強冷凍循環後延遲多久開始融霜

  //7.4 蒸發器風扇參數
    FnC,    //風扇運行模式
            //C-n=風扇與壓縮機同開同停融霜時停止; o_n=風扇除了融霜期間停止外其他時間一直運行;
            //C_Y=風扇與壓縮機同開同停融霜期間運轉; o_Y=風扇持續運轉(包含融霜期間)
    Fnd,    //融霜後風扇啟動延時, 範圍:0 ~ 255分鐘, 設定從容霜結束到風扇運轉的延遲時間
    Fct,    //避免風扇頻繁起停的溫差, 範圍:攝氏0 ~ 50度(0=此功能無效), 庫溫與蒸發器溫差大於此參數風扇才運轉
    FSt,    //風扇停止溫度, 範圍:攝氏-50 ~ 50度, 當蒸發器溫度超過此溫度時, 風扇總是停止
    Fon,    //壓縮機停止時風扇運轉時間, 範圍:0 ~ 15分鐘,
            //當Fon!=0, FoF=0時, 蒸發器風扇在製冷期間一直運轉
            //當Fon=0, FoF=0時, 蒸發器風扇在製冷期間一直保持停止
    FoF,    //壓縮機停止時風扇停止時間, 範圍:0 ~ 15分鐘,
            //當(Fon=0, FoF!=0) 或 (Fon=0, FoF=0)時, 蒸發器風扇在製冷期間一直保持停止
    FAP,    //風扇控制探頭選擇, nP=無融霜終止探頭; P1=庫溫探頭; P2=蒸發器探頭; P3=探頭3; P4=探頭4

  //7.5 報警參數
    ALC,    //溫度報警配置, Ab=絕對溫度(ALL與ALU的設定值為絕對溫度); rE=溫度報警參數(溫度大於SET+ALU或是小於SET-ALL時發出警報)
    ALU,    //高溫警報設定值, ALC=rE, 範圍:攝氏0 ~ 50度; ALC=Ab, 範圍:攝氏ALL ~ 110度, 當>=此設定且經過ALd延時後, 發出HA高溫警報訊號
    ALL,    //低溫報警設定 ALC=rE, 範圍:攝氏0 ~ 50度; ALC=Ab, 範圍:攝氏-50 ~ ALU度, 當<=此設定且經過ALd延時後, 發出LA低溫警報訊號
    AFH,    //溫度報警及風扇停止回歸差值, 範圍:攝氏0.1 ~ 25.5度, 與FSt設定的回歸差值, 總是正值
    ALd,    //溫度報警延時, 範圍:0 ~ 255分鐘, 從檢測到報警繼電器動作的時間間隔
    dAO,    //上電啟動時溫度報警延時, 範圍:0.0分 ~ 23.5小時, 上電到報警繼電器動作的時間間隔

  //7.6 冷凝溫度報警
    AP2,    //冷凝溫度報警檢測探頭選擇, nP, P1, P2, P3, P4
    AL2,    //冷凝溫度低溫警報, 範圍:攝氏-55 ~ -150度, 當溫度低於此參數時顯示LA2, 並延時Ad2
    Au2,    //冷凝溫度高溫警報, 範圍:攝氏-55 ~ -150度, 當溫度低於此參數時顯示HA2, 並延時Ad2
    AH2,    //冷凝溫度警報復位溫, 範圍:攝氏0.1 ~ 25.5度
    Ad2,    //冷凝溫度警報延時, 範圍:0 ~ 255分鐘
    dA2,    //上電啟動時冷凝溫度警報延時, 範圍:0.0 ~ 23.5小時, 分辨率10分鐘
    bLL,    //冷凝溫度低溫警報時壓縮機是否停機, n=不停機; y= 停機, 自動調節在AC分鐘後重新啟動
    AC2,    //冷凝溫度高溫警報時壓縮機是否停機, n=不停機; y= 停機, 自動調節在AC分鐘後重新啟動

  //7.7 數字輸入參數
    i1P,    //可設置數字輸入極性, CL:閉合時有效; OP:斷開時有效
    i1F,    //可設置數字輸入工作模式, EAL=一般報警, 發生時會顯示EA; bAL=嚴重報警, 發生時顯示CA; PAL=壓力開關報警, 發生時顯示CA
            //dor=門開關功能; dEF=啟動融霜; AUS=不要選用; Htr=控制方向轉換(製冷-製熱);FAn=不要設置此值; ES=節能設置
    did,    //數字輸入報警延時, 範圍:0 ~ 255分鐘, 當i1F=EAL或bAL時代表報警延時, 當i1F=dor時, 開門報警延時;
            //當i1F=PAL時, 此參數為計算壓力開關中斷次數的計時時間, 在時間內次數到達nPS次, 則確認警報有效
    nPS,    //壓力開關次數, 範圍:0 ~ 15次, 與did搭配, 若警報有效則只能透過斷開控制器電源再上電, 才能恢復正常使用
    odc,    //門開時壓縮機和蒸發器風扇的狀態, no=維持原來狀態; Fan=僅風扇關閉; CPr=僅壓縮機關閉; F_C=風扇壓縮機都關閉
    rrd,    //開門警報延時did後輸出是否重啟, 當i1F=dor時, no=警報延時後輸出不變, yES=警報延時後輸出重啟
    HES,    //節能設置週期溫度升高度數, 範圍:-30 ~ 30度

  //7.8 其他參數
    Adr,    //RS485地址, 範圍:1 ~ 244
    PbC,    //探頭類型選擇, Ptc=PTC探頭; ntc=NTC探頭
    onF,    //待機鍵操作功能, nu=無任何功能; oFF待機允許; ES=節能運行
    dp1,    //探頭1讀數, 庫溫
    dp2,    //探頭2讀數, 蒸發器溫度
    dp3,    //探頭3讀數, 輔助探頭, 有探頭才可用
    dp4,    //探頭4讀數, 輔助探頭
    rSE,    //正常運行時的實際設定點, 只能讀取
    rEL,    //控制器軟體版本, 只能讀取
    Ptb,    //參數表代碼, 只能讀取

  End,
};

enum digitis
{
  dig_0 =  0,
  dig_1 =  1,
  dig_2 =  2,
  dig_3 =  3,
  dig_4 =  4,
  dig_5 =  5,
  dig_6 =  6,
  dig_7 =  7,
  dig_8 =  8,
  dig_9 =  9,
  dig_A = 10,
  dig_b = 11,
  dig_C = 12,
  dig_D = 13,
  dig_E = 14,
  dig_F = 15,
  dig_G = 19,
  dig_H = 20,
  dig_K = 21,
  dig_L = 22,
  dig_n = 23,
  dig_o = 24,
  dig_P = 25,
  dig_r = 26,
  dig_t = 27,
  dig_U = 28,
  dig_V = 29,
  dig_y = 30,
  dig_i = 32,
  dig_W = 33,
  dig_J = 34,
  dig_c = 36,
  dig_d = 37,
  dig_S =  5,
  dig_O =  0,
  dig_I =  1,
  dig_u = 38,
  dig_M = 39,

  dig_off = 16,
  dig_on = 17,
  dig_minus = 31,
  dig_underscore = 35,
};


/* task function protocol -----------------------------------------------*/
// void r_tau_1ms_callback(timer_callback_args_t *p_args);
void ShowTable(uint8_t table);
void ShowNumber(int32_t num);
void LED_Display_main(void);

#endif /* INC_DISPLAY_H_ */
