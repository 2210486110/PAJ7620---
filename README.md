# 代码使用手册
**编译注意事项**  
此代码使用**SDCC编译**,想使用其他编译器需要更改部分代码  
VSCODE写51是真的舒服  
  
**引脚设置**  
PAJ7620的SCL,SDA分别接P11,P10,Int不接任何引脚 (你想接上的话,我也拦不住你)  
VCC与GND怎么接看你心情  
其余引脚设置在头文件中都有声明,可按照需求修改引脚  

**其他**  
PAJ7620的所有功能并没有写在头文件中  
只有最基本的手势判断函数  
(主要是还用不到就没写)  
但是常用寄存器位置已经宏定义好了  
想用的话直接使用就可  

**实际效果**  
通电即开始  
将PAJ7620摆放正确后直接使用手势操控  
死亡时晶体管会显示蛇的长度,即得分  
约1s后自动开始新一轮游戏  
有闲心的话自己加上手势控制游戏开始
  
**已知BUG**   
蛇死亡后点阵屏有概率乱闪  
51空间有限,蛇长最大值为40  
手势识别精确度不高  
若蛇的长度为2时,蛇可以反向移动 (与当前方向相反,且不会死亡)  **已修复**  


**小问题**  
60帧拍摄下,点阵会乱闪,肉眼观察显示正常  