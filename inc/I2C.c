#include <reg52.h>
#include <Delay.h>

__sbit __at(0x90 + 0) I2C_SDA;
__sbit __at(0x90 + 1) I2C_SCL;

/**
  * @brief I2C开始
  * @param 无
  * @retval 无
  */
void I2C_Start(void)
{
	I2C_SDA = 1;
	I2C_SCL = 1;
	I2C_SDA = 0;
	I2C_SCL = 0;
}
/**
  * @brief I2C结束
  * @param 无
  * @retval 无
  */
void I2C_Stop(void)
{
	I2C_SCL = 0;
	I2C_SDA = 0;
	I2C_SCL = 1;
	I2C_SDA = 1;
}
/**
  * @brief I2C发送一个字节
  * @param DATA 要发送的字节
  * @retval 无
  */
void I2C_SendByte(unsigned char DATA)
{
	unsigned char i;
	for (i = 0; i < 8; i++)
	{
		I2C_SDA = DATA & (0x80 >> i);
		I2C_SCL = 1;
		I2C_SCL = 0;
	}
}
/**
  * @brief I2C接收一个字节
  * @param 无
  * @retval 接收到的字节
  */
unsigned char I2C_ReceiveByte(void)
{
	unsigned char DATA = 0x00;
	unsigned char i;
	I2C_SDA = 1;
	for (i = 0; i < 8; i++)
	{
		I2C_SCL = 1;
		if (I2C_SDA)
		{
			DATA |= (0x80 >> i);
		}
		I2C_SCL = 0;
	}
	return DATA;
}
/**
  * @brief I2C发送应答
  * @param AckBit 应答位, 0为应答, 1为非应答
  * @retval 无
  */
void I2C_SendAck(unsigned char AckBit)
{
	I2C_SDA = AckBit;
	I2C_SCL = 1;
	I2C_SCL = 0;
}
/**
  * @brief I2C接收应答
  * @param 无
  * @retval 应答位, 0为应答, 1为非应答
  */
unsigned char I2C_ReceiveAck(void)
{
	I2C_SDA = 1;
	I2C_SCL = 1;
	if (I2C_SDA)
	{
		return 1;
	}
	I2C_SCL = 0;
	return 0;
}

/**
  * @brief I2C向对应设备对应位置写入1个字节
  * @param ADD 设备地址
  * @param WADD 数据地址
  * @param DATA 写入的数据
  * @retval 无
  */
void I2C_E_W(unsigned char ADD, unsigned char WADD, unsigned char DATA)
{
	I2C_Start();
	I2C_SendByte(ADD);
	I2C_ReceiveAck();
	I2C_SendByte(WADD);
	I2C_ReceiveAck();
	I2C_SendByte(DATA);
	I2C_ReceiveAck();
	I2C_Stop();
	Delay(5);
}

/**
  * @brief I2C从对应设备对应位置读取1个字节
  * @param ADD 设备地址
  * @param WADD 数据地址
  * @retval 读取到的数据
  */
unsigned char I2C_E_R(unsigned char ADD, unsigned char WADD)
{
	unsigned char DATA;
	I2C_Start();
	I2C_SendByte(ADD);
	I2C_ReceiveAck();
	I2C_SendByte(WADD);
	I2C_ReceiveAck();
	I2C_Start();
	I2C_SendByte(ADD | 0x01);
	I2C_ReceiveAck();
	DATA = I2C_ReceiveByte();
	I2C_SendAck(0x01);
	I2C_Stop();
	return DATA;
}