# MelodyOfInvolution
A music game with extreme complexity running in the console.一个难度极高的音游(在控制台中运行)
## Attention
As a learner, I cannot do very vell in programming. Thus, the source code of the project might NOT deserve to be read, for it is neither extraordinary nor easy for reading. If you just want to read the code so as to know how the game exactly works, that is OK. However, I still think the method of playing is good(At least it will be better after several updates).<br>
作为一个初学者，我进行开发的能力有限。因此，该项目的源代码并不值得阅读，因为源代码既不包含先进的技术，也不易于阅读。如果阅读源代码的目的只是知道这个程序是怎么运行起来的，那么是可以的。但即使如此，我还是相信这个游戏的玩法是可以的，至少在几个版本后会变好。
## How to Play It
**如何游玩**(For Version 1.0)<br>
### Key
**按键**<br>
This game needs you to operate with your keyboard. The keys of 3x4 area in the left of the keyboard, along with Shift or Caps Lock, will be used.<br>
该游戏需要用键盘操作，需要使用键盘左侧3x4区域内的按键，以及Shift或Caps Lock键。<br>
```
          | Q W E R
Caps Lock | A S D F
Shift     | Z X C V
```
This is beacuse Melody Of Involution is a music game with 3 Judging Lines and 4 Tracks.<br>
Melody Of Involution是3判定线、4轨道的音游<br>
Each row of keys matches each Judging Line, while each line of keys matches each Track.<br>
一行按键对应一条判定线，一列按键对应一个轨道<br>
### Interface
**界面说明**<br>
```
Melody Of Involution
Version Release 1.0
 _________________________
| 0 PERFECT / FULL COMBO    //Perfect&Lost Counter
|[  0%]   1/1000            //Progress
|_________________________
|   w       |               //Where notes appear
|           |               //Notes will drop at a certain speed
|           |               //Notes move in this area
|           |
|           |
|           |
|           |
|           |
|           |
|           |
|           |               //According to the sequence in which notes pass, the following 3 lines are named Judging Line 1, 2 and 3
|           |               //You need to press the right key when a note reach its Judging Line
>           |               //This is Judging Line 1:Letters in the first row of the keyboard  (Q/W/E/R/q/w/e/r)
|           |
>           |               //This is Judging Line 2:Letters in the second row of the keyboard (A/S/D/F/a/s/d/f)
|           |
>           |               //This is Judging Line 3:Letters in the third row of the keyboard  (Z/X/C/V/z/x/c/v)
|           |
__________________________  //If you lose some note, warnings will be displayed here
```
### Score
**得分说明**<br>
The maxium score is 1000000.<br>
满分为1000000<br>
Your score will be calculated by the formula below.<br>
得分将按下面的公式计算<br>
```
number_of_notes_cleared / number_of_all_the_notes) * 1000000
```
If you get FULL COMBO, your score is 1000000.<br>
得到FULL COMBO则得分为1000000<br>
**Scores & Levels**<br>
**得分与等级**<br>
Given that Melody Of Involution is a complex music game, despite the fact that Melody Of Involution uses levels which might have similar names to other music games, the score that a level requires here is much lower. More details are as follows.<br>
鉴于Melody Of Involution难度较高，即使采用了和其他音游相似的等级，在这里达到该等级所需的得分也会低得多，详见下表<br>
```
[0,200000)       F 再接再厉
[200000,350000)  D 余音绕梁
[350000,500000)  C 余音绕梁
[500000,650000)  B 声动梁尘
[650000,800000)  A 声动梁尘
[800000,920000)  S 高遏行云
[920000,1000000) V 高遏行云
{1000000}       EX 大音天籁
```
## 开发日志
在后面的版本中，将会采用更加直观的音符呈现方式<br>
预计在1.1或1.2版本换用1/2/3/A/B/C来区分不同判定线和大小写<br>
预计在2.0版本换用彩色的音符<br>
