// SPDX-License-Identifier: LGPL-2.1-or-later
#include "BufferDraw.h"
 
static const uint8_t * font;
static const uint16_t * font_conf;
static unsigned int draw_mode = NORMAL;

static uint16_t char_x;
static uint16_t char_y;
 
static uint8_t * image;
static uint16_t len;
static uint16_t bytes_per_line;
static uint16_t width;
static uint16_t height;
static uint8_t black;
static uint8_t white;
 
void BD_init( uint8_t * p_buffer, uint16_t p_len, uint16_t p_width, uint16_t p_height, uint8_t p_black, uint8_t p_white)
{
    image=p_buffer;
    len=p_len;
    width=p_width;
    bytes_per_line=width/8;
    height=p_height;
    black=p_black;
    white=p_white;
}

// set one pixel in buffer
void BD_pixel(uint16_t x, uint16_t y, uint8_t color)
{
   // first check parameter
   if(x >= width || y >= height) return;

   if(draw_mode == NORMAL) {
       if(color == 0)
           image[(x / 8) + ((y) * bytes_per_line)] &= ~(1 << (7-x%8));  // erase pixel
       else
           image[(x / 8) + ((y) * bytes_per_line)] |= (1 << (7-x%8));   // set pixel
   } else { // XOR mode
       if(color == 1)
           image[(x / 8) + ((y) * bytes_per_line)] ^= (1 << (7-x%8));   // xor pixel
   }
}

// clear screen
void BD_clear(void)
{
   for(uint16_t i =0; i <len; i++)
   {
       image[i]= white ? 0xff: 0;
   }
}

// print line
void BD_line(int x0, int y0, int x1, int y1, int color)
{
   int   dx = 0, dy = 0;
   int   dx_sym = 0, dy_sym = 0;
   int   dx_x2 = 0, dy_x2 = 0;
   int   di = 0;

   dx = x1-x0;
   dy = y1-y0;

   if (dx > 0) {
       dx_sym = 1;
   } else {
       dx_sym = -1;
   }

   if (dy > 0) {
       dy_sym = 1;
   } else {
       dy_sym = -1;
   }

   dx = dx_sym*dx;
   dy = dy_sym*dy;

   dx_x2 = dx*2;
   dy_x2 = dy*2;

   if (dx >= dy) {
       di = dy_x2 - dx;
       while (x0 != x1) {

           BD_pixel(x0, y0, color);
           x0 += dx_sym;
           if (di<0) {
               di += dy_x2;
           } else {
               di += dy_x2 - dx_x2;
               y0 += dy_sym;
           }
       }
       BD_pixel(x0, y0, color);
   } else {
       di = dx_x2 - dy;
       while (y0 != y1) {
           BD_pixel(x0, y0, color);
           y0 += dy_sym;
           if (di < 0) {
               di += dx_x2;
           } else {
               di += dx_x2 - dy_x2;
               x0 += dx_sym;
           }
       }
       BD_pixel(x0, y0, color);
   }
}

// print rect
void BD_rect(int x0, int y0, int x1, int y1, int color)
{

   if (x1 > x0) BD_line(x0,y0,x1,y0,color);
   else  BD_line(x1,y0,x0,y0,color);

   if (y1 > y0) BD_line(x0,y0,x0,y1,color);
   else BD_line(x0,y1,x0,y0,color);

   if (x1 > x0) BD_line(x0,y1,x1,y1,color);
   else  BD_line(x1,y1,x0,y1,color);

   if (y1 > y0) BD_line(x1,y0,x1,y1,color);
   else BD_line(x1,y1,x1,y0,color);
}

// print filled rect
void BD_fillrect(int x0, int y0, int x1, int y1, int color)
{
   int l,c,i;
   if(x0 > x1) {
       i = x0;
       x0 = x1;
       x1 = i;
   }

   if(y0 > y1) {
       i = y0;
       y0 = y1;
       y1 = i;
   }

   for(l = x0; l<= x1; l ++) {
       for(c = y0; c<= y1; c++) {
          BD_pixel(l,c,color);
       }
   }
}

// print circle
void BD_circle(int x0, int y0, int r, int color)
{
   int x = -r, y = 0, err = 2-2*r, e2;
   do {
       BD_pixel(x0-x, y0+y,color);
       BD_pixel(x0+x, y0+y,color);
       BD_pixel(x0+x, y0-y,color);
       BD_pixel(x0-x, y0-y,color);
       e2 = err;
       if (e2 <= y) {
           err += ++y*2+1;
           if (-x == y && e2 <= x) e2 = 0;
       }
       if (e2 > x) err += ++x*2+1;
   } while (x <= 0);

}

// print filled circle
void BD_fillcircle(int x0, int y0, int r, int color)
{
   int x = -r, y = 0, err = 2-2*r, e2;
   do {
       BD_line(x0-x, y0-y, x0-x, y0+y, color);
       BD_line(x0+x, y0-y, x0+x, y0+y, color);
       e2 = err;
       if (e2 <= y) {
           err += ++y*2+1;
           if (-x == y && e2 <= x) e2 = 0;
       }
       if (e2 > x) err += ++x*2+1;
   } while (x <= 0);
}

// set drawing mode
void BD_setmode(int mode)
{
   draw_mode = mode;
}

// set cursor position
void BD_locate(int x, int y)
{
   char_x = x;
   char_y = y;
}

// calc char columns
int BD_columns(void)
{
   return width / font_conf[1];
}

// calc char rows
int BD_rows(void)
{
   return height / font_conf[2];
}

// print char
int BD_putc(char value)
{
   if (value == '\n') {    // new BD_line
       char_x = 0;
       char_y = char_y + font_conf[2];
       if (char_y >= (height - font_conf[2])) {
           char_y = 0;
       }
   } else {
       BD_character(char_x, char_y, value);
   }
   return value;
}

// print string
void BD_puts(const char * value)
{
    const char * c=value;
    while(*c != '\0'){
        BD_putc(*c);
        c++;
    }

}

// paint char out of font
void BD_character(int x, int y, int c)
{
   unsigned int hor,vert,offset,bpl,j,i,b;
   const unsigned char* zeichen;
   unsigned char z,w;

   if ((c < 31) || (c > 127)) return;   // test char range

   // read font parameter from start of array
   offset = font_conf[0];                    // bytes / char
   hor = font_conf[1];                       // get hor size of font
   vert = font_conf[2];                      // get vert size of font
   bpl = font_conf[3];                       // bytes per BD_line

   if (char_x + hor > width) {
       char_x = 0;
       char_y = char_y + vert;
       if (char_y >= height - font_conf[2]) {
           char_y = 0;
       }
   }

   zeichen = &font[((c -32) * offset) ]; // start of char bitmap
   w = zeichen[0];                          // width of actual char
   // construct the char into the buffer
   for (j=0; j<vert; j++) {  //  vert BD_line
       for (i=0; i<hor; i++) {   //  horz BD_line
           z =  zeichen[bpl * i + ((j & 0xF8) >> 3)+1];
           b = 1 << (j & 0x07);
           if (( z & b ) == 0x00) {
              BD_pixel(x+i,y+j,white);
           } else {
              BD_pixel(x+i,y+j,black);
           }

       }
   }

   char_x += w;
}

// set actual font
void BD_set_font(const unsigned char* f, const unsigned short* f_conf)
{
   font = f;
   font_conf = f_conf;
}

void BD_print_bm(struct Bitmap bm, int x, int y)
{
   int h,v,b;
   char d;

   for(v=0; v < bm.ySize; v++) {   // lines
       for(h=0; h < bm.xSize; h++) { //pixel
           if(h + x > width) break;
           if(v + y > height) break;
           d = bm.data[bm.Byte_in_Line * v + ((h & 0xF8) >> 3)];
           b = 0x80 >> (h & 0x07);
           if((d & b) == 0) {
              BD_pixel(x+h,y+v,0);
           } else {
              BD_pixel(x+h,y+v,1);
           }
       }
   }

}

void BD_clip_image(const unsigned char * im_in, uint16_t x, uint16_t y, uint16_t i_width, uint16_t i_height){
    if(x>=width || y >= height) return;
    if(x+i_width>width) i_width = width -x;
    if(y+i_height>height) i_height = height -y;
    x = (x/8); // align to lowest bytes

    for(uint16_t i = 0;i<i_width/8;i++)
        for(uint16_t j = 0;j<i_height;j++)
        {
            image[x+i+(y+j)*bytes_per_line]=im_in[i+j*i_width/8];
        }

}
