/*****************************************************************************
* | File      	:   EPD_4in2_V2.c
* | Author      :   Waveshare team
* | Function    :   4.2inch e-paper V2
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2023-09-11
* | Info        :
* -----------------------------------------------------------------------------
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "EPD_4in2_V2.h"
#include "Debug.h"

const unsigned char LUT_ALL[233]={							
0x01,	0x0A,	0x1B,	0x0F,	0x03,	0x01,	0x01,	
0x05,	0x0A,	0x01,	0x0A,	0x01,	0x01,	0x01,	
0x05,	0x08,	0x03,	0x02,	0x04,	0x01,	0x01,	
0x01,	0x04,	0x04,	0x02,	0x00,	0x01,	0x01,	
0x01,	0x00,	0x00,	0x00,	0x00,	0x01,	0x01,	
0x01,	0x00,	0x00,	0x00,	0x00,	0x01,	0x01,	
0x01,	0x0A,	0x1B,	0x0F,	0x03,	0x01,	0x01,	
0x05,	0x4A,	0x01,	0x8A,	0x01,	0x01,	0x01,	
0x05,	0x48,	0x03,	0x82,	0x84,	0x01,	0x01,	
0x01,	0x84,	0x84,	0x82,	0x00,	0x01,	0x01,	
0x01,	0x00,	0x00,	0x00,	0x00,	0x01,	0x01,	
0x01,	0x00,	0x00,	0x00,	0x00,	0x01,	0x01,	
0x01,	0x0A,	0x1B,	0x8F,	0x03,	0x01,	0x01,	
0x05,	0x4A,	0x01,	0x8A,	0x01,	0x01,	0x01,	
0x05,	0x48,	0x83,	0x82,	0x04,	0x01,	0x01,	
0x01,	0x04,	0x04,	0x02,	0x00,	0x01,	0x01,	
0x01,	0x00,	0x00,	0x00,	0x00,	0x01,	0x01,	
0x01,	0x00,	0x00,	0x00,	0x00,	0x01,	0x01,	
0x01,	0x8A,	0x1B,	0x8F,	0x03,	0x01,	0x01,	
0x05,	0x4A,	0x01,	0x8A,	0x01,	0x01,	0x01,	
0x05,	0x48,	0x83,	0x02,	0x04,	0x01,	0x01,	
0x01,	0x04,	0x04,	0x02,	0x00,	0x01,	0x01,	
0x01,	0x00,	0x00,	0x00,	0x00,	0x01,	0x01,	
0x01,	0x00,	0x00,	0x00,	0x00,	0x01,	0x01,	
0x01,	0x8A,	0x9B,	0x8F,	0x03,	0x01,	0x01,	
0x05,	0x4A,	0x01,	0x8A,	0x01,	0x01,	0x01,	
0x05,	0x48,	0x03,	0x42,	0x04,	0x01,	0x01,	
0x01,	0x04,	0x04,	0x42,	0x00,	0x01,	0x01,	
0x01,	0x00,	0x00,	0x00,	0x00,	0x01,	0x01,	
0x01,	0x00,	0x00,	0x00,	0x00,	0x01,	0x01,	
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
0x02,	0x00,	0x00,	0x07,	0x17,	0x41,	0xA8,	
0x32,	0x30,						
};		

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_Reset(void)
{
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(100);
    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(2);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(100);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_SendCommand(UBYTE Reg)
{
    DEV_Digital_Write(EPD_DC_PIN, 0);
    DEV_Digital_Write(EPD_CS_PIN, 0);
    DEV_SPI_WriteByte(Reg);
    DEV_Digital_Write(EPD_CS_PIN, 1);
}

/******************************************************************************
function :	send data
parameter:
    Data : Write data
******************************************************************************/
static void EPD_SendData(UBYTE Data)
{
    DEV_Digital_Write(EPD_DC_PIN, 1);
    DEV_Digital_Write(EPD_CS_PIN, 0);
    DEV_SPI_WriteByte(Data);
    DEV_Digital_Write(EPD_CS_PIN, 1);
}

