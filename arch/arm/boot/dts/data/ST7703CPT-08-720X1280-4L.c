/***********************************************************************
** Created By:        
** Created Date:        
** Version:             
** Descriptions:        
** 
************************************************************************/
#include "main.h" // 当拷贝到Keil工程中，请注释掉此行
void Initial_LCM()
{
	Reset(0x00);  //复位
	Delay(0x30);
	Reset(0x01);
	Delay(0x30);



	
SPI_WriteCmd(0xb7);
SPI_WriteData(0x50);//  LP mode(DCS mode & HS clk disable) 
SPI_WriteData(0x00);    


SPI_WriteCmd(0xb8);
SPI_WriteData(0x00);
SPI_WriteData(0x00);   //VC(Virtual ChannelID) Control Register

SPI_WriteCmd(0xb9);
SPI_WriteData(0x00);//1=PLL disable
SPI_WriteData(0x00);
Delay(10);
//TX_CLK/MS should be between 5Mhz to100Mhz
SPI_WriteCmd(0xBA);
SPI_WriteData(0x14);
SPI_WriteData(0x42);
Delay(10);
SPI_WriteCmd(0xBB);
SPI_WriteData(0x03);
SPI_WriteData(0x00);
Delay(10);
SPI_WriteCmd(0xb9);
SPI_WriteData(0x01);//1=PLL disable
SPI_WriteData(0x00);
Delay(100);

SPI_WriteCmd(0xDE);
SPI_WriteData(0x03);
SPI_WriteData(0x00);  
Delay(10);

SPI_WriteCmd(0xc9);
SPI_WriteData(0x02);
SPI_WriteData(0x23);
Delay(100);

//////////////////Initial  CODE///////////////////////
     


SSD2828_WritePackageSize(2);
SPI_WriteData(0x11);
SPI_WriteData(0); ////Sleep Out
Delay(250);

SSD2828_WritePackageSize(4);
SPI_WriteData(0xB9);
SPI_WriteData(0xF1);
SPI_WriteData(0x12);
SPI_WriteData(0x83); /// Set EXTC
  
SSD2828_WritePackageSize(28);
SPI_WriteData(0xBA);
SPI_WriteData(0x33);
SPI_WriteData(0x81);
SPI_WriteData(0x05);
SPI_WriteData(0xF9);
SPI_WriteData(0x0E);
SPI_WriteData(0x0E);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x44);
SPI_WriteData(0x25);
SPI_WriteData(0x00);
SPI_WriteData(0x91);
SPI_WriteData(0x0A);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x02);
SPI_WriteData(0x4F);
SPI_WriteData(0x01);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x37);

SSD2828_WritePackageSize(2);
SPI_WriteData(0xB8);
SPI_WriteData(0x25);

SSD2828_WritePackageSize(4);
SPI_WriteData(0xBF);
SPI_WriteData(0x02);
SPI_WriteData(0x11);
SPI_WriteData(0x00); ///Set PCR


SSD2828_WritePackageSize(11);
SPI_WriteData(0xB3);
SPI_WriteData(0x0C);
SPI_WriteData(0x10);
SPI_WriteData(0x0A);
SPI_WriteData(0x50);
SPI_WriteData(0x03);
SPI_WriteData(0xFF);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);

SSD2828_WritePackageSize(12);
SPI_WriteData(0xC0);
SPI_WriteData(0x73);
SPI_WriteData(0x73);
SPI_WriteData(0x50);
SPI_WriteData(0x50);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x08);
SPI_WriteData(0x70);
SPI_WriteData(0x00);/// Set SCR

SSD2828_WritePackageSize(2);
SPI_WriteData(0xBC);
SPI_WriteData(0x46);  /// Set VDC

SSD2828_WritePackageSize(2);
SPI_WriteData(0xCC);
SPI_WriteData(0x0B); /// Set Panel

SSD2828_WritePackageSize(2);
SPI_WriteData(0xB4);
SPI_WriteData(0x80); /// Set Panel Inversion

SSD2828_WritePackageSize(4);
SPI_WriteData(0xB2);
SPI_WriteData(0xC8);
SPI_WriteData(0x12);
SPI_WriteData(0x30); /// Set RSO

SSD2828_WritePackageSize(15);
SPI_WriteData(0xE3);
SPI_WriteData(0x07);
SPI_WriteData(0x07);
SPI_WriteData(0x0B);
SPI_WriteData(0x0B);
SPI_WriteData(0x03);
SPI_WriteData(0x0B);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0xFF);
SPI_WriteData(0x00);
SPI_WriteData(0xC0);
SPI_WriteData(0x10); /// Set EQ

SSD2828_WritePackageSize(13);
SPI_WriteData(0xC1);
SPI_WriteData(0x53);
SPI_WriteData(0x00);
SPI_WriteData(0x1E);
SPI_WriteData(0x1E);
SPI_WriteData(0x77);
SPI_WriteData(0xE1);
SPI_WriteData(0xCC);
SPI_WriteData(0xDD);
SPI_WriteData(0x67);
SPI_WriteData(0x77);
SPI_WriteData(0x33);
SPI_WriteData(0x33);/// Set POWER //1.0_160824

SSD2828_WritePackageSize(7);
SPI_WriteData(0xC6);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0xFF);
SPI_WriteData(0xFF);
SPI_WriteData(0x01);
SPI_WriteData(0xFF);  // ECO

SSD2828_WritePackageSize(3);
SPI_WriteData(0xB5);
SPI_WriteData(0x09);
SPI_WriteData(0x09);  /// Set BGP

SSD2828_WritePackageSize(3);
SPI_WriteData(0xB6);
SPI_WriteData(0x87);
SPI_WriteData(0x95);  /// Set VCOM

SSD2828_WritePackageSize(64);
SPI_WriteData(0xE9);
SPI_WriteData(0xC2);
SPI_WriteData(0x10);
SPI_WriteData(0x05);
SPI_WriteData(0x05);
SPI_WriteData(0x01);
SPI_WriteData(0x05);
SPI_WriteData(0xA0);
SPI_WriteData(0x12);
SPI_WriteData(0x31);
SPI_WriteData(0x23);
SPI_WriteData(0x3F);
SPI_WriteData(0x81);
SPI_WriteData(0x0A);
SPI_WriteData(0xA0);
SPI_WriteData(0x37);
SPI_WriteData(0x18);
SPI_WriteData(0x00);
SPI_WriteData(0x80);
SPI_WriteData(0x01);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x80);
SPI_WriteData(0x01);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x48);
SPI_WriteData(0xF8);
SPI_WriteData(0x86);
SPI_WriteData(0x42);
SPI_WriteData(0x08);
SPI_WriteData(0x88);
SPI_WriteData(0x88);
SPI_WriteData(0x80);
SPI_WriteData(0x88);
SPI_WriteData(0x88);
SPI_WriteData(0x88);
SPI_WriteData(0x58);
SPI_WriteData(0xF8);
SPI_WriteData(0x87);
SPI_WriteData(0x53);
SPI_WriteData(0x18);
SPI_WriteData(0x88);
SPI_WriteData(0x88);
SPI_WriteData(0x81);
SPI_WriteData(0x88);
SPI_WriteData(0x88);
SPI_WriteData(0x88);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x01);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);  /// Set GIP

SSD2828_WritePackageSize(62);
SPI_WriteData(0xEA);
SPI_WriteData(0x00);
SPI_WriteData(0x1A);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x02);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x1F);
SPI_WriteData(0x88);
SPI_WriteData(0x81);
SPI_WriteData(0x35);
SPI_WriteData(0x78);
SPI_WriteData(0x88);
SPI_WriteData(0x88);
SPI_WriteData(0x85);
SPI_WriteData(0x88);
SPI_WriteData(0x88);
SPI_WriteData(0x88);
SPI_WriteData(0x0F);
SPI_WriteData(0x88);
SPI_WriteData(0x80);
SPI_WriteData(0x24);
SPI_WriteData(0x68);
SPI_WriteData(0x88);
SPI_WriteData(0x88);
SPI_WriteData(0x84);
SPI_WriteData(0x88);
SPI_WriteData(0x88);
SPI_WriteData(0x88);
SPI_WriteData(0x23);
SPI_WriteData(0x10);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x1C);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x30);
SPI_WriteData(0x05);
SPI_WriteData(0xA0);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00); /// Set GIP2

