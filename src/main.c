#include <reg52.h>     //51头文件
#include <I2C.h>       //I2C协议
#include <PAJ7620.h>   //PAJ7620模块初始化
#include <Delay.h>     //延时模块
#include <MatrixLED.h> //8*8点阵显示模块
#include <Nixie.h>     //晶体管显示

#define MAX_LEN 40 //蛇身最大长度
#define INIT_LEN 2 //蛇身初始长度
#define SECOND 150 //蛇移动延时(推荐设置150,数字越大越慢)

/*
变量定义区
*/
unsigned char Map_Data[8];                                  //地图数据
unsigned char Key_Num, Key_Temp, Snake_Dir, Food_Need_Flag; //此时按键键值 键值缓存 蛇行动方向 食物需要标志
struct FOOD                                                 //食物结构体
{
    unsigned char x; //坐标
    unsigned char y; //坐标
} Food;
struct SNAKE //蛇结构体
{
    unsigned char x[MAX_LEN]; //蛇身坐标数组
    unsigned char y[MAX_LEN]; //蛇身坐标数组
    unsigned char Len_Now;    //蛇身长度
} Snake;

/*
函数声明区
*/
void Game_End(void);
void Show_Map(void);
void Food_Init(void);
void Snake_Init(void);
void Timer1_Init(void);
void Timer0_Init(void);
void Snake_Move(void);
void Map_Init(void);
void Snake_Move(void);
unsigned char Snake_Check(void);
void Data_Write(unsigned char x, unsigned char y, unsigned char DATA);
/*
主函数
*/
void main(void)
{
    PAJ7620_Init();
    Map_Init();    //地图初始化
    Snake_Init();  //蛇身初始化
    Timer0_Init(); //定时器0初始化
    Timer1_Init(); //定时器0初始化
    Food_Init();   //食物初始化
    while (1)
    {
        if (Snake_Check()) //蛇状态检测
        {                  //通过
            Snake_Move();  //蛇移动
        }
        else              //游戏重新开始
        {                 //未通过
            Game_End();   //游戏结束
            Map_Init();   //地图初始化
            Snake_Init(); //蛇身初始化
            Food_Init();  //食物初始化
        }
    }
}

/*
其他函数
*/
void Map_Init(void) //地图初始化/重置
{
    Map_Data[0] = 0x00;
    Map_Data[1] = 0x00;
    Map_Data[2] = 0x00;
    Map_Data[3] = 0x00;
    Map_Data[4] = 0x00;
    Map_Data[5] = 0x00;
    Map_Data[6] = 0x00;
    Map_Data[7] = 0x00;
}

void Snake_Init(void) //蛇身初始化
{
    for (unsigned char i = 0; i < MAX_LEN; i++) //蛇身使用unsigned char类型最大数255填充
    {
        Snake.x[i] = 255;
        Snake.y[i] = 255;
    }
    Snake.Len_Now = INIT_LEN; //蛇身长度设置为初始长度
    Snake.x[0] = 0;           //初始化蛇头
    Snake.y[0] = 1;
    Data_Write(0, 1, 1);                         //蛇头数据写入
    for (unsigned char i = 1; i < INIT_LEN; i++) //初始化蛇身
    {
        Snake.x[i] = 0;
        Snake.y[i] = 0;
    }
    Data_Write(0, 0, 1); //蛇身数据写入
    Snake_Dir = 1;       //方向初始化
    Key_Num = 1;         //PAJ7620读取值初始化
}

void Snake_Move(void) //蛇移动
{
    Delay(SECOND);                                                           //移动间隔
    Data_Write(Snake.x[Snake.Len_Now - 1], Snake.y[Snake.Len_Now - 1], 0); //在蛇尾对应位写入0删除蛇尾
    for (unsigned char i = Snake.Len_Now - 1; i > 0; i--)                  //将蛇身坐标顺位向后移
    {
        Snake.x[i] = Snake.x[i - 1];
        Snake.y[i] = Snake.y[i - 1];
    }
    if (Key_Num != 0) //检测此时方向值
    {
        if(Key_Num == 1 && Snake_Dir != 2)
        {
            Snake_Dir = Key_Num;
        }
        else if(Key_Num == 2 && Snake_Dir != 1)
        {
            Snake_Dir = Key_Num;
        }
        else if(Key_Num == 3 && Snake_Dir != 4)
        {
            Snake_Dir = Key_Num;
        }
        else if(Key_Num == 4 && Snake_Dir != 3)
        {
            Snake_Dir = Key_Num;
        }
    }
    switch (Snake_Dir) //依据方向判断方向并移动蛇头
    {
    case 1:
        Snake.y[0]++;
        break;
    case 3:
        Snake.x[0]--;
        break;
    case 4:
        Snake.x[0]++;
        break;
    case 2:
        Snake.y[0]--;
        break;
    default:
        break;
    }
    Data_Write(Snake.x[0], Snake.y[0], 1);            //在蛇头位置写入1显示蛇头
    if (Snake.x[0] == Food.x && Snake.y[0] == Food.y) //若吃到食物 身长 + 1 将食物需求位置1
    {
        Snake.Len_Now++;
        Food_Need_Flag = 1;
    }
}

unsigned char Snake_Check(void) //蛇状态检测
{

    if (Snake.x[0] > 7 || Snake.y[0] > 7 || Snake.Len_Now > MAX_LEN) //若出图  蛇长过长 游戏结束
    {
        return 0;
    }
    for (unsigned char i = 1; i < Snake.Len_Now; i++) //若蛇吃自身 游戏结束
    {
        if (Snake.x[0] == Snake.x[i] && Snake.y[0] == Snake.y[i])
        {
            return 0;
        }
    }
    return 1; //游戏正常进行
}

void Show_Map(void) //点阵显示地图
{
    for (unsigned char i = 0; i < 8; i++)
    {
        MatrixLED_Show_Column(i, Map_Data[i]);
    }
}

void Data_Write(unsigned char x, unsigned char y, unsigned char DATA) //地图数据写入
{
    if (DATA) //若写入的数据位1
    {
        Map_Data[x] |= 0x01 << y; //对应位或运算
    }
    else //若写入的数据位0
    {
        Map_Data[x] &= ~(0x01 << y); //对应位取反与运算
    }
}

void Food_Creat(void) //食物生成
{
    unsigned char Creat_Flag; //生成成功标志位
    do
    {
        Creat_Flag = 1;                                   //标志位置1
        Food.x = TL0 % 7;                                 //利用定时器伪随机
        Food.y = TL1 % 7;                                 //利用定时器伪随机
        for (unsigned char i = 0; i < Snake.Len_Now; i++) //检测是否有蛇身遮挡
        {
            if (Food.x == Snake.x[i] && Food.y == Snake.y[i])
            {
                Creat_Flag = 0;
            }
        }
        if (Creat_Flag) //生成成功则在对应位写1
        {
            Data_Write(Food.x, Food.y, 1);
        }
    } while (!Creat_Flag);
    Food_Need_Flag = 0; //需求标志位置0
}

void Food_Init(void) //食物初始化 默认位于7,7位置
{
    Food_Need_Flag = 0;
    Food.x = 7;
    Food.y = 7;
    Data_Write(7, 7, 1);
}

void Timer0_Init(void) //定时器0初始化 10毫秒@11.0592MHz
{
    TMOD &= 0xF0;
    TMOD |= 0x01;
    TL0 = 0x00;
    TH0 = 0xDC;
    TF0 = 0;
    TR0 = 1;
    ET0 = 1;
    EA = 1;
    PT0 = 0;
}

void Timer0_Interrupt(void) __interrupt 1 //定时扫描PAJ7620识别结果寄存器
{
    TL0 = 0x00;
    TH0 = 0xDC;
    Key_Temp = PAJ7620_GetDir();
    if (Key_Temp == 1 || Key_Temp == 2 || Key_Temp == 3 || Key_Temp == 4)
    {
        Key_Num = Key_Temp;
    }
}

void Timer1_Init(void) //定时器1初始化 20毫秒@11.0592MHz
{
    TMOD &= 0x0F;
    TMOD |= 0x10;
    TL1 = 0x00;
    TH1 = 0xB8;
    TF1 = 0;
    TR1 = 1;
    ET1 = 1;
    EA = 1;
    PT1 = 0;
}

void Timer1_Interrupt(void) __interrupt 3 //定时刷新地图显示 检测食物是否需要生成
{
    TL1 = 0x00;
    TH1 = 0xB8;
    Show_Map();
    if (Food_Need_Flag)
    {
        Food_Creat();
    }
}

void Game_End(void)
{
    EA = 0; //中断关闭
    unsigned char i = 255;
    while (i--) //晶体管显示蛇长,即显示成绩
    {
        Nixie_ShowNum(7, Snake.Len_Now / 10);
        Nixie_ShowNum(8, Snake.Len_Now % 10);
    }
    Delay(1000); //延时1s
    EA = 1;      //中断开始
}