/******************************************************************************
function :	Wait until the busy_pin goes LOW
parameter:
******************************************************************************/
void EPD_ReadBusy(void)
{
    Debug("e-Paper busy\r\n");
    while(DEV_Digital_Read(EPD_BUSY_PIN) == 1) {      //LOW: idle, HIGH: busy
        DEV_Delay_ms(10);
    }
    Debug("e-Paper busy release\r\n");
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_TurnOnDisplay(void)
{
    EPD_SendCommand(0x22);
	EPD_SendData(0xF7);
    EPD_SendCommand(0x20);
    EPD_ReadBusy();
}

static void EPD_TurnOnDisplay_Fast(void)
{
    EPD_SendCommand(0x22);
	EPD_SendData(0xC7);
    EPD_SendCommand(0x20);
    EPD_ReadBusy();
}

static void EPD_TurnOnDisplay_Partial(void)
{
    EPD_SendCommand(0x22);
	EPD_SendData(0xFF);
    EPD_SendCommand(0x20);
    EPD_ReadBusy();
}

static void EPD_TurnOnDisplay_4Gray(void)
{
    EPD_SendCommand(0x22);
	EPD_SendData(0xCF);
    EPD_SendCommand(0x20);
    EPD_ReadBusy();
}

/******************************************************************************
function :	Setting the display window
parameter:
******************************************************************************/
static void EPD_SetWindows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend)
{
    EPD_SendCommand(0x44); // SET_RAM_X_ADDRESS_START_END_POSITION
    EPD_SendData((Xstart>>3) & 0xFF);
    EPD_SendData((Xend>>3) & 0xFF);
	
    EPD_SendCommand(0x45); // SET_RAM_Y_ADDRESS_START_END_POSITION
    EPD_SendData(Ystart & 0xFF);
    EPD_SendData((Ystart >> 8) & 0xFF);
    EPD_SendData(Yend & 0xFF);
    EPD_SendData((Yend >> 8) & 0xFF);
}

/******************************************************************************
function :	Set Cursor
parameter:
******************************************************************************/
static void EPD_SetCursor(UWORD Xstart, UWORD Ystart)
{
    EPD_SendCommand(0x4E); // SET_RAM_X_ADDRESS_COUNTER
    EPD_SendData(Xstart & 0xFF);

    EPD_SendCommand(0x4F); // SET_RAM_Y_ADDRESS_COUNTER
    EPD_SendData(Ystart & 0xFF);
    EPD_SendData((Ystart >> 8) & 0xFF);
}

