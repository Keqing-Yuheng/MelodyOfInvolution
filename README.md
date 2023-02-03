# Melody Of Involution
A music game with extreme complexity running in the console.  
**韵律漩涡**:一个基于控制台的高难度音游  
**Version Release 1.2.1**  
GitHub @ Keqing-Yuheng  

**简体中文** | [English](https://github.com/Keqing-Yuheng/MelodyOfInvolution/blob/main/README-English.md "README-English.md")  

## 如何游玩
运行**Launcher - Melody Of Involution.exe**启动Melody Of Involution  
然后按程序内提示操作  
建议不要直接运行MelodyOfInvolution.exe以减少闪烁  
### 按键
Melody Of Involution需要用键盘操作，使用键盘左侧3x4区域内的按键，及Shift或Caps Lock键
```
          | Q W E R
Caps Lock | A S D F
Shift     | Z X C V
```
Melody Of Involution是3判定线、4轨道的音游，一行按键对应一条判定线，一列按键对应一个轨道
### 界面
```
Melody Of Involution
Version Release 1.1
 _________________________
| 0 PERFECT / FULL COMBO    //Perfect&Lost计数器
|[  0%]   1/1000            //进度
|_________________________
|   w       |               //Note出现的地方
|           |               //Notes以一定速度下落
|           |               //Notes移动区域
|           |
|           |
|           |
|           |
|           |
|           |
|           |               //根据Note经过判定线的顺序，下面的3条判定线(>所在行)分别记为判定线1/2/3
|           |               //当Note到达相应判定线时按下对应的按键
|           |               //判定线的判定区域是>所在行及其上方1行
>           |               //判定线1 对应键盘第1行按键(Q/W/E/R/q/w/e/r)
|           |
>           |               //判定线2 对应键盘第2行按键(A/S/D/F/a/s/d/f)
|           |
>           |               //判定线3 对应键盘第3行按键(Z/X/C/V/z/x/c/v)
|           |               //Perfect时，Note将消失显示+以标识
__________________________  //Lost时，此行对应轨道出将出现^以警告
```
### 得分说明
满分**1000000**  
得到FULL COMBO则得分为1000000  
```
得分计算公式  
Perfect数 / Note数 * 1000000  
也可以以另一种方式来表示  
number_of_notes_cleared / number_of_all_the_notes * 1000000  
```
鉴于Melody Of Involution难度较高，虽然采用了与其他音游相似的等级名称，但达到某一等级所需的得分大幅降低，详见下表  
```
[0,200000)       F  再接再厉
[200000,350000)  D  余音绕梁
[350000,500000)  C  余音绕梁
[500000,650000)  B  声动梁尘
[650000,800000)  A  声动梁尘
[800000,920000)  S  高遏行云
[920000,1000000) V  高遏行云
{1000000}        EX 大音天籁
```
## 数据文件结构
### Melody Of Involution包含的可执行文件如下
#### Launcher - Melody Of Involution.exe
Melody Of Involution启动器  
建议通过此程序启动游戏  
参数:无  
#### MelodyOfInvolution.exe
Melody Of Involution主程序  
可通过启动器打开  
参数:  
- `-mute` 关闭音频
- `-silent` 关闭音频
#### MOI_AdventureMode.exe
Melody Of Involution冒险模式  
可通过启动器打开  
参数:`MOI_Adventure [int64:score] [string:reg_id]`  
`reg_id`可使用`MOI_Adventure_Record`  
#### MOI_ImgDisplay.exe
Melody Of Involution曲绘查看器  
参数:  
- 参数1 指定显示的位图路径 缺省则为`MOI_Welcome.bmp`
- 参数2 指定窗口标题 缺省则为`Melody Of Involution`
- 参数3 若为`Still`则换用`SetDIBitsToDevice()`输出 若为其他则不变
- 若传入更多参数，将只取前2个参数分别作为位图路径与窗口标题
#### MOI_SpeedUpright.exe
Melody Of Involution自动速率校正工具  
参数:指定用于校正的项目 缺省则为MOI_Test_Project.txt  
### Melody Of Involution程序运行所需的数据文件如下
(在缺失下列文件时运行Melody Of Involution程序组可能导致意料之外的错误)  
#### MOI_Config.ini
Melody Of Involution基本配置文件  
设置窗口颜色，音频开关，曲绘开关，播放器路径  
**第1行**
- 第1位:0-F的16进制数(不区分大小写) 设置启动器背景色
- 第2位:0-F的16进制数(不区分大小写) 设置启动器前景色
- 第3位:0-F的16进制数(不区分大小写) 设置主程序背景色
- 第4位:0-F的16进制数(不区分大小写) 设置主程序前景色
- 第5位:0-F的16进制数(不区分大小写) 设置其他程序背景色
- 第6位:0-F的16进制数(不区分大小写) 设置其他程序前景色
- 第7位:0-关闭音频/1-开启音频
- 第8位:0-关闭曲绘/1-开启曲绘
**第2行**
播放器路径 填`#NULL`表示空  
#### MOI_ExpData.ini
Melody Of Involution - Adventure Mode经验值  
#### MOI_SpeedUpright_Data.ini
Melody Of Involution速率校正文件  
表示运行一个周期用时 以ms为单位  
#### MOI_INDEX_Play.lst
Melody Of Involution关卡索引(尚未完善)  
由若干组组成，在每组中，第1行为关卡简写(如`*M0101`)，第2行为该关卡项目文件相对路径  
### Melody Of Involution项目包含的文件如下
#### 项目文件(*.txt)
记录Melody Of Involution项目基本信息  
**第1行**
- 第1位至第4位:一个周期需要的时长，单位ms，必要时用前导0补足4位(控制下落速率，越小越快，但值过小会超出计算机的极限，无法实现)
- 第5位:分隔符(建议使用`/`)
- 第6位至第11位:谱面文件行数，必要时用前导0补足6位
- 第12位:分隔符(建议使用`/`)
- 第13位至第18位:Note数，必要时用前导0补足6位
- 第19位:分隔符(建议使用`/`)
- 第20位:项目音频开关 0-关 1-开
- 第21位:项目曲绘开关 0-关 1-开
**第2行**
谱面文件路径(*.lrc) 必填  
**第3行**
音频文件路径 若无则该行留空  
**第4行**
曲绘文件路径(*.bmp) 若无则该行留空  
**第4行以下**
任意描述，不会被程序读取，无格式要求  
#### 曲绘文件(*.lrc)
**有效字符**
- 24种Note
- `#` 与空格等效
- 空格
**无效字符**
- `$` 将会被忽略
- `\n`(换行符) 将会被忽略
每读取4个有效字符即作为一行  
按显示界面换行是考虑谱面文件本身可读性  
**注意事项**
- 编写谱面文件时，请考虑谱面难度，以及是否能够完成
- 合理运用有效字符与无效字符，以优化排版与统计
#### 音频文件
音频不由Melody Of Involution播放，故无格式要求，仅需考虑文件音质、体积与兼容性  
#### 曲绘文件
必须是8bit位深度BMP位图  
### Melody Of Involution剧情文件格式如下
- 第1行 标题
- 第2行 观看剧情所需的等级
- 第3行及以下 剧情文本
    > 剧情文本以1行为1个单位  
    > `\`是控制字符  
    > `\!`用于输出图片，其后紧跟图片在窗口中占的行数以留出位置，并在下一行写图片路径  
    > `\:`用于分隔讲述者与讲述内容，将输出`:`并换行  
    > `\|`用于换行  
    > 如遇其他内容，则正常输出  
    > 一行输出结束后，等待按键以开始下一行输出  
## 开发日志
在后面的版本中，将会换用更加直观的音符呈现方式  
预计在1.2或1.3版本换用1/2/3/A/B/C来区分不同判定线和大小写  
预计在2.0版本换用彩色的Note  