/**
 ******************************************************************************
 * @file    datapool.h
 * @author  KCE R&D Team
 * @date    2024-08-29
 * @author  Ruby
 *
 * @copyright Copyright © 2024 KCE. All rights reserved.
 ******************************************************************************
 */


#define INC_DATAPOOL_H_ 
#ifdef INC_DATAPOOL_H_

/* Enum type definitions -----------------------------------------------------*/
enum UserDefineByteTables
{
  Str,    // Start, 0:New_eeprom, 1:No_problem, 2:Reset_eeprom
  Set,      //設定點, 範圍LS~US, 非設定層
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
    tdF,    //融霜類型: EL=電熱(融霜時壓縮機關閉), in=熱氣(融霜時壓縮機運轉)
    dFP,    //融霜終止檢測探頭選擇, nP=無探頭, P1~P4
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
    
  //非使用者可調整參數
    RecordHigh,
    RecordLow,

    var_end,

  //唯讀參數
    dp1,    //探頭1讀數, 庫溫
    dp2,    //探頭2讀數, 蒸發器溫度
    dp3,    //探頭3讀數, 輔助探頭, 有探頭才可用
    dp4,    //探頭4讀數, 輔助探頭
    rSE,    //正常運行時的實際設定點, 只能讀取
    rEL,    //控制器軟體版本, 只能讀取
    Ptb,    //參數表代碼, 只能讀取
    pr2,    //進入隱藏層時顯示的字符, 只能顯示

  End,
};

enum Resetflag
{
  New_eeprom,
  No_problem,
  Reset_eeprom,
};

enum Levelflag
{
  Vindex,
  Vvalue,
  Pr2_symbol,
};

enum MenuLevel
{
  NaN,    //not a number
  Pr1,    //第一層可以看到的參數
  Pr2,    //第二層中才能看到的參數, 進入第二層可看到所有參數
};

enum ProbeExist
{
  No_exist,
  Yes_exist,
};

enum DegreeUnit
{
  degree_C,
  degree_F,
};

enum NumberType
{
  NaNt,    //not a number too
  dE,
  in,
};

enum Paneldisplayselection
{
  disp_P1,
  disp_P2,
  disp_P3,
  disp_P4,
  disp_SEt,
  disp_dtr,
};

enum sensorchoose
{
  defrost_np,
  defrost_p1,
  defrost_p2,
  defrost_p3,
  defrost_p4,
};

enum defrosttype
{
  type_EL,
  type_in,
};

enum defrostshow
{
  defrost_rt,
  defrost_it,
  defrost_SET,
  defrost_dEF,
};

enum defrostwork
{
  work_no,
  work_yes,
};

enum fanwork
{
  fan_c_n,
  fan_o_n,
  fan_c_y,
  fan_o_y,
};

enum alarmset
{
  alarm_rE,
  alarm_Ab,
};

enum polartype
{
  polar_CL,
  polar_OP,
};

enum alarmdelay
{
  alarm_EAL,
  alarm_bAL,
  alarm_PAL,
  alarm_dor,
  alarm_dEF,
  alarm_AUS,
  alarm_Htr,
  alarm_FAn,
  alarm_ES,
};

enum outstate
{
  state_no,
  state_Fan,
  state_CPr,
  state_F_C,
};

enum sensortype
{
  type_Ptc,
  type_ntc,
};

enum standbybutton
{
  button_nu,
  button_oFF,
  button_ES,
};

/* typedef definitions -----------------------------------------------------*/
typedef struct
{
  uint8_t UserByte;
  float Range_Low;
  float Range_High;
  float DefaultValue;
  uint8_t Mode;
  uint8_t DataDigit;
} ByteSettingTable;

typedef struct
{
  int16_t Range_Low;
  int16_t Range_High;
  int16_t DefaultValue;
} SYSTEM_TABLE;

/* task function protocol -----------------------------------------------*/

void UserTabletoSytem(void);
void offset_EEtoSYS(void);
void get_Pv(void);
void get_HistoryMax(void);
void get_HistoryMin(void);
uint8_t get_bytetable_pr1(void);
uint8_t get_bytetable_pr2(void);
int16_t check_Limit_Value(int16_t data, int8_t index);
int16_t celsius_to_fahrenheit(int16_t pv);
// void upload_syscfg_data(int8_t pr_index);

#endif /* INC_DATAPOOL_H_ */