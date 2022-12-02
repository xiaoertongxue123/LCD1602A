#include "lcd1602a.h"
#include "delay.h"

/*外设GPIO初始化*/
void GPIOInit(void){
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE,ENABLE);
	
	/*PE7~14用于接DB0~DB7*/
	GPIO_InitStructure.GPIO_Pin			= GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode		=	GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType		=	GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_PuPd		=	GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed		=	GPIO_High_Speed;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/*PD9、PD10接A和Vdd;PD14--V0;PD15--RS;PD0--RW;PD1--E*/
	GPIO_InitStructure.GPIO_Pin			= GPIO_Pin_9 | GPIO_Pin_10| GPIO_Pin_14| GPIO_Pin_15| GPIO_Pin_0| GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode		=	GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType		=	GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_PuPd		=	GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed		=	GPIO_High_Speed;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

}

/*向LCD中写入信息（指令/数据）*/
void write(uint16_t data){
/**       
					通过操作GPIO端口引脚的方式写入
	GPIO_WriteBit(GPIOE, GPIO_Pin_14,(BitAction)((data & 0x80) >> 7 ));
	GPIO_WriteBit(GPIOE, GPIO_Pin_13,(BitAction)((data & 0x40) >> 6 ));
	GPIO_WriteBit(GPIOE, GPIO_Pin_12,(BitAction)((data & 0x20) >> 5 ));
	GPIO_WriteBit(GPIOE, GPIO_Pin_11,(BitAction)((data & 0x10) >> 4 ));
	GPIO_WriteBit(GPIOE, GPIO_Pin_10,(BitAction)((data & 0x08) >> 3 ));
	GPIO_WriteBit(GPIOE, GPIO_Pin_9,(BitAction)((data & 0x04) >> 2 ));
	GPIO_WriteBit(GPIOE, GPIO_Pin_8,(BitAction)((data & 0x02) >> 1 ));
	GPIO_WriteBit(GPIOE, GPIO_Pin_7,(BitAction)(data & 0x01));	
	**/

	
	GPIO_Write(GPIOE,((data << 7) & 0x7F80));
	delay_ms(10);
	

}


/*写入指令*/
void writeIns(uint8_t ins){
	
	RS = 0;
	RW = 0;
	E = 0;
	
	write(ins);
	
	E = 1;
	delay_ms(5);
	E = 0;	
}

/*写入数据*/
void writeData(uint8_t data){
	
	RS = 1;
	RW = 0;
	E = 0;
	
	write(data);
	
	E = 1;
	delay_ms(5);
	E = 0;	
}

/*清屏*/
void clearScreen(void){
	writeIns(0x01);
}

/*光标位置*/
void cursor(uint8_t position,uint8_t flag){
	uint8_t addr;
	if(flag == 0){
		addr = 0x00 + position;
	}else{
		addr = 0x40 + position;
	}
	
	writeIns(addr | 0x80);
	
}

/*显示字符串*/
void showStr(uint8_t position,uint8_t flag,uint8_t *str){
	cursor(position,flag);
	while(*str != '\0'){
		writeData(*str ++);
	}
}

/*滚动字符*/
void moveStr(uint8_t position,uint8_t flag,uint8_t *str){
	
	writeIns(0x1c);
	cursor(position,flag);
	while(*str != '\0'){
		writeData(*str ++);
	}
}

/*LCD1602初始化*/
void LCD_Init(void){

	GPIOInit();
  A = 1;
	Vdd = 1;
	V0 = 1;
	
	writeIns(0x38);//
	delay_ms(5);
	writeIns(0x0c);//开显示，光标关闭
	delay_ms(5);
	writeIns(0x06);
	delay_ms(5);
	writeIns(0x01);
	delay_ms(5);
}
