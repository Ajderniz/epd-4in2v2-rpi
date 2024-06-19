#include <stdlib.h>		// malloc() free() exit()
#include <signal.h>		//signal()
#include <string.h>
#include "EPD_4in2_V2.h"
#include "DEV_Config.h"
#include "GUI_Paint.h"
#include "GUI_BMPfile.h"
#include "ImageData.h"
#include "Debug.h"

void  Handler(int signo)
{
    //System Exit
    printf("\r\nHandler:exit\r\n");
    DEV_Module_Exit();

    exit(0);
}

int main(void)
{
    // Exception handling:ctrl + c
    signal(SIGINT, Handler);
    
    printf("EPD_test Demo\r\n");
    if(DEV_Module_Init()!=0){
        return -1;
    }

    printf("e-Paper Init and Clear...\r\n");
    EPD_Init();
    EPD_Clear();
    DEV_Delay_ms(500);

    //Create a new image cache
    UBYTE *BlackImage;
    /* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
    UWORD Imagesize = ((EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1)) * EPD_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_WIDTH, EPD_HEIGHT, 0, WHITE);

#if 1  // show bmp
    printf("show window BMP-----------------\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    GUI_ReadBmp("./pic/100x100.bmp", 10, 10);
    EPD_Display(BlackImage);
    DEV_Delay_ms(2000);

    // EPD_Init_Fast(Seconds_1_5S);
    // printf("show bmp------------------------\r\n");
    // Paint_SelectImage(BlackImage);
    // GUI_ReadBmp("./pic/4in2.bmp", 0, 0);
    // EPD_Display_Fast(BlackImage);
    // DEV_Delay_ms(2000);
#endif        

#if 1  // show image for array   
    EPD_Init_Fast(Seconds_1_5S);
    printf("show image for array\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_DrawBitMap(gImage_4in2);
    EPD_Display_Fast(BlackImage);
    DEV_Delay_ms(2000);
#endif

#if 1   // Drawing on the image

    EPD_Init();
    //1.Select Image
    printf("SelectImage:BlackImage\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);

    // 2.Drawing on the image
    printf("Drawing:BlackImage\r\n");
    Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawCircle(45, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(105, 95, 20, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawLine(85, 95, 125, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawString_EN(10, 0, "waveshare", &Font16, BLACK, WHITE);
    Paint_DrawString_EN(10, 20, "hello world", &Font12, WHITE, BLACK);
    Paint_DrawNum(10, 33, 123456789, &Font12, BLACK, WHITE);
    Paint_DrawNum(10, 50, 987654321, &Font16, WHITE, BLACK);
    Paint_DrawString_CN(130, 0, "你好abc", &Font12CN, BLACK, WHITE);
    Paint_DrawString_CN(130, 20, "微雪电子", &Font24CN, WHITE, BLACK);

    printf("EPD_Display\r\n");
    // EPD_Display(BlackImage);
	EPD_Display(BlackImage);
	DEV_Delay_ms(2000);
#endif

#if 1
	printf("Partial refresh\r\n");
    Paint_NewImage(BlackImage, 200, 50, 0, WHITE);
	PAINT_TIME sPaint_time;
    sPaint_time.Hour = 12;
    sPaint_time.Min = 34;
    sPaint_time.Sec = 56;
    UBYTE num = 10;
	for (;;) {
		sPaint_time.Sec = sPaint_time.Sec + 1;
		if (sPaint_time.Sec == 60) {
			sPaint_time.Min = sPaint_time.Min + 1;
			sPaint_time.Sec = 0;
			if (sPaint_time.Min == 60) {
				sPaint_time.Hour =  sPaint_time.Hour + 1;
				sPaint_time.Min = 0;
				if (sPaint_time.Hour == 24) {
					sPaint_time.Hour = 0;
					sPaint_time.Min = 0;
					sPaint_time.Sec = 0;
				}
			}
		}
		Paint_Clear(WHITE);
		Paint_DrawTime(20, 10, &sPaint_time, &Font20, WHITE, BLACK);
		EPD_PartialDisplay(BlackImage, 80, 200, 200, 50);
		DEV_Delay_ms(500);//Analog clock 1s
		num = num - 1;
		if(num == 0) {
			break;
		}
    }
#endif


#if 1
    // EPD_Init();
	// EPD_Clear();
	EPD_Init_4Gray();
	printf("show Gray------------------------\r\n");
	free(BlackImage);
	BlackImage = NULL;
	Imagesize = ((EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 4 ): (EPD_WIDTH / 4 + 1)) * EPD_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
	Paint_NewImage(BlackImage, EPD_WIDTH, EPD_HEIGHT, 0, WHITE);
	Paint_SetScale(4);
	Paint_Clear(WHITE);

	Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawCircle(45, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(105, 95, 20, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawLine(85, 95, 125, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawString_EN(10, 0, "waveshare", &Font16, BLACK, WHITE);
    Paint_DrawString_EN(10, 20, "hello world", &Font12, WHITE, BLACK);
    Paint_DrawNum(10, 33, 123456789, &Font12, BLACK, WHITE);
    Paint_DrawNum(10, 50, 987654321, &Font16, WHITE, BLACK);
    Paint_DrawString_CN(140, 0, "你好abc", &Font12CN, GRAY1, GRAY4);
    Paint_DrawString_CN(140, 40, "你好abc", &Font12CN, GRAY2, GRAY3);
    Paint_DrawString_CN(140, 80, "你好abc", &Font12CN, GRAY3, GRAY2);
    Paint_DrawString_CN(140, 120, "你好abc", &Font12CN, GRAY4, GRAY1);
	
    Paint_DrawString_CN(220, 0, "微雪电子", &Font24CN, GRAY1, GRAY4);
    Paint_DrawString_CN(220, 40, "微雪电子", &Font24CN, GRAY2, GRAY3);
    Paint_DrawString_CN(220, 80, "微雪电子", &Font24CN, GRAY3, GRAY2);
    Paint_DrawString_CN(220, 120, "微雪电子", &Font24CN, GRAY4, GRAY1);
	
	EPD_Display_4Gray(BlackImage);
	DEV_Delay_ms(2000);

	Paint_Clear(WHITE);
    EPD_Display_4Gray(gImage_4in2_4Gray1);
	DEV_Delay_ms(2000);

	GUI_ReadBmp_4Gray("./pic/4in2_Scale.bmp",0 , 0);
	EPD_Display_4Gray(BlackImage);
	DEV_Delay_ms(2000);
	
	Paint_Clear(WHITE);
    GUI_ReadBmp("./pic/100x100.bmp", 20, 20);
    EPD_Display_4Gray(BlackImage);
	DEV_Delay_ms(2000);

#endif

    EPD_Init();
    EPD_Clear();
    printf("Goto Sleep...\r\n");
    EPD_Sleep();
    free(BlackImage);
    BlackImage = NULL;
    DEV_Delay_ms(2000);//important, at least 2s
    // close 5V
    printf("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();
    
    return 0;
}
