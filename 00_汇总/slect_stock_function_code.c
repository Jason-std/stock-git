{
  选股条件：沿股票沿箱体缓慢向上运行
  1. 价格在箱体内波动（振幅控制在一定范围）
  2. 整体呈现缓慢上行趋势
  3. 上涨斜率较为平缓（小于45度）
}

{定义箱体周期（可调整，此处为30个交易日）} 
BOX_DAYS := 60;

{计算箱体区间} 
BOX_HIGH := HHV(HIGH, BOX_DAYS);  { 箱体最高价}
BOX_LOW := LLV(LOW, BOX_DAYS);    {箱体最低价} 
BOX_RANGE := BOX_HIGH - BOX_LOW;  {箱体区间幅度} 

{条件1：箱体幅度适中（不超过12%，避免宽幅震荡）} 
COND1 := BOX_RANGE / BOX_LOW < 0.12;

{条件2：价格在箱体内运行（波动幅度小）} 
COND2 := EVERY(HIGH < BOX_HIGH*1.02 AND LOW > BOX_LOW*0.98, BOX_DAYS);

{条件3：整体呈上行趋势（收盘价逐步抬高）} 
MA_SHORT := MA(CLOSE, 5);   { 短期均线}
MA_LONG := MA(CLOSE, 60);   { 长期均线}
COND3 := MA_SHORT > MA_LONG AND EVERY(MA_LONG > REF(MA_LONG,1), 10);

{条件4：缓慢上行，斜率小于45度（涨幅温和）} 
PRICE_UP := CLOSE - REF(CLOSE, BOX_DAYS);  {箱体期间价格涨幅} 
COND4 := PRICE_UP > 0 AND PRICE_UP / BOX_DAYS < 0.015;  {日均涨幅小于1.5%} 

{条件5：90%筹码集中度<10%} 
COST_CON := (COST(95) - COST(5)) / (COST(95) + COST(5)) * 100;
COND5 := COST_CON < 10;

{条件6：最近3日成交量小于箱体期间平均成交量} 
BOX_AVG_VOL := REF(MA(VOL, BOX_DAYS), 4);  {箱体期间平均成交量} 
COND6 := EVERY(VOL < BOX_AVG_VOL, 3);

{ 条件7：最近3日最低价在60日均线附近 }
MA60 := MA(C, 60);
COND7 := 
    ABS(L - MA60)/MA60 <= 0.015 AND    { 今日最低价在±1.5%内 }
    ABS(REF(L,1) - MA60)/MA60 <= 0.015 AND { 昨日最低价 }
    ABS(REF(L,2) - MA60)/MA60 <= 0.015;    { 前日最低价 }	

{综合选股条件} 
SELECT:COND1 AND COND2 AND COND3 AND COND4 AND COND5 AND COND6 AND COND7;












一、箱体挖坑V2（箱体向上运行突然下跌）

请写一段通达信选股代码，选出符合以下条件的股票：
1、最近3个交易日的最低价都在60日均线附近。
1、4到30个交易日之前，都延着箱体和缓慢上行。
3、最近3个交易日的成交量都箱体运行时（4-30个交易日）的平均成交量小。
4、90%筹码集中度小于10%。
5、注释用中文，代码变量用英文字母表示，长度小于15个字符。



MA60 := MA(CLOSE, 60); {60日均线}  
BOX_DAYS := 27;  {箱体天数(30-4+1)} 

{条件1：最近3日最低价在60日均线附近(±1.5%)} 
COND1 := EVERY(LOW > MA60*0.985 AND LOW < MA60*1.015, 3);

{箱体区间定义(4到30个交易日之前)} 
BOX_HIGH := REF(HHV(HIGH, BOX_DAYS), 4);  {箱体最高价} 
BOX_LOW := REF(LLV(LOW, BOX_DAYS), 4);    {箱体最低价} 
BOX_MID := (BOX_HIGH + BOX_LOW) / 2;      {箱体中值} 

{条件2.1：处于箱体内运行} 
COND2_1 := REF(EVERY(HIGH < BOX_HIGH*1.03 AND LOW > BOX_LOW*0.97, BOX_DAYS), 4);

{条件2.2：缓慢上行，斜率<45度(涨幅小于天数的1/2)} 
PRICE_CHG := REF(CLOSE, 4) - REF(CLOSE, 30);
COND2_2 := PRICE_CHG > 0 AND PRICE_CHG / BOX_DAYS < 0.5;
COND2 := COND2_1 AND COND2_2;

{条件3：最近3日成交量小于箱体期间平均成交量} 
BOX_AVG_VOL := REF(MA(VOL, BOX_DAYS), 4);  {箱体期间平均成交量} 
COND3 := EVERY(VOL < BOX_AVG_VOL, 3);

{条件4：90%筹码集中度<10%} 
COST_CON := (COST(95) - COST(5)) / (COST(95) + COST(5)) * 100;
CON4 := COST_CON < 10;

{ 综合选股条件}
选股:COND1 AND COND2 AND COND3 AND COND4;



二、横盘挖坑
写一段通达信选股公式，选出符合以下条件的个股：
1、长期横盘，横盘交易日大于30天。
2、最近3个交易日的最低价都在60日均线附近。
3、最近3个交易日的成交量都比横盘时的平均成交量小。
4、90%筹码集中度小于12%。


==============================V1 英文版===================
{ TDX FORMULA: CONSOLIDATION PULLBACK }

DAYS := 60;     { CONSOLIDATION DAYS }
AMPT := 15;     { AMPLITUDE THRESHOLD %}

{ CONDITION 1: CONSOLIDATION }
H60 := HHV(H, DAYS);
L60 := LLV(L, DAYS);
RNG := (H60 - L60) / L60 * 100;
CON1 := RNG < AMPT AND BARSCOUNT(C) >= DAYS;

{ AVG VOLUME DURING CONSOLIDATION }
V_AVG := MA(V, DAYS);

{ CONDITION 2: LOW NEAR MA60 }
{ 条件2：最近3日最低价在60日均线附近 }
MA60 := MA(C, 60);
NEAR_MA60 := 
    ABS(L - MA60)/MA60 <= 0.015 AND    { 今日最低价在±1.5%内 }
    ABS(REF(L,1) - MA60)/MA60 <= 0.015 AND { 昨日最低价 }
    ABS(REF(L,2) - MA60)/MA60 <= 0.015;    { 前日最低价 }	

{ CONDITION 3: VOLUME BELOW AVG }
CON3 := 
    V < V_AVG AND 
    REF(V,1) < V_AVG AND 
    REF(V,2) < V_AVG;

{ CONDITION 4: COST CONCENTRATION }
COST_CON := (COST(95) - COST(5)) / (COST(95) + COST(5)) * 100;
CON4 := COST_CON < 10;

{ FINAL SELECTION }
SELECT: CON1 AND CON2 AND CON3 AND CON4;


==============================V2 中文版===================

写一段通达信选股公式，选出符合以下条件的个股：
1、长期横盘，横盘交易日大于30天。
2、最近3个交易日的最低价都在60日均线附近。
3、最近3个交易日的成交量都比横盘时的平均成交量小。
4、90%筹码集中度小于12%。

{ 通达信选股公式：横盘回调缩量选股（最低价+成交量比较版） }

横盘天数 := 60; { 定义横盘观察周期，建议60天（大于30天） }
振幅阈值 := 10; { 定义横盘最大振幅百分比 }

{ 条件1：长期横盘（大于30天） }
横盘高点 := HHV(H, 横盘天数);
横盘低点 := LLV(L, 横盘天数);
横盘幅度 := (横盘高点 - 横盘低点) / 横盘低点 * 100;
横盘条件 := 横盘幅度 < 振幅阈值 AND BARSCOUNT(C) >= 横盘天数;

{ 计算横盘期平均成交量 }
横盘期均量 := MA(V, 横盘天数);

{ 条件2：最近3日最低价在60日均线附近 }
MA60 := MA(C, 60);
最低价附近条件 := 
    ABS(L - MA60)/MA60 <= 0.015 AND    { 今日最低价在±1.5%内 }
    ABS(REF(L,1) - MA60)/MA60 <= 0.015 AND { 昨日最低价 }
    ABS(REF(L,2) - MA60)/MA60 <= 0.015;    { 前日最低价 }

{ 条件3：最近3日成交量都小于横盘期平均量 }
缩量条件 := 
    V < 横盘期均量 AND 
    REF(V,1) < 横盘期均量 AND 
    REF(V,2) < 横盘期均量;

{ 条件4：90%筹码集中度小于12% }
集中度 := (COST(95) - COST(5)) / (COST(95) + COST(5)) * 100;
筹码条件 := 集中度 < 12;

{ 综合选股条件 }
选股条件 := 横盘条件 AND 最低价附近条件 AND 缩量条件 AND 筹码条件;

选股: 选股条件;