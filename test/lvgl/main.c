#include "lvgl/lvgl.h"
#include "screen.h"
#include "uart.h"

#define	here()	printf("@ %s, %d\n", __FILE__, __LINE__)

#define	HW_COLUMNS	SCREEN_WIDTH

lv_obj_t *label2, *label1;
lv_obj_t *label3;

static void btn_event_cb(lv_obj_t * btn, lv_event_t event)
{
    if(event == LV_EVENT_RELEASED) {
        lv_label_set_text(label1, "RELEASED");
    } else if (event == LV_EVENT_PRESSED) {
        lv_label_set_text(label1, "CLICKED");
    }
}

bool my_input_read(lv_indev_drv_t * drv, lv_indev_data_t*data)
{
    data->point.x = 0;	//Touch_Coord[0]; 
    data->point.y = 0;	//Touch_Coord[1];
    data->state = LV_INDEV_STATE_REL;	//f_touch_detected ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
    return false; /*No buffering now so no more data read*/
}

void flush_one_row(uint32_t address, uint8_t *buffer, int len)
{
	//printf("address: 0x%08x, buffer: %p, len: %d\n", address, buffer, len);
}

void my_flush_cb(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

	//printf("x1: %d, y1: %d, x2: %d, y2: %d\n", area->x1, area->y1, area->x2, area->y2);
	int i, j;
	uint32_t *buffer = (uint32_t *)color_p;
	//uint32_t rgb32;
	for (j=area->y1; j<=area->y2; j++) {
		for (i=area->x1; i<=area->x2; i++) {
			//put_pixel(i, j, *buffer++);
			put_pixel_ex(i, j, *buffer++);
		}
	}

    lv_disp_flush_ready(disp_drv);
}

void my_rounder(struct _disp_drv_t * disp_drv, lv_area_t *a)
{
/*
    a->x1 = a->x1 & ~(0x7);
    a->x2 = a->x2 |  (0x7);
*/
}

void my_set_px_cb(struct _disp_drv_t * disp_drv, uint8_t * buf, lv_coord_t buf_w, lv_coord_t x, lv_coord_t y, lv_color_t color, lv_opa_t opa)
{
/*
    buf += buf_w/8 * y;
    buf += x/8;
    if(lv_color_brightness(color) > 128) {(*buf) |= (1 << (7 - x % 8));}
    else {(*buf) &= ~(1 << (7 - x % 8));}
*/
}

void delay(uint32_t n)
{
	while (n) {
		n--;
	}
}

int main(void)
{
	uart_putchar('A');
	screen_init();   

	uart_putchar('B');

    lv_init();
    
	uart_putchar('C');

	static uint32_t gbuf[24*320];
    static lv_disp_buf_t disp_buf;
    lv_disp_buf_init(&disp_buf, gbuf, NULL, 24*320);
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.buffer = &disp_buf;
    disp_drv.flush_cb = my_flush_cb;
    //disp_drv.set_px_cb = my_set_px_cb;
    //disp_drv.rounder_cb = my_rounder;
  
    lv_disp_t * disp;
    disp = lv_disp_drv_register(&disp_drv);
    
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb =my_input_read;
    lv_indev_t * my_indev = lv_indev_drv_register(&indev_drv);
    
    lv_obj_t * scr = lv_disp_get_scr_act(NULL);
    //lv_theme_t * th = lv_theme_mono_init(0, NULL);
    lv_theme_t * th = lv_theme_material_init(0, NULL);
    /* Set the mono system theme */
    lv_theme_set_current(th);
    
    /*Create a Label on the currently active screen*/
    label1 =  lv_label_create(scr, NULL);
    lv_label_set_text(label1, "");
    lv_obj_set_pos(label1,30, 30);// position, position);
    
	/*Create a button on the currently loaded screen*/    
    lv_obj_t * btn1 = lv_btn_create(scr, NULL);
    lv_obj_set_event_cb(btn1, btn_event_cb);                                  /*Set function to be called when the button is released*/
    //lv_obj_align(btn1, label2, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);               /*Align below the label*/
    lv_obj_set_pos(btn1, 20, 50);
    
    /*Create a label on the button (the 'label' variable can be reused)*/
    label2 = lv_label_create(btn1, NULL);
    lv_label_set_text(label2, "Click me!");
    lv_obj_set_size(btn1, 100, 20);
    
    /*Create a label on the button (the 'label' variable can be reused)*/
    label3 = lv_label_create(scr, NULL);
    lv_label_set_text(label3, "???");
    lv_obj_set_pos(label3,10, 100);// position, position);

#if 1
	uint32_t cnt = 0;
	while (1) {
		lv_task_handler();
        lv_tick_inc(1);
        delay(1000);
		if ( cnt % 100 == 0 ) {
			char buffer[64];
			sprintf(buffer, "cnt: %d", cnt/100);
			lv_label_set_text(label3, buffer);
		}
		cnt++;
    }
	screen_exit();
#endif
	return 0;
}
