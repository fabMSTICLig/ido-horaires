// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * Draw in image array based on https://os.mbed.com/users/dreschpe/code/EaEpaper/
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
 
#ifndef BUFFERD_RAW_H
#define BUFFERD_RAW_H

#include <stdint.h>

/** Draw mode
  * NORMAl
  * XOR set pixel by xor of the screen
  */
enum {NORMAL,XOR};
 
/** Bitmap
 */
struct Bitmap{
    int xSize;
    int ySize;
    int Byte_in_Line;
    char* data;
};
/** Initialize the Buffer Draw Lib 
 * len must be at least width x height / 8
 */
void BD_init( uint8_t * buffer, uint16_t len, uint16_t width, uint16_t height, uint8_t black, uint8_t white);

/**
 * set or reset a single pixel
 *
 * @param x horizontal position
 * @param y vertical position
 * @param color : 0 white, 1 black
 */ 
void BD_pixel(uint16_t x, uint16_t y, uint8_t color);

/** Fill the screen with white
 *
 */ 
void BD_clear(void);

/** draw a 1 pixel line
 *
 * @param x0,y0 start point
 * @param x1,y1 stop point
 * @param color  : 0 white, 1 black
 */  
void BD_line(int x0, int y0, int x1, int y1, int color);

/** draw a rect
 *
 * @param x0,y0 top left corner
 * @param x1,y1 down right corner
 * @param color : 0 white, 1 black
 */    
void BD_rect(int x0, int y0, int x1, int y1, int color);

/** draw a filled rect
 *
 * @param x0,y0 top left corner
 * @param x1,y1 down right corner
 * @param color : 0 white, 1 black
 */  
void BD_fillrect(int x0, int y0, int x1, int y1, int color);

/** draw a circle
 *
 * @param x0,y0 center
 * @param r radius
 * @param color : 0 white, 1 black                                                          
 */    
void BD_circle(int x0, int y0, int r, int color);

/** draw a filled circle
 *
 * @param x0,y0 center
 * @param r radius
 * @param color : 0 white, 1 black
 */  
void BD_fillcircle(int x0, int y0, int r, int color);

/** set drawing mode
 *  @param NORMAL : paint normal color, XOR : paint XOR of current pixels
 */
void BD_setmode(int mode);

/** setup cursor position
 *
 * @param x x-position (top left)
 * @param y y-position 
 */   
void BD_locate(int x, int y);

/** calculate the max number of char in a line
 *
 * @returns max columns
 * depends on actual font size
 */ 
int BD_columns(void);

/** calculate the max number of columns
 *
 * @returns max column
 * depends on actual font size
 */   
int BD_rows(void);

/** put a char on the screen
 *
 * @param value char to print
 * @returns printed char
 */
int BD_putc(char value);

/** put a string on the screen
 *
 * @param value pointer to string to print
 */
void BD_puts(const char * value);

/** paint a character on given position out of the active font to the screen buffer
 *
 * @param x x-position (top left)
 * @param y y-position 
 * @param c char code
 */
void BD_character(int x, int y, int c);

/** select the font to use
 *
 * @param f pointer to font array 
 *                                                                              
 *   font array can created with GLCD Font Creator from http://www.mikroe.com
 *   you have to add 4 parameter at the beginning of the font array to use: 
 *   - the number of byte / char
 *   - the vertial size in pixel
 *   - the horizontal size in pixel
 *   - the number of byte per vertical line
 *   you also have to change the array to char[]
 */  
void BD_set_font(const unsigned char* f, const unsigned short* f_conf);

/** print bitmap to buffer
  *
  * @param bm struct Bitmap in flash
  * @param x  x start
  * @param y  y start 
  *
  */
void BD_print_bm(struct Bitmap bm, int x, int y);

/** Clip a buffer inside the main buffer
 * 
 * @param image buffer to clip
 * @param x x position of the input buffer
 * @param y y position of the input buffer
 * @param width width of the input buffer
 * @param height height of the input buffer
 *
 */
void BD_clip_image(const unsigned char * image, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

 #endif /* BUFFER_DRAW_H */