//LUT download
static void EPD_4Gray_lut(void)
{
    unsigned char i;

    //WS byte 0~152, the content of VS[nX-LUTm], TP[nX], RP[n], SR[nXY], FR[n] and XON[nXY]
    EPD_SendCommand(0x32);					
    for(i=0;i<227;i++)
    {
        EPD_SendData(LUT_ALL[i]);
    }	
    //WS byte 153, the content of Option for LUT end	
    EPD_SendCommand(0x3F);					
    EPD_SendData(LUT_ALL[i++]);

    //WS byte 154, the content of gate leve
    EPD_SendCommand(0x03);					
    EPD_SendData(LUT_ALL[i++]);//VGH

    //WS byte 155~157, the content of source level
    EPD_SendCommand(0x04);					
    EPD_SendData(LUT_ALL[i++]);//VSH1
    EPD_SendData(LUT_ALL[i++]);//VSH2
    EPD_SendData(LUT_ALL[i++]);//VSL

    //WS byte 158, the content of VCOM level
    EPD_SendCommand(0x2c);					
    EPD_SendData(LUT_ALL[i++]);//VCOM
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_Init(void)
{
    EPD_Reset();

    EPD_ReadBusy();   
    EPD_SendCommand(0x12);   // soft  reset
    EPD_ReadBusy();

    EPD_SendCommand(0x21); //  Display update control
    EPD_SendData(0x40);		
    EPD_SendData(0x00);		

    EPD_SendCommand(0x3C); //BorderWavefrom
    EPD_SendData(0x05);
	
    EPD_SendCommand(0x11);	// data  entry  mode
    EPD_SendData(0x03);		// X-mode   
		
	EPD_SetWindows(0, 0, EPD_WIDTH-1, EPD_HEIGHT-1);
	 
	EPD_SetCursor(0, 0);
	
    EPD_ReadBusy();
}

/******************************************************************************
function :	Initialize Fast the e-Paper register
parameter:
******************************************************************************/
void EPD_Init_Fast(UBYTE Mode)
{
    EPD_Reset();

    EPD_ReadBusy();   
    EPD_SendCommand(0x12);   // soft  reset
    EPD_ReadBusy();

    EPD_SendCommand(0x21);	 
    EPD_SendData(0x40);
    EPD_SendData(0x00); 

    EPD_SendCommand(0x3C);     
    EPD_SendData(0x05);  

    if(Mode == Seconds_1_5S)
    {
        //1.5s
        EPD_SendCommand(0x1A); // Write to temperature register
        EPD_SendData(0x6E);		
    }
    else if(Mode == Seconds_1S)
    {
        //1s
        EPD_SendCommand(0x1A); // Write to temperature register
        EPD_SendData(0x5A);	
    }
                 
    EPD_SendCommand(0x22); // Load temperature value
    EPD_SendData(0x91);		
    EPD_SendCommand(0x20);	
    EPD_ReadBusy();   
	
    EPD_SendCommand(0x11);	// data  entry  mode
    EPD_SendData(0x03);		// X-mode   
		
	EPD_SetWindows(0, 0, EPD_WIDTH-1, EPD_HEIGHT-1);
	 
	EPD_SetCursor(0, 0);
	
    EPD_ReadBusy();
}


void EPD_Init_4Gray(void)
{
    EPD_Reset();

    EPD_SendCommand(0x12);  //SWRESET
    EPD_ReadBusy();   

    EPD_SendCommand(0x21);	 
    EPD_SendData(0x00);
    EPD_SendData(0x00);

    EPD_SendCommand(0x3C);     
    EPD_SendData(0x03);

    EPD_SendCommand(0x0C); //BTST
    EPD_SendData(0x8B);//8B
    EPD_SendData(0x9C);//9C
    EPD_SendData(0xA4);//96 A4
    EPD_SendData(0x0F);//0F	

    EPD_4Gray_lut(); //LUT

    EPD_SendCommand(0x11);	// data  entry  mode
    EPD_SendData(0x03);		// X-mode   
		
	EPD_SetWindows(0, 0, EPD_WIDTH-1, EPD_HEIGHT-1);
	 
	EPD_SetCursor(0, 0);
}
/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_Clear(void)
{
    UWORD Width, Height;
    Width = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1);
    Height = EPD_HEIGHT;

    EPD_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_SendData(0xFF);
        }
    }

    EPD_SendCommand(0x26);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_SendData(0xFF);
        }
    }
    EPD_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_Display(UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1);
    Height = EPD_HEIGHT;

    EPD_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_SendData(Image[i + j * Width]);
        }
    }

    EPD_SendCommand(0x26);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_SendData(Image[i + j * Width]);
        }
    }
    EPD_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and fast displays
parameter:
******************************************************************************/
void EPD_Display_Fast(UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1);
    Height = EPD_HEIGHT;

    EPD_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_SendData(Image[i + j * Width]);
        }
    }

    EPD_SendCommand(0x26);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_SendData(Image[i + j * Width]);
        }
    }
    EPD_TurnOnDisplay_Fast();
}


void EPD_Display_4Gray(UBYTE *Image)
{
    UDOUBLE i,j,k,m;
    UBYTE temp1,temp2,temp3;
/****Color display description****
      white  gray2  gray1  black
0x10|  01     01     00     00
0x13|  01     00     01     00
*********************************/
	EPD_SendCommand(0x24);	       
	// EPD_4IN2_HEIGHT
	// EPD_4IN2_WIDTH
	for(m = 0; m<EPD_HEIGHT;m++)
		for(i=0;i<EPD_WIDTH/8;i++)
		{
			temp3=0;
			for(j=0;j<2;j++)	
			{
				
				temp1 = Image[(m*(EPD_WIDTH/8)+i)*2+j];
				for(k=0;k<2;k++)	
				{
					temp2 = temp1&0xC0 ;
					if(temp2 == 0xC0)
						temp3 |= 0x01;//white
					else if(temp2 == 0x00)
						temp3 |= 0x00;  //black
					else if(temp2 == 0x80) 
						temp3 |= 0x00;  //gray1
					else //0x40
						temp3 |= 0x01; //gray2
					temp3 <<= 1;	
					
					temp1 <<= 2;
					temp2 = temp1&0xC0 ;
					if(temp2 == 0xC0)  //white
						temp3 |= 0x01;
					else if(temp2 == 0x00) //black
						temp3 |= 0x00;
					else if(temp2 == 0x80)
						temp3 |= 0x00; //gray1
					else    //0x40
							temp3 |= 0x01;	//gray2	
					if(j!=1 || k!=1)				
						temp3 <<= 1;
					
					temp1 <<= 2;
				}
				
			 }
			EPD_SendData(temp3);			
		}
    // new  data
    EPD_SendCommand(0x26);	 
	for(m = 0; m<EPD_HEIGHT;m++)
		for(i=0;i<EPD_WIDTH/8;i++)
		{
			temp3=0;
			for(j=0;j<2;j++)	
			{
				temp1 = Image[(m*(EPD_WIDTH/8)+i)*2+j];
				for(k=0;k<2;k++)	
				{
					temp2 = temp1&0xC0 ;
					if(temp2 == 0xC0)
						temp3 |= 0x01;//white
					else if(temp2 == 0x00)
						temp3 |= 0x00;  //black
					else if(temp2 == 0x80) 
						temp3 |= 0x01;  //gray1
					else //0x40
						temp3 |= 0x00; //gray2
					temp3 <<= 1;	
					
					temp1 <<= 2;
					temp2 = temp1&0xC0 ;
					if(temp2 == 0xC0)  //white
						temp3 |= 0x01;
					else if(temp2 == 0x00) //black
						temp3 |= 0x00;
					else if(temp2 == 0x80)
						temp3 |= 0x01; //gray1
					else    //0x40
							temp3 |= 0x00;	//gray2
					if(j!=1 || k!=1)					
						temp3 <<= 1;
					
					temp1 <<= 2;
				}
				
			 }
			EPD_SendData(temp3);	
		}
    EPD_TurnOnDisplay_4Gray();
}

// Send partial data for partial refresh
void EPD_PartialDisplay(UBYTE *Image, UWORD x, UWORD y, UWORD w, UWORD l)
{
    UWORD Width, Height;
    Width = (w % 8 == 0)? (w / 8 ): (w / 8 + 1);
    Height = l;

	EPD_SendCommand(0x3C); //BorderWavefrom,
	EPD_SendData(0x80);	

	EPD_SendCommand(0x21); 
	EPD_SendData(0x00);
	EPD_SendData(0x00);

	EPD_SendCommand(0x3C); 
	EPD_SendData(0x80); 

	EPD_SetWindows(x, y, x+w-1, y+l-1);
	EPD_SetCursor(x, y);
	
    EPD_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_SendData(Image[i + j * Width]);
        }
    }
	
	EPD_TurnOnDisplay_Partial();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_Sleep(void)
{
    EPD_SendCommand(0x10); // DEEP_SLEEP
    EPD_SendData(0x01);
	DEV_Delay_ms(200);
}