SSD2828_WritePackageSize(35);
SPI_WriteData(0xE0);
SPI_WriteData(0x00);
SPI_WriteData(0x06);
SPI_WriteData(0x08);
SPI_WriteData(0x2A);
SPI_WriteData(0x31);
SPI_WriteData(0x3F);
SPI_WriteData(0x38);
SPI_WriteData(0x36);
SPI_WriteData(0x07);
SPI_WriteData(0x0C);
SPI_WriteData(0x0D);
SPI_WriteData(0x11);
SPI_WriteData(0x13);
SPI_WriteData(0x12);
SPI_WriteData(0x13);
SPI_WriteData(0x11);
SPI_WriteData(0x18);
SPI_WriteData(0x00);
SPI_WriteData(0x06);
SPI_WriteData(0x08);
SPI_WriteData(0x2A);
SPI_WriteData(0x31);
SPI_WriteData(0x3F);
SPI_WriteData(0x38);
SPI_WriteData(0x36);
SPI_WriteData(0x07);
SPI_WriteData(0x0C);
SPI_WriteData(0x0D);
SPI_WriteData(0x11);
SPI_WriteData(0x13);
SPI_WriteData(0x12);
SPI_WriteData(0x13);
SPI_WriteData(0x11);
SPI_WriteData(0x18);  /// Set Gamma 20160621

SSD2828_WritePackageSize(2);
SPI_WriteData(0x29);
SPI_WriteData(0);  ///Display On

Delay(50);




////////////////////Initial  CODE/////////////////////



//SSD2828_Initial
SPI_WriteCmd(0xb7);
SPI_WriteData(0x50);
SPI_WriteData(0x00);  
 
SPI_WriteCmd(0xb8);
SPI_WriteData(0x00);
SPI_WriteData(0x00);   

SPI_WriteCmd(0xb9);
SPI_WriteData(0x00);
SPI_WriteData(0x00);

SPI_WriteCmd(0xBA);
SPI_WriteData(0x2b);
SPI_WriteData(0x82);

SPI_WriteCmd(0xBB);
SPI_WriteData(0x07);
SPI_WriteData(0x00);

SPI_WriteCmd(0xb9);
SPI_WriteData(0x01);
SPI_WriteData(0x00);

SPI_WriteCmd(0xc9);
SPI_WriteData(0x02);
SPI_WriteData(0x23); 
Delay(5);

SPI_WriteCmd(0xCA);
SPI_WriteData(0x01);
SPI_WriteData(0x23);

SPI_WriteCmd(0xCB); 
SPI_WriteData(0x10);
SPI_WriteData(0x05);

SPI_WriteCmd(0xCC); 
SPI_WriteData(0x05);
SPI_WriteData(0x10);
Delay(5);

SPI_WriteCmd(0xD0); 
SPI_WriteData(0x00);
SPI_WriteData(0x00);
Delay(5);


LoadLcdParameterConfig();   	

SPI_WriteCmd(0xb6);
SPI_WriteData(0x03);
SPI_WriteData(0x00);


SPI_WriteCmd(0xDE);
SPI_WriteData(0x03);  //01 2LINE  02  3LINE  03 4LINE
SPI_WriteData(0x00);

SPI_WriteCmd(0xD6);
SPI_WriteData(0x05);
SPI_WriteData(0x00);

SPI_WriteCmd(0xB7);
SPI_WriteData(0x4B);
SPI_WriteData(0x02);Delay(100);

SPI_WriteCmd(0x2C);




     
}