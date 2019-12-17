/* -------------------------------------------------------------------------------- */
/* -- �GUI - Generic GUI module (C)Achim D�bler, 2015                            -- */
/* -------------------------------------------------------------------------------- */
// �GUI is a generic GUI module for embedded systems.
// This is a free software that is open for education, research and commercial
// developments under license policy of following terms.
//
//  Copyright (C) 2015, Achim D�bler, all rights reserved.
//  URL: http://www.embeddedlightning.com/
//
// * The �GUI module is a free software and there is NO WARRANTY.
// * No restriction on use. You can use, modify and redistribute it for
//   personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
// * Redistributions of source code must retain the above copyright notice.
//
/* -------------------------------------------------------------------------------- */
/* -- REVISION HISTORY                                                           -- */
/* -------------------------------------------------------------------------------- */
//  Mar 18, 2015  V0.3  Driver support added.
//                      Window and object support added.
//                      Touch support added.
//                      Fixed some minor bugs.
//
//  Oct 20, 2014  V0.2  Function UG_DrawRoundFrame() added.
//                      Function UG_FillRoundFrame() added.
//                      Function UG_DrawArc() added.
//                      Fixed some minor bugs.
//
//  Oct 11, 2014  V0.1  First release.
/* -------------------------------------------------------------------------------- */
#include "ugui.h"

/* Static functions */
 UG_RESULT _UG_WindowDrawTitle( UG_WINDOW* wnd );
 void _UG_WindowUpdate( UG_WINDOW* wnd );
 UG_RESULT _UG_WindowClear( UG_WINDOW* wnd );
 void _UG_TextboxUpdate(UG_WINDOW* wnd, UG_OBJECT* obj);
 void _UG_ButtonUpdate(UG_WINDOW* wnd, UG_OBJECT* obj);
 void _UG_ImageUpdate(UG_WINDOW* wnd, UG_OBJECT* obj);

 /* Pointer to the gui */
static UG_GUI* gui;

UG_S16 UG_Init( UG_GUI* g, void (*p)(UG_S16,UG_S16,UG_COLOR), UG_S16 x, UG_S16 y )
{
   UG_U8 i;

   g->pset = (void(*)(UG_S16,UG_S16,UG_COLOR))p;
   g->x_dim = x;
   g->y_dim = y;
   g->console.x_start = 4;
   g->console.y_start = 4;
   g->console.x_end = g->x_dim - g->console.x_start-1;
   g->console.y_end = g->y_dim - g->console.x_start-1;
   g->console.x_pos = g->console.x_end;
   g->console.y_pos = g->console.y_end;
   g->font.char_h_space = 1;
   g->font.char_v_space = 1;
   g->font.p = NULL;
   g->desktop_color = 0x5E8BEf;
   g->fore_color = C_WHITE;
   g->back_color = C_BLACK;
   g->next_window = NULL;
   g->active_window = NULL;
   g->last_window = NULL;

   /* Clear drivers */
   for(i=0;i<NUMBER_OF_DRIVERS;i++)
   {
      g->driver[i].driver = NULL;
      g->driver[i].state = 0;
   }

   gui = g;
   return 1;
}

UG_S16 UG_SelectGUI( UG_GUI* g )
{
   gui = g;
   return 1;
}

void UG_FontSelect( const UG_FONT* font )
{
   gui->font.p = font->p;
   gui->font.char_width = font->char_width;
   gui->font.char_height = font->char_height;
}

void UG_FillScreen( UG_COLOR c )
{
   UG_FillFrame(0,0,gui->x_dim-1,gui->y_dim-1,c);
}

void UG_FillFrame( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c )
{
   UG_S16 n,m;

   if ( x2 < x1 )
   {
      n = x2;
      x2 = x1;
      x1 = n;
   }
   if ( y2 < y1 )
   {
      n = y2;
      y2 = y1;
      y1 = n;
   }

   /* Is hardware acceleration available? */
   if ( gui->driver[DRIVER_FILL_FRAME].state & DRIVER_ENABLED )
   {
      if( ((UG_RESULT(*)(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c))gui->driver[DRIVER_FILL_FRAME].driver)(x1,y1,x2,y2,c) == UG_RESULT_OK ) return;
   }

   for( m=y1; m<=y2; m++ )
   {
      for( n=x1; n<=x2; n++ )
      {
         gui->pset(n,m,c);
      }
   }
}

void UG_FillRoundFrame( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_S16 r, UG_COLOR c )
{
   UG_S16  x,y,xd;

   if ( x2 < x1 )
   {
      x = x2;
      x2 = x1;
      x1 = x;
   }
   if ( y2 < y1 )
   {
      y = y2;
      y2 = y1;
      y1 = y;
   }

   if ( r<=0 ) return;

   xd = 3 - (r << 1);
   x = 0;
   y = r;

   UG_FillFrame(x1 + r, y1, x2 - r, y2, c);

   while ( x <= y )
   {
     if( y > 0 )
     {
        UG_DrawLine(x2 + x - r, y1 - y + r, x2+ x - r, y + y2 - r, c);
        UG_DrawLine(x1 - x + r, y1 - y + r, x1- x + r, y + y2 - r, c);
     }
     if( x > 0 )
     {
        UG_DrawLine(x1 - y + r, y1 - x + r, x1 - y + r, x + y2 - r, c);
        UG_DrawLine(x2 + y - r, y1 - x + r, x2 + y - r, x + y2 - r, c);
     }
     if ( xd < 0 )
     {
        xd += (x << 2) + 6;
     }
     else
     {
        xd += ((x - y) << 2) + 10;
        y--;
     }
     x++;
   }
}

void UG_DrawMesh( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c )
{
   UG_S16 n,m;

   if ( x2 < x1 )
   {
      n = x2;
      x2 = x1;
      x1 = n;
   }
   if ( y2 < y1 )
   {
      n = y2;
      y2 = y1;
      y1 = n;
   }

   for( m=y1; m<=y2; m+=2 )
   {
      for( n=x1; n<=x2; n+=2 )
      {
         gui->pset(n,m,c);
      }
   }
}

void UG_DrawFrame( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c )
{
   UG_DrawLine(x1,y1,x2,y1,c);
   UG_DrawLine(x1,y2,x2,y2,c);
   UG_DrawLine(x1,y1,x1,y2,c);
   UG_DrawLine(x2,y1,x2,y2,c);
}

void UG_DrawRoundFrame( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_S16 r, UG_COLOR c )
{
   UG_S16 n;
   if ( x2 < x1 )
   {
      n = x2;
      x2 = x1;
      x1 = n;
   }
   if ( y2 < y1 )
   {
      n = y2;
      y2 = y1;
      y1 = n;
   }

   if ( r > x2 ) return;
   if ( r > y2 ) return;

   UG_DrawLine(x1+r, y1, x2-r, y1, c);
   UG_DrawLine(x1+r, y2, x2-r, y2, c);
   UG_DrawLine(x1, y1+r, x1, y2-r, c);
   UG_DrawLine(x2, y1+r, x2, y2-r, c);
   UG_DrawArc(x1+r, y1+r, r, 0x0C, c);
   UG_DrawArc(x2-r, y1+r, r, 0x03, c);
   UG_DrawArc(x1+r, y2-r, r, 0x30, c);
   UG_DrawArc(x2-r, y2-r, r, 0xC0, c);
}

void UG_DrawPixel( UG_S16 x0, UG_S16 y0, UG_COLOR c )
{
   gui->pset(x0,y0,c);
}

void UG_DrawCircle( UG_S16 x0, UG_S16 y0, UG_S16 r, UG_COLOR c )
{
   UG_S16 x,y,xd,yd,e;

   if ( x0<0 ) return;
   if ( y0<0 ) return;
   if ( r<=0 ) return;

   xd = 1 - (r << 1);
   yd = 0;
   e = 0;
   x = r;
   y = 0;

   while ( x >= y )
   {
      gui->pset(x0 - x, y0 + y, c);
      gui->pset(x0 - x, y0 - y, c);
      gui->pset(x0 + x, y0 + y, c);
      gui->pset(x0 + x, y0 - y, c);
      gui->pset(x0 - y, y0 + x, c);
      gui->pset(x0 - y, y0 - x, c);
      gui->pset(x0 + y, y0 + x, c);
      gui->pset(x0 + y, y0 - x, c);

      y++;
      e += yd;
      yd += 2;
      if ( ((e << 1) + xd) > 0 )
      {
         x--;
         e += xd;
         xd += 2;
      }
   }
}

void UG_FillCircle( UG_S16 x0, UG_S16 y0, UG_S16 r, UG_COLOR c )
{
   UG_S16  x,y,xd;

   if ( x0<0 ) return;
   if ( y0<0 ) return;
   if ( r<=0 ) return;

   xd = 3 - (r << 1);
   x = 0;
   y = r;

   while ( x <= y )
   {
     if( y > 0 )
     {
        UG_DrawLine(x0 - x, y0 - y,x0 - x, y0 + y, c);
        UG_DrawLine(x0 + x, y0 - y,x0 + x, y0 + y, c);
     }
     if( x > 0 )
     {
        UG_DrawLine(x0 - y, y0 - x,x0 - y, y0 + x, c);
        UG_DrawLine(x0 + y, y0 - x,x0 + y, y0 + x, c);
     }
     if ( xd < 0 )
     {
        xd += (x << 2) + 6;
     }
     else
     {
        xd += ((x - y) << 2) + 10;
        y--;
     }
     x++;
   }
   UG_DrawCircle(x0, y0, r,c);
}

void UG_DrawArc( UG_S16 x0, UG_S16 y0, UG_S16 r, UG_U8 s, UG_COLOR c )
{
   UG_S16 x,y,xd,yd,e;

   if ( x0<0 ) return;
   if ( y0<0 ) return;
   if ( r<=0 ) return;

   xd = 1 - (r << 1);
   yd = 0;
   e = 0;
   x = r;
   y = 0;

   while ( x >= y )
   {
      // Q1
      if ( s & 0x01 ) gui->pset(x0 + x, y0 - y, c);
      if ( s & 0x02 ) gui->pset(x0 + y, y0 - x, c);

      // Q2
      if ( s & 0x04 ) gui->pset(x0 - y, y0 - x, c);
      if ( s & 0x08 ) gui->pset(x0 - x, y0 - y, c);

      // Q3
      if ( s & 0x10 ) gui->pset(x0 - x, y0 + y, c);
      if ( s & 0x20 ) gui->pset(x0 - y, y0 + x, c);

      // Q4
      if ( s & 0x40 ) gui->pset(x0 + y, y0 + x, c);
      if ( s & 0x80 ) gui->pset(x0 + x, y0 + y, c);

      y++;
      e += yd;
      yd += 2;
      if ( ((e << 1) + xd) > 0 )
      {
         x--;
         e += xd;
         xd += 2;
      }
   }
}

void UG_DrawLine( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c )
{
   UG_S16 n, dx, dy, sgndx, sgndy, dxabs, dyabs, x, y, drawx, drawy;

   if ( x2 < x1 )
   {
      n = x2;
      x2 = x1;
      x1 = n;
   }
   if ( y2 < y1 )
   {
      n = y2;
      y2 = y1;
      y1 = n;
   }

   /* Is hardware acceleration available? */
   if ( gui->driver[DRIVER_DRAW_LINE].state & DRIVER_ENABLED )
   {
      if( ((UG_RESULT(*)(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c))gui->driver[DRIVER_DRAW_LINE].driver)(x1,y1,x2,y2,c) == UG_RESULT_OK ) return;
   }

   dx = x2 - x1;
   dy = y2 - y1;
   dxabs = (dx>0)?dx:-dx;
   dyabs = (dy>0)?dy:-dy;
   sgndx = (dx>0)?1:-1;
   sgndy = (dy>0)?1:-1;
   x = dyabs >> 1;
   y = dxabs >> 1;
   drawx = x1;
   drawy = y1;

   gui->pset(drawx, drawy,c);

   if( dxabs >= dyabs )
   {
      for( n=0; n<dxabs; n++ )
      {
         y += dyabs;
         if( y >= dxabs )
         {
            y -= dxabs;
            drawy += sgndy;
         }
         drawx += sgndx;
         gui->pset(drawx, drawy,c);
      }
   }
   else
   {
      for( n=0; n<dyabs; n++ )
      {
         x += dxabs;
         if( x >= dyabs )
         {
            x -= dyabs;
            drawx += sgndx;
         }
         drawy += sgndy;
         gui->pset(drawx, drawy,c);
      }
   }
}

void UG_PutString( UG_S16 x, UG_S16 y, char* str )
{
   UG_S16 xp,yp;
   char chr;

   xp=x;
   yp=y;

   while ( *str != 0 )
   {
      chr = *str;
      if ( chr == '\n' )
      {
         xp = gui->x_dim;
         str++;
         continue;
      }

      if ( xp+gui->font.char_width > gui->x_dim - 1 )
      {
         xp = x;
         yp += gui->font.char_height+gui->font.char_v_space;
      }

      UG_PutChar(chr, xp, yp, gui->fore_color, gui->back_color);

      xp += gui->font.char_width+gui->font.char_h_space;
      str++;
   }
}

void UG_PutChar( char chr, UG_S16 x, UG_S16 y, UG_COLOR fc, UG_COLOR bc )
{
   UG_U16 i,j,k,xo,yo,c,bn;
   UG_U8 b,bt;
   unsigned char* p;

   bt = (UG_U8)chr;

   switch ( bt )
   {
      case 0xF6: bt = 0x94; break; // �
      case 0xD6: bt = 0x99; break; // �
      case 0xFC: bt = 0x81; break; // �
      case 0xDC: bt = 0x9A; break; // �
      case 0xE4: bt = 0x84; break; // �
      case 0xC4: bt = 0x8E; break; // �
      case 0xB5: bt = 0xE6; break; // �
      case 0xB0: bt = 0xF8; break; // �
   }

   yo = y;
   bn = gui->font.char_width;
   if ( !bn ) return;
   bn >>= 3;
   if ( gui->font.char_width % 8 ) bn++;
   p = gui->font.p;
   p+= bt * gui->font.char_height * bn;

   for( j=0;j<gui->font.char_height;j++ )
   {
      xo = x;
      c=gui->font.char_width;
      for( i=0;i<bn;i++ )
      {
         b = *p++;
         for( k=0;(k<8) && c;k++ )
         {
            if( b & 0x01 )
            {
               gui->pset(xo,yo,fc);
            }
            else
            {
               gui->pset(xo,yo,bc);
            }
            b >>= 1;
            xo++;
            c--;
         }
      }
      yo++;
   }
}

void UG_ConsolePutString( char* str )
{
   char chr;

   while ( *str != 0 )
   {
      chr = *str;
      if ( chr == '\n' )
      {
         gui->console.x_pos = gui->x_dim;
         str++;
         continue;
      }

      gui->console.x_pos += gui->font.char_width+gui->font.char_h_space;

      if ( gui->console.x_pos+gui->font.char_width > gui->console.x_end )
      {
         gui->console.x_pos = gui->console.x_start;
         gui->console.y_pos += gui->font.char_height+gui->font.char_v_space;
      }
      if ( gui->console.y_pos+gui->font.char_height > gui->console.y_end )
      {
         gui->console.x_pos = gui->console.x_start;
         gui->console.y_pos = gui->console.y_start;
         UG_FillFrame(gui->console.x_start,gui->console.y_start,gui->console.x_end,gui->console.y_end,gui->console.back_color);
      }

      UG_PutChar(chr, gui->console.x_pos, gui->console.y_pos, gui->console.fore_color, gui->console.back_color);
      str++;
   }
}

void UG_ConsoleSetArea( UG_S16 xs, UG_S16 ys, UG_S16 xe, UG_S16 ye )
{
   gui->console.x_start = xs;
   gui->console.y_start = ys;
   gui->console.x_end = xe;
   gui->console.y_end = ye;
}

void UG_ConsoleSetForecolor( UG_COLOR c )
{
   gui->console.fore_color = c;
}

void UG_ConsoleSetBackcolor( UG_COLOR c )
{
   gui->console.back_color = c;
}

void UG_SetForecolor( UG_COLOR c )
{
   gui->fore_color = c;
}

void UG_SetBackcolor( UG_COLOR c )
{
   gui->back_color = c;
}

UG_S16 UG_GetXDim( void )
{
   return gui->x_dim;
}

UG_S16 UG_GetYDim( void )
{
   return gui->y_dim;
}

void UG_FontSetHSpace( UG_U16 s )
{
   gui->font.char_h_space = s;
}

void UG_FontSetVSpace( UG_U16 s )
{
   gui->font.char_v_space = s;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const UG_COLOR pal_window[] =
{
   /* Frame 0 */
   0x646464,
   0x646464,
   0x646464,
   0x646464,
   /* Frame 1 */
   0xFFFFFF,
   0xFFFFFF,
   0x696969,
   0x696969,
   /* Frame 2 */
   0xE3E3E3,
   0xE3E3E3,
   0xA0A0A0,
   0xA0A0A0,
};

const UG_COLOR pal_button_pressed[] =
{
   /* Frame 0 */
   0x646464,
   0x646464,
   0x646464,
   0x646464,
   /* Frame 1 */
   0xA0A0A0,
   0xA0A0A0,
   0xA0A0A0,
   0xA0A0A0,
   /* Frame 2 */
   0xF0F0F0,
   0xF0F0F0,
   0xF0F0F0,
   0xF0F0F0,
};

const UG_COLOR pal_button_released[] =
{
   /* Frame 0 */
   0x646464,
   0x646464,
   0x646464,
   0x646464,
   /* Frame 1 */
   0xFFFFFF,
   0xFFFFFF,
   0x696969,
   0x696969,
   /* Frame 2 */
   0xE3E3E3,
   0xE3E3E3,
   0xA0A0A0,
   0xA0A0A0,
};
/* -------------------------------------------------------------------------------- */
/* -- INTERNAL FUNCTIONS                                                         -- */
/* -------------------------------------------------------------------------------- */
void _UG_PutText(UG_TEXT* txt)
{
   UG_U16 sl,rc;
   UG_S16 xp,yp;
   UG_S16 xs=txt->a.xs;
   UG_S16 ys=txt->a.ys;
   UG_S16 xe=txt->a.xe;
   UG_S16 ye=txt->a.ye;
   UG_U8  align=txt->align;
   UG_S16 char_width=txt->font->char_width;
   UG_S16 char_height=txt->font->char_height;
   UG_S16 char_h_space=txt->h_space;
   UG_S16 char_v_space=txt->v_space;
   UG_U16 i,j,k,xo,yo,cw,bn;
   UG_U8  b,bt;

   unsigned char* p;

   char* str = txt->str;
   char* c = str;

   if ( txt->font->p == NULL ) return;
   if ( str == NULL ) return;
   if ( (ye - ys) < txt->font->char_height ) return;

   rc=1;
   c=str;
   while ( *c != 0 )
   {
      if ( *c == '\n' ) rc++;
      c++;
   }

   yp = 0;
   if ( align & (ALIGN_V_CENTER | ALIGN_V_BOTTOM) )
   {
      yp = ye - ys + 1;
      yp -= char_height*rc;
      yp -= char_v_space*(rc-1);
      if ( yp < 0 ) return;
   }
   if ( align & ALIGN_V_CENTER ) yp >>= 1;
   yp += ys;

   while( 1 )
   {
      sl=0;
      c=str;
      while( (*c != 0) && (*c != '\n') )
      {
         c++;
         sl++;
      }

      xp = xe - xs + 1;
      xp -= char_width*sl;
      xp -= char_h_space*(sl-1);
      if ( xp < 0 ) return;

      if ( align & ALIGN_H_LEFT ) xp = 0;
      else if ( align & ALIGN_H_CENTER ) xp >>= 1;
      xp += xs;

      while( (*str != '\n') )
      {
         if ( *str == 0 ) return;
         /*----------------------------------*/
         /* Draw one char                    */
         /*----------------------------------*/
         bt = (UG_U8)*str;
         switch ( bt )
         {
            case 0xF6: bt = 0x94; break; // �
            case 0xD6: bt = 0x99; break; // �
            case 0xFC: bt = 0x81; break; // �
            case 0xDC: bt = 0x9A; break; // �
            case 0xE4: bt = 0x84; break; // �
            case 0xC4: bt = 0x8E; break; // �
            case 0xB5: bt = 0xE6; break; // �
            case 0xB0: bt = 0xF8; break; // �
         }
         yo = yp;
         bn = char_width;
         bn >>= 3;
         if ( char_width % 8 ) bn++;
         p = txt->font->p;
         p+= bt * char_height * bn;
         for( j=0;j<char_height;j++ )
         {
            xo = xp;
            cw=char_width;
            for( i=0;i<bn;i++ )
            {
               b = *p++;
               for( k=0;(k<8) && cw;k++ )
               {
                  if( b & 0x01 )
                  {
                     gui->pset(xo,yo,txt->fc);
                  }
                  else
                  {
                     gui->pset(xo,yo,txt->bc);
                  }
                  b >>= 1;
                  xo++;
                  cw--;
               }
            }
            yo++;
         }
         /*----------------------------------*/
         xp += char_width + char_h_space;
         str++;
      }
      str++;
      yp += char_height + char_v_space;
   }
}

UG_OBJECT* _UG_GetFreeObject( UG_WINDOW* wnd )
{
   UG_U8 i;
   UG_OBJECT* obj=(UG_OBJECT*)wnd->objlst;

   for(i=0;i<wnd->objcnt;i++)
   {
      obj = (UG_OBJECT*)(&wnd->objlst[i]);
      if ( (obj->state & OBJ_STATE_FREE) && (obj->state & OBJ_STATE_VALID) )
      {
         /* Free object found! */
         return obj;
      }
   }
   return NULL;
}

UG_OBJECT* _UG_SearchObject( UG_WINDOW* wnd, UG_U8 type, UG_U8 id )
{
   UG_U8 i;
   UG_OBJECT* obj=(UG_OBJECT*)wnd->objlst;

   for(i=0;i<wnd->objcnt;i++)
   {
      obj = (UG_OBJECT*)(&wnd->objlst[i]);
      if ( !(obj->state & OBJ_STATE_FREE) && (obj->state & OBJ_STATE_VALID) )
      {
         if ( (obj->type == type) && (obj->id == id) )
         {
            /* Requested object found! */
            return obj;
         }
      }
   }
   return NULL;
}

UG_RESULT _UG_DeleteObject( UG_WINDOW* wnd, UG_U8 type, UG_U8 id )
{
   UG_OBJECT* obj=NULL;

   obj = _UG_SearchObject( wnd, type, id );

   /* Object found? */
   if ( obj != NULL )
   {
      /* We dont't want to delete a visible or busy object! */
      if ( (obj->state & OBJ_STATE_VISIBLE) || (obj->state & OBJ_STATE_UPDATE) ) return UG_RESULT_FAIL;
      obj->state = OBJ_STATE_INIT;
      obj->data = NULL;
      obj->event = 0;
      obj->id = 0;
      obj->touch_state = 0;
      obj->type = 0;
      obj->update = NULL;
      return UG_RESULT_OK;
   }
   return UG_RESULT_FAIL;
}

void _UG_ProcessTouchData( UG_WINDOW* wnd )
{
   UG_S16 xp,yp;
   UG_U16 i,objcnt;
   UG_OBJECT* obj;
   UG_U8 objstate;
   UG_U8 objtouch;
   UG_U8 tchstate;

   xp = gui->touch.xp;
   yp = gui->touch.yp;
   tchstate = gui->touch.state;

   objcnt = wnd->objcnt;
   for(i=0; i<objcnt; i++)
   {
      obj = (UG_OBJECT*)&wnd->objlst[i];
      objstate = obj->state;
      objtouch = obj->touch_state;
      if ( !(objstate & OBJ_STATE_FREE) && (objstate & OBJ_STATE_VALID) && (objstate & OBJ_STATE_VISIBLE) && !(objstate & OBJ_STATE_REDRAW))
      {
         /* Process touch data */
         if ( (tchstate) && xp != -1 )
         {
            if ( !(objtouch & OBJ_TOUCH_STATE_IS_PRESSED) )
            {
               objtouch |= OBJ_TOUCH_STATE_PRESSED_OUTSIDE_OBJECT | OBJ_TOUCH_STATE_CHANGED;
               objtouch &= ~(OBJ_TOUCH_STATE_RELEASED_ON_OBJECT | OBJ_TOUCH_STATE_RELEASED_OUTSIDE_OBJECT | OBJ_TOUCH_STATE_CLICK_ON_OBJECT);
            }
            objtouch &= ~OBJ_TOUCH_STATE_IS_PRESSED_ON_OBJECT;
            if ( xp >= obj->a_abs.xs )
            {
               if ( xp <= obj->a_abs.xe )
               {
                  if ( yp >= obj->a_abs.ys )
                  {
                     if ( yp <= obj->a_abs.ye )
                     {
                        objtouch |= OBJ_TOUCH_STATE_IS_PRESSED_ON_OBJECT;
                        if ( !(objtouch & OBJ_TOUCH_STATE_IS_PRESSED) )
                        {
                           objtouch &= ~OBJ_TOUCH_STATE_PRESSED_OUTSIDE_OBJECT;
                           objtouch |= OBJ_TOUCH_STATE_PRESSED_ON_OBJECT;
                        }
                     }
                  }
               }
            }
            objtouch |= OBJ_TOUCH_STATE_IS_PRESSED;
         }
         else if ( objtouch & OBJ_TOUCH_STATE_IS_PRESSED )
         {
            if ( objtouch & OBJ_TOUCH_STATE_IS_PRESSED_ON_OBJECT )
            {
               if ( objtouch & OBJ_TOUCH_STATE_PRESSED_ON_OBJECT ) objtouch |= OBJ_TOUCH_STATE_CLICK_ON_OBJECT;
               objtouch |= OBJ_TOUCH_STATE_RELEASED_ON_OBJECT;
            }
            else
            {
               objtouch |= OBJ_TOUCH_STATE_RELEASED_OUTSIDE_OBJECT;
            }
            if ( objtouch & OBJ_TOUCH_STATE_IS_PRESSED )
            {
               objtouch |= OBJ_TOUCH_STATE_CHANGED;
            }
            objtouch &= ~(OBJ_TOUCH_STATE_PRESSED_OUTSIDE_OBJECT | OBJ_TOUCH_STATE_PRESSED_ON_OBJECT | OBJ_TOUCH_STATE_IS_PRESSED);
         }
      }
      obj->touch_state = objtouch;
   }
}

void _UG_UpdateObjects( UG_WINDOW* wnd )
{
   UG_U16 i,objcnt;
   UG_OBJECT* obj;
   UG_U8 objstate;
   UG_U8 objtouch;

   /* Check each object, if it needs to be updated? */
   objcnt = wnd->objcnt;
   for(i=0; i<objcnt; i++)
   {
      obj = (UG_OBJECT*)&wnd->objlst[i];
      objstate = obj->state;
      objtouch = obj->touch_state;
      if ( !(objstate & OBJ_STATE_FREE) && (objstate & OBJ_STATE_VALID) )
      {
         if ( objstate & OBJ_STATE_UPDATE )
         {
            obj->update(wnd,obj);
         }
         if ( (objstate & OBJ_STATE_VISIBLE) && (objstate & OBJ_STATE_TOUCH_ENABLE) )
         {
            if ( (objtouch & (OBJ_TOUCH_STATE_CHANGED | OBJ_TOUCH_STATE_IS_PRESSED)) )
            {
               obj->update(wnd,obj);
            }
         }
      }
   }
}

void _UG_HandleEvents( UG_WINDOW* wnd )
{
   UG_U16 i,objcnt;
   UG_OBJECT* obj;
   UG_U8 objstate;
   static UG_MESSAGE msg;
   msg.src = NULL;

   /* Handle window-related events */
   //ToDo


   /* Handle object-related events */
   msg.type = MSG_TYPE_OBJECT;
   objcnt = wnd->objcnt;
   for(i=0; i<objcnt; i++)
   {
      obj = (UG_OBJECT*)&wnd->objlst[i];
      objstate = obj->state;
      if ( !(objstate & OBJ_STATE_FREE) && (objstate & OBJ_STATE_VALID) )
      {
         if ( obj->event != OBJ_EVENT_NONE )
         {
            msg.src = &obj;
            msg.id = obj->type;
            msg.sub_id = obj->id;

            wnd->cb( &msg );

            obj->event = OBJ_EVENT_NONE;
         }
      }
   }
}

void _UG_DrawObjectFrame( UG_S16 xs, UG_S16 ys, UG_S16 xe, UG_S16 ye, UG_COLOR* p )
{
   // Frame 0
   UG_DrawLine(xs, ys  , xe-1, ys  , *p++);
   UG_DrawLine(xs, ys+1, xs  , ye-1, *p++);
   UG_DrawLine(xs, ye  , xe  , ye  , *p++);
   UG_DrawLine(xe, ys  , xe  , ye-1, *p++);
   // Frame 1
   UG_DrawLine(xs+1, ys+1, xe-2, ys+1, *p++);
   UG_DrawLine(xs+1, ys+2, xs+1, ye-2, *p++);
   UG_DrawLine(xs+1, ye-1, xe-1, ye-1, *p++);
   UG_DrawLine(xe-1, ys+1, xe-1, ye-2, *p++);
   // Frame 2
   UG_DrawLine(xs+2, ys+2, xe-3, ys+2, *p++);
   UG_DrawLine(xs+2, ys+3, xs+2, ye-3, *p++);
   UG_DrawLine(xs+2, ye-2, xe-2, ye-2, *p++);
   UG_DrawLine(xe-2, ys+2, xe-2, ye-3, *p);
}

/* -------------------------------------------------------------------------------- */
/* -- DRIVER FUNCTIONS                                                           -- */
/* -------------------------------------------------------------------------------- */
void UG_DriverRegister( UG_U8 type, void* driver )
{
   if ( type >= NUMBER_OF_DRIVERS ) return;

   gui->driver[type].driver = driver;
   gui->driver[type].state = DRIVER_REGISTERED | DRIVER_ENABLED;
}

void UG_DriverEnable( UG_U8 type )
{
   if ( type >= NUMBER_OF_DRIVERS ) return;
   if ( gui->driver[type].state & DRIVER_REGISTERED )
   {
      gui->driver[type].state |= DRIVER_ENABLED;
   }
}

void UG_DriverDisable( UG_U8 type )
{
   if ( type >= NUMBER_OF_DRIVERS ) return;
   if ( gui->driver[type].state & DRIVER_REGISTERED )
   {
      gui->driver[type].state &= ~DRIVER_ENABLED;
   }
}

/* -------------------------------------------------------------------------------- */
/* -- MISCELLANEOUS FUNCTIONS                                                    -- */
/* -------------------------------------------------------------------------------- */
void UG_Update( void )
{
   UG_WINDOW* wnd;

   /* Is somebody waiting for this update? */
   if ( gui->state & UG_SATUS_WAIT_FOR_UPDATE ) gui->state &= ~UG_SATUS_WAIT_FOR_UPDATE;

   /* Keep track of the windows */
   if ( gui->next_window != gui->active_window )
   {
      if ( gui->next_window != NULL )
      {
         gui->last_window = gui->active_window;
         gui->active_window = gui->next_window;

         /* Do we need to draw an inactive title? */
         if ( (gui->last_window!=NULL) && (gui->last_window->style & WND_STYLE_SHOW_TITLE) && (gui->last_window->state & WND_STATE_VISIBLE) )
         {
            /* Do both windows differ in size */
            if ( (gui->last_window->xs != gui->active_window->xs) || (gui->last_window->xe != gui->active_window->xe) || (gui->last_window->ys != gui->active_window->ys) || (gui->last_window->ye != gui->active_window->ye) )
            {
               /* Redraw title of the last window */
               _UG_WindowDrawTitle( gui->last_window );
            }
         }
         gui->active_window->state &= ~WND_STATE_REDRAW_TITLE;
         gui->active_window->state |= WND_STATE_UPDATE | WND_STATE_VISIBLE;
      }
   }

   /* Is there an active window */
   if ( gui->active_window != NULL )
   {
      wnd = gui->active_window;

      /* Does the window need to be updated? */
      if ( wnd->state & WND_STATE_UPDATE )
      {
         /* Do it! */
         _UG_WindowUpdate( wnd );
      }

      /* Is the window visible? */
      if ( wnd->state & WND_STATE_VISIBLE )
      {
         _UG_ProcessTouchData( wnd );
         _UG_UpdateObjects( wnd );
         _UG_HandleEvents( wnd );
      }
   }
}

void UG_WaitForUpdate( void )
{
   gui->state |= UG_SATUS_WAIT_FOR_UPDATE;
   while ( (volatile UG_U8)gui->state & UG_SATUS_WAIT_FOR_UPDATE ){};
}

void UG_DrawBMP( UG_S16 xp, UG_S16 yp, UG_BMP* bmp )
{
   UG_S16 x,y,xs;
   UG_U8 r,g,b;
   UG_U16* p;
   UG_U16 tmp;
   UG_COLOR c;

   if ( bmp->p == NULL ) return;

   /* Only support 16 BPP so far */
   if ( bmp->bpp == BMP_BPP_16 )
   {
      p = (UG_U16*)bmp->p;
   }
   else
   {
      return;
   }

   xs = xp;
   for(y=0;y<bmp->height;y++)
   {
      xp = xs;
      for(x=0;x<bmp->width;x++)
      {
         tmp = *p++;
         /* Convert RGB565 to RGB888 */
         r = (tmp>>11)&0x1F;
         r<<=3;
         g = (tmp>>5)&0x3F;
         g<<=2;
         b = (tmp)&0x1F;
         b<<=3;
         c = ((UG_COLOR)r<<16) | ((UG_COLOR)g<<8) | (UG_COLOR)b;
         UG_DrawPixel( xp++ , yp , c );
      }
      yp++;
   }
}

void UG_TouchUpdate( UG_S16 xp, UG_S16 yp, UG_U8 state )
{
   gui->touch.xp = xp;
   gui->touch.yp = yp;
   gui->touch.state = state;
}

/* -------------------------------------------------------------------------------- */
/* -- WINDOW FUNCTIONS                                                           -- */
/* -------------------------------------------------------------------------------- */
UG_RESULT UG_WindowCreate( UG_WINDOW* wnd, UG_OBJECT* objlst, UG_U8 objcnt, void (*cb)( UG_MESSAGE* ) )
{
   UG_U8 i;
   UG_OBJECT* obj=NULL;

   if ( (wnd == NULL) || (objlst == NULL) || (objcnt == 0) ) return UG_RESULT_FAIL;

   /* Initialize all objects of the window */
   for(i=0; i<objcnt; i++)
   {
      obj = (UG_OBJECT*)&objlst[i];
      obj->state = OBJ_STATE_INIT;
      obj->data = NULL;
   }

   /* Initialize window */
   wnd->objcnt = objcnt;
   wnd->objlst = objlst;
   wnd->state = WND_STATE_VALID;
   wnd->fc = 0x000000;
   wnd->bc = 0xF0F0F0;
   wnd->xs = 0;
   wnd->ys = 0;
   wnd->xe = UG_GetXDim()-1;
   wnd->ye = UG_GetYDim()-1;
   wnd->cb = cb;
   wnd->style = WND_STYLE_3D | WND_STYLE_SHOW_TITLE;

   /* Initialize window title-bar */
   wnd->title.str = NULL;
   wnd->title.font = NULL;
   wnd->title.h_space = 2;
   wnd->title.v_space = 2;
   wnd->title.align = ALIGN_CENTER_LEFT;
   wnd->title.fc = C_WHITE;
   wnd->title.bc = C_BLUE;
   wnd->title.ifc = C_WHITE;
   wnd->title.ibc = C_GRAY;
   wnd->title.height = 15;

   return UG_RESULT_OK;
}

UG_RESULT UG_WindowDelete( UG_WINDOW* wnd )
{
   if ( wnd == gui->active_window ) return UG_RESULT_FAIL;

   /* Only delete valid windows */
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      wnd->state = 0;
      wnd->cb = NULL;
      wnd->objcnt = 0;
      wnd->objlst = NULL;
      wnd->xs = 0;
      wnd->ys = 0;
      wnd->xe = 0;
      wnd->ye = 0;
      wnd->style = 0;
      return UG_RESULT_OK;
   }
   return UG_RESULT_FAIL;
}

UG_RESULT UG_WindowShow( UG_WINDOW* wnd )
{
   if ( wnd != NULL )
   {
      /* Force an update, even if this is the active window! */
      wnd->state |= WND_STATE_VISIBLE | WND_STATE_UPDATE;
      wnd->state &= ~WND_STATE_REDRAW_TITLE;
      gui->next_window = wnd;
      return UG_RESULT_OK;
   }
   return UG_RESULT_FAIL;
}

UG_RESULT UG_WindowHide( UG_WINDOW* wnd )
{
   if ( wnd != NULL )
   {
      if ( wnd == gui->active_window )
      {
         /* Is there an old window which just lost the focus? */
         if ( (gui->last_window != NULL) && (gui->last_window->state & WND_STATE_VISIBLE) )
         {
            if ( (gui->last_window->xs > wnd->xs) || (gui->last_window->ys > wnd->ys) || (gui->last_window->xe < wnd->xe) || (gui->last_window->ye < wnd->ye) )
            {
               _UG_WindowClear( wnd );
            }
            gui->next_window = gui->last_window;
         }
         else
         {
            gui->active_window->state &= ~WND_STATE_VISIBLE;
            gui->active_window->state |= WND_STATE_UPDATE;
         }
      }
      else
      {
         /* If the old window is visible, clear it! */
         _UG_WindowClear( wnd );
      }
      return UG_RESULT_OK;
   }
   return UG_RESULT_FAIL;
}

UG_RESULT UG_WindowResize( UG_WINDOW* wnd, UG_S16 xs, UG_S16 ys, UG_S16 xe, UG_S16 ye )
{
   UG_S16 pos;
   UG_S16 xmax,ymax;

   xmax = UG_GetXDim()-1;
   ymax = UG_GetYDim()-1;

   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      /* Do some checks... */
      if ( (xs < 0) || (ys < 0) ) return UG_RESULT_FAIL;
      if ( (xe > xmax) || (ye > ymax) ) return UG_RESULT_FAIL;
      pos = xe-xs;
      if ( pos < 10 ) return UG_RESULT_FAIL;
      pos = ye-ys;
      if ( pos < 10 ) return UG_RESULT_FAIL;

      /* ... and if everything is OK move the window! */
      wnd->xs = xs;
      wnd->ys = ys;
      wnd->xe = xe;
      wnd->ye = ye;

      if ( (wnd->state & WND_STATE_VISIBLE) && (gui->active_window == wnd) )
      {
         if ( wnd->ys ) UG_FillFrame(0, 0, xmax,wnd->ys-1,gui->desktop_color);
         pos = wnd->ye+1;
         if ( !(pos > ymax) ) UG_FillFrame(0, pos, xmax,ymax,gui->desktop_color);
         if ( wnd->xs ) UG_FillFrame(0, wnd->ys, wnd->xs-1,wnd->ye,gui->desktop_color);
         pos = wnd->xe+1;
         if ( !(pos > xmax) ) UG_FillFrame(pos, wnd->ys,xmax,wnd->ye,gui->desktop_color);

         wnd->state &= ~WND_STATE_REDRAW_TITLE;
         wnd->state |= WND_STATE_UPDATE;
      }
      return UG_RESULT_OK;
   }
   return UG_RESULT_FAIL;
}

UG_RESULT UG_WindowAlert( UG_WINDOW* wnd )
{
   UG_COLOR c;
   c = UG_WindowGetTitleTextColor( wnd );
   if ( UG_WindowSetTitleTextColor( wnd, UG_WindowGetTitleColor( wnd ) ) == UG_RESULT_FAIL ) return UG_RESULT_FAIL;
   if ( UG_WindowSetTitleColor( wnd, c ) == UG_RESULT_FAIL ) return UG_RESULT_FAIL;
   return UG_RESULT_OK;
}

UG_RESULT UG_WindowSetForeColor( UG_WINDOW* wnd, UG_COLOR fc )
{
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      wnd->fc = fc;
      wnd->state |= WND_STATE_UPDATE;
      return UG_RESULT_OK;
   }
   return UG_RESULT_FAIL;
}

UG_RESULT UG_WindowSetBackColor( UG_WINDOW* wnd, UG_COLOR bc )
{
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      wnd->bc = bc;
      wnd->state |= WND_STATE_UPDATE;
      return UG_RESULT_OK;
   }
   return UG_RESULT_FAIL;
}

UG_RESULT UG_WindowSetTitleTextColor( UG_WINDOW* wnd, UG_COLOR c )
{
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      wnd->title.fc = c;
      wnd->state |= WND_STATE_UPDATE | WND_STATE_REDRAW_TITLE;
      return UG_RESULT_OK;
   }
   return UG_RESULT_FAIL;
}

UG_RESULT UG_WindowSetTitleColor( UG_WINDOW* wnd, UG_COLOR c )
{
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      wnd->title.bc = c;
      wnd->state |= WND_STATE_UPDATE | WND_STATE_REDRAW_TITLE;
      return UG_RESULT_OK;
   }
   return UG_RESULT_FAIL;
}

UG_RESULT UG_WindowSetTitleInactiveTextColor( UG_WINDOW* wnd, UG_COLOR c )
{
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      wnd->title.ifc = c;
      wnd->state |= WND_STATE_UPDATE | WND_STATE_REDRAW_TITLE;
      return UG_RESULT_OK;
   }
   return UG_RESULT_FAIL;
}

UG_RESULT UG_WindowSetTitleInactiveColor( UG_WINDOW* wnd, UG_COLOR c )
{
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      wnd->title.ibc = c;
      wnd->state |= WND_STATE_UPDATE | WND_STATE_REDRAW_TITLE;
      return UG_RESULT_OK;
   }
   return UG_RESULT_FAIL;
}

UG_RESULT UG_WindowSetTitleText( UG_WINDOW* wnd, char* str )
{
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      wnd->title.str = str;
      wnd->state |= WND_STATE_UPDATE | WND_STATE_REDRAW_TITLE;
      return UG_RESULT_OK;
   }
   return UG_RESULT_FAIL;
}

UG_RESULT UG_WindowSetTitleTextFont( UG_WINDOW* wnd, const UG_FONT* font )
{
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      wnd->state |= WND_STATE_UPDATE | WND_STATE_REDRAW_TITLE;
      wnd->title.font = font;
      if ( wnd->title.height <= (font->char_height + 1) )
      {
         wnd->title.height = font->char_height + 2;
         wnd->state &= ~WND_STATE_REDRAW_TITLE;
      }
      return UG_RESULT_OK;
   }
   return UG_RESULT_FAIL;
}

UG_RESULT UG_WindowSetTitleTextHSpace( UG_WINDOW* wnd, UG_S8 hs )
{
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      wnd->title.h_space = hs;
      wnd->state |= WND_STATE_UPDATE | WND_STATE_REDRAW_TITLE;
      return UG_RESULT_OK;
   }
   return UG_RESULT_FAIL;
}

UG_RESULT UG_WindowSetTitleTextVSpace( UG_WINDOW* wnd, UG_S8 vs )
{
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      wnd->title.v_space = vs;
      wnd->state |= WND_STATE_UPDATE | WND_STATE_REDRAW_TITLE;
      return UG_RESULT_OK;
   }
   return UG_RESULT_FAIL;
}

UG_RESULT UG_WindowSetTitleTextAlignment( UG_WINDOW* wnd, UG_U8 align )
{
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      wnd->title.align = align;
      wnd->state |= WND_STATE_UPDATE | WND_STATE_REDRAW_TITLE;
      return UG_RESULT_OK;
   }
   return UG_RESULT_FAIL;
}

UG_RESULT UG_WindowSetTitleHeight( UG_WINDOW* wnd, UG_U8 height )
{
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      wnd->title.height = height;
      wnd->state &= ~WND_STATE_REDRAW_TITLE;
      wnd->state |= WND_STATE_UPDATE;
      return UG_RESULT_OK;
   }
   return UG_RESULT_FAIL;
}

UG_RESULT UG_WindowSetXStart( UG_WINDOW* wnd, UG_S16 xs )
{
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      wnd->xs = xs;
      if ( UG_WindowResize( wnd, wnd->xs, wnd->ys, wnd->xe, wnd->ye) == UG_RESULT_FAIL ) return UG_RESULT_FAIL;
      return UG_RESULT_OK;
   }
   return UG_RESULT_FAIL;
}

UG_RESULT UG_WindowSetYStart( UG_WINDOW* wnd, UG_S16 ys )
{
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      wnd->ys = ys;
      if ( UG_WindowResize( wnd, wnd->xs, wnd->ys, wnd->xe, wnd->ye) == UG_RESULT_FAIL ) return UG_RESULT_FAIL;
      return UG_RESULT_OK;
   }
   return UG_RESULT_FAIL;
}

UG_RESULT UG_WindowSetXEnd( UG_WINDOW* wnd, UG_S16 xe )
{
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      wnd->xe = xe;
      if ( UG_WindowResize( wnd, wnd->xs, wnd->ys, wnd->xe, wnd->ye) == UG_RESULT_FAIL ) return UG_RESULT_FAIL;
      return UG_RESULT_OK;
   }
   return UG_RESULT_FAIL;
}

UG_RESULT UG_WindowSetYEnd( UG_WINDOW* wnd, UG_S16 ye )
{
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      wnd->ye = ye;
      if ( UG_WindowResize( wnd, wnd->xs, wnd->ys, wnd->xe, wnd->ye) == UG_RESULT_FAIL ) return UG_RESULT_FAIL;
      return UG_RESULT_OK;
   }
   return UG_RESULT_FAIL;
}

UG_RESULT UG_WindowSetStyle( UG_WINDOW* wnd, UG_U8 style )
{
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      /* 3D or 2D? */
      if ( style & WND_STYLE_3D )
      {
         wnd->style |= WND_STYLE_3D;
      }
      else
      {
         wnd->style &= ~WND_STYLE_3D;
      }
      /* Show title-bar? */
      if ( style & WND_STYLE_SHOW_TITLE )
      {
         wnd->style |= WND_STYLE_SHOW_TITLE;
      }
      else
      {
         wnd->style &= ~WND_STYLE_SHOW_TITLE;
      }
      wnd->state |= WND_STATE_UPDATE;
      return UG_RESULT_OK;
   }
   return UG_RESULT_FAIL;
}

UG_COLOR UG_WindowGetForeColor( UG_WINDOW* wnd )
{
   UG_COLOR c = C_BLACK;
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      c = wnd->fc;
   }
   return c;
}

UG_COLOR UG_WindowGetBackColor( UG_WINDOW* wnd )
{
   UG_COLOR c = C_BLACK;
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      c = wnd->bc;
   }
   return c;
}

UG_COLOR UG_WindowGetTitleTextColor( UG_WINDOW* wnd )
{
   UG_COLOR c = C_BLACK;
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      c = wnd->title.fc;
   }
   return c;
}

UG_COLOR UG_WindowGetTitleColor( UG_WINDOW* wnd )
{
   UG_COLOR c = C_BLACK;
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      c = wnd->title.bc;
   }
   return c;
}

UG_COLOR UG_WindowGetTitleInactiveTextColor( UG_WINDOW* wnd )
{
   UG_COLOR c = C_BLACK;
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      c = wnd->title.ifc;
   }
   return c;
}

UG_COLOR UG_WindowGetTitleInactiveColor( UG_WINDOW* wnd )
{
   UG_COLOR c = C_BLACK;
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      c = wnd->title.ibc;
   }
   return c;
}

char* UG_WindowGetTitleText( UG_WINDOW* wnd )
{
   char* str = NULL;
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      str = wnd->title.str;
   }
   return str;
}

UG_FONT* UG_WindowGetTitleTextFont( UG_WINDOW* wnd )
{
   UG_FONT* f = NULL;
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      f = (UG_FONT*)wnd->title.font;
   }
   return f;
}

UG_S8 UG_WindowGetTitleTextHSpace( UG_WINDOW* wnd )
{
   UG_S8 hs = 0;
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      hs = wnd->title.h_space;
   }
   return hs;
}

UG_S8 UG_WindowGetTitleTextVSpace( UG_WINDOW* wnd )
{
   UG_S8 vs = 0;
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      vs = wnd->title.v_space;
   }
   return vs;
}

UG_U8 UG_WindowGetTitleTextAlignment( UG_WINDOW* wnd )
{
   UG_U8 align = 0;
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      align = wnd->title.align;
   }
   return align;
}

UG_U8 UG_WindowGetTitleHeight( UG_WINDOW* wnd )
{
   UG_U8 h = 0;
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      h = wnd->title.height;
   }
   return h;
}

UG_S16 UG_WindowGetXStart( UG_WINDOW* wnd )
{
   UG_S16 xs = -1;
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      xs = wnd->xs;
   }
   return xs;
}

UG_S16 UG_WindowGetYStart( UG_WINDOW* wnd )
{
   UG_S16 ys = -1;
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      ys = wnd->ys;
   }
   return ys;
}

UG_S16 UG_WindowGetXEnd( UG_WINDOW* wnd )
{
   UG_S16 xe = -1;
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      xe = wnd->xe;
   }
   return xe;
}

UG_S16 UG_WindowGetYEnd( UG_WINDOW* wnd )
{
   UG_S16 ye = -1;
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      ye = wnd->ye;
   }
   return ye;
}

UG_U8 UG_WindowGetStyle( UG_WINDOW* wnd )
{
   UG_U8 style = 0;
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      style = wnd->style;
   }
   return style;
}

UG_RESULT UG_WindowGetArea( UG_WINDOW* wnd, UG_AREA* a )
{
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      a->xs = wnd->xs;
      a->ys = wnd->ys;
      a->xe = wnd->xe;
      a->ye = wnd->ye;
      if ( wnd->style & WND_STYLE_3D )
      {
         a->xs+=3;
         a->ys+=3;
         a->xe-=3;
         a->ye-=3;
      }
      if ( wnd->style & WND_STYLE_SHOW_TITLE )
      {
         a->ys+= wnd->title.height+1;
      }
      return UG_RESULT_OK;
   }
   return UG_RESULT_FAIL;
}

UG_S16 UG_WindowGetInnerWidth( UG_WINDOW* wnd )
{
   UG_S16 w = 0;
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      w = wnd->xe-wnd->xs;

      /* 3D style? */
      if ( wnd->style & WND_STYLE_3D ) w-=6;

      if ( w < 0 ) w = 0;
   }
   return w;
}

UG_S16 UG_WindowGetOuterWidth( UG_WINDOW* wnd )
{
   UG_S16 w = 0;
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      w = wnd->xe-wnd->xs;

      if ( w < 0 ) w = 0;
   }
   return w;
}

UG_S16 UG_WindowGetInnerHeight( UG_WINDOW* wnd )
{
   UG_S16 h = 0;
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      h = wnd->ye-wnd->ys;

      /* 3D style? */
      if ( wnd->style & WND_STYLE_3D ) h-=6;

      /* Is the title active */
      if ( wnd->style & WND_STYLE_SHOW_TITLE ) h-=wnd->title.height;

      if ( h < 0 ) h = 0;
   }
   return h;
}

UG_S16 UG_WindowGetOuterHeight( UG_WINDOW* wnd )
{
   UG_S16 h = 0;
   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      h = wnd->ye-wnd->ys;

      if ( h < 0 ) h = 0;
   }
   return h;
}

UG_RESULT _UG_WindowDrawTitle( UG_WINDOW* wnd )
{
   UG_TEXT txt;
   UG_S16 xs,ys,xe,ye;

   if ( (wnd != NULL) && (wnd->state & WND_STATE_VALID) )
   {
      xs = wnd->xs;
      ys = wnd->ys;
      xe = wnd->xe;
      ye = wnd->ye;

      /* 3D style? */
      if ( wnd->style & WND_STYLE_3D )
      {
         xs+=3;
         ys+=3;
         xe-=3;
         ye-=3;
      }

      /* Is the window active or inactive? */
      if ( wnd == gui->active_window )
      {
         txt.bc = wnd->title.bc;
         txt.fc = wnd->title.fc;
      }
      else
      {
         txt.bc = wnd->title.ibc;
         txt.fc = wnd->title.ifc;
      }

      /* Draw title */
      UG_FillFrame(xs,ys,xe,ys+wnd->title.height-1,txt.bc);

      /* Draw title text */
      txt.str = wnd->title.str;
      txt.font = wnd->title.font;
      txt.a.xs = xs+3;
      txt.a.ys = ys;
      txt.a.xe = xe;
      txt.a.ye = ys+wnd->title.height-1;
      txt.align = wnd->title.align;
      txt.h_space = wnd->title.h_space;
      txt.v_space = wnd->title.v_space;
      _UG_PutText( &txt );

      /* Draw line */
      UG_DrawLine(xs,ys+wnd->title.height,xe,ys+wnd->title.height,pal_window[11]);
      return UG_RESULT_OK;
   }
   return UG_RESULT_FAIL;
}

void _UG_WindowUpdate( UG_WINDOW* wnd )
{
   UG_U16 i,objcnt;
   UG_OBJECT* obj;
   UG_S16 xs,ys,xe,ye;

   xs = wnd->xs;
   ys = wnd->ys;
   xe = wnd->xe;
   ye = wnd->ye;

   wnd->state &= ~WND_STATE_UPDATE;
   /* Is the window visible? */
   if ( wnd->state & WND_STATE_VISIBLE )
   {
      /* 3D style? */
      if ( (wnd->style & WND_STYLE_3D) && !(wnd->state & WND_STATE_REDRAW_TITLE) )
      {
         _UG_DrawObjectFrame(xs,ys,xe,ye,(UG_COLOR*)pal_window);
         xs+=3;
         ys+=3;
         xe-=3;
         ye-=3;
      }
      /* Show title bar? */
      if ( wnd->style & WND_STYLE_SHOW_TITLE )
      {
         _UG_WindowDrawTitle( wnd );
         ys += wnd->title.height+1;
         if ( wnd->state & WND_STATE_REDRAW_TITLE )
         {
            wnd->state &= ~WND_STATE_REDRAW_TITLE;
            return;
         }
      }
      /* Draw window area? */
      UG_FillFrame(xs,ys,xe,ye,wnd->bc);

      /* Force each object to be updated! */
      objcnt = wnd->objcnt;
      for(i=0; i<objcnt; i++)
      {
         obj = (UG_OBJECT*)&wnd->objlst[i];
         if ( !(obj->state & OBJ_STATE_FREE) && (obj->state & OBJ_STATE_VALID) && (obj->state & OBJ_STATE_VISIBLE) ) obj->state |= (OBJ_STATE_UPDATE | OBJ_STATE_REDRAW);
      }
   }
   else
   {
      UG_FillFrame(wnd->xs,wnd->xs,wnd->xe,wnd->ye,gui->desktop_color);
   }
}

UG_RESULT _UG_WindowClear( UG_WINDOW* wnd )
{
   if ( wnd != NULL )
   {
      if (wnd->state & WND_STATE_VISIBLE)
      {
         wnd->state &= ~WND_STATE_VISIBLE;
         UG_FillFrame( wnd->xs, wnd->ys, wnd->xe, wnd->ye, gui->desktop_color );

         if ( wnd != gui->active_window )
         {
            /* If the current window is visible, update it! */
            if ( gui->active_window->state & WND_STATE_VISIBLE )
            {
               gui->active_window->state &= ~WND_STATE_REDRAW_TITLE;
               gui->active_window->state |= WND_STATE_UPDATE;
            }
         }
      }
      return UG_RESULT_OK;
   }
   return UG_RESULT_FAIL;
}

/* -------------------------------------------------------------------------------- */
/* -- BUTTON FUNCTIONS                                                           -- */
/* -------------------------------------------------------------------------------- */
UG_RESULT UG_ButtonCreate( UG_WINDOW* wnd, UG_BUTTON* btn, UG_U8 id, UG_S16 xs, UG_S16 ys, UG_S16 xe, UG_S16 ye )
{
   UG_OBJECT* obj;

   obj = _UG_GetFreeObject( wnd );
   if ( obj == NULL ) return UG_RESULT_FAIL;

   /* Initialize object-specific parameters */
   btn->state = BTN_STATE_RELEASED;
   btn->bc = wnd->bc;
   btn->fc = wnd->fc;
   btn->abc = wnd->bc;
   btn->afc = wnd->fc;
   btn->style = BTN_STYLE_3D;
   btn->font = NULL;
   btn->str = (char*)"-";

   /* Initialize standard object parameters */
   obj->update = _UG_ButtonUpdate;
   obj->touch_state = OBJ_TOUCH_STATE_INIT;
   obj->type = OBJ_TYPE_BUTTON;
   obj->event = OBJ_EVENT_NONE;
   obj->a_rel.xs = xs;
   obj->a_rel.ys = ys;
   obj->a_rel.xe = xe;
   obj->a_rel.ye = ye;
   obj->a_abs.xs = -1;
   obj->a_abs.ys = -1;
   obj->a_abs.xe = -1;
   obj->a_abs.ye = -1;
   obj->id = id;
   obj->state |= OBJ_STATE_VISIBLE | OBJ_STATE_REDRAW | OBJ_STATE_VALID | OBJ_STATE_TOUCH_ENABLE;
   obj->data = (void*)btn;

   /* Update function: Do your thing! */
   obj->state &= ~OBJ_STATE_FREE;

   return UG_RESULT_OK;
}

UG_RESULT UG_ButtonDelete( UG_WINDOW* wnd, UG_U8 id )
{
   return _UG_DeleteObject( wnd, OBJ_TYPE_BUTTON, id );
}

UG_RESULT UG_ButtonShow( UG_WINDOW* wnd, UG_U8 id )
{
   UG_OBJECT* obj=NULL;

   obj = _UG_SearchObject( wnd, OBJ_TYPE_BUTTON, id );
   if ( obj == NULL ) return UG_RESULT_FAIL;

   obj->state |= OBJ_STATE_VISIBLE;
   obj->state |= OBJ_STATE_UPDATE | OBJ_STATE_REDRAW;

   return UG_RESULT_OK;
}

UG_RESULT UG_ButtonHide( UG_WINDOW* wnd, UG_U8 id )
{
   UG_OBJECT* obj=NULL;
   UG_BUTTON* btn=NULL;

   obj = _UG_SearchObject( wnd, OBJ_TYPE_BUTTON, id );
   if ( obj == NULL ) return UG_RESULT_FAIL;

   btn = (UG_BUTTON*)(obj->data);

   btn->state &= ~BTN_STATE_PRESSED;
   obj->touch_state = OBJ_TOUCH_STATE_INIT;
   obj->event = OBJ_EVENT_NONE;
   obj->state &= ~OBJ_STATE_VISIBLE;
   obj->state |= OBJ_STATE_UPDATE;

   return UG_RESULT_OK;
}

UG_RESULT UG_ButtonSetForeColor( UG_WINDOW* wnd, UG_U8 id, UG_COLOR fc )
{
   UG_OBJECT* obj=NULL;
   UG_BUTTON* btn=NULL;

   obj = _UG_SearchObject( wnd, OBJ_TYPE_BUTTON, id );
   if ( obj == NULL ) return UG_RESULT_FAIL;

   btn = (UG_BUTTON*)(obj->data);
   btn->fc = fc;
   obj->state |= OBJ_STATE_UPDATE | OBJ_STATE_REDRAW;

   return UG_RESULT_OK;
}

UG_RESULT UG_ButtonSetBackColor( UG_WINDOW* wnd, UG_U8 id, UG_COLOR bc )
{
   UG_OBJECT* obj=NULL;
   UG_BUTTON* btn=NULL;

   obj = _UG_SearchObject( wnd, OBJ_TYPE_BUTTON, id );
   if ( obj == NULL ) return UG_RESULT_FAIL;

   btn = (UG_BUTTON*)(obj->data);
   btn->bc = bc;
   obj->state |= OBJ_STATE_UPDATE | OBJ_STATE_REDRAW;

   return UG_RESULT_OK;
}

UG_RESULT UG_ButtonSetAlternateForeColor( UG_WINDOW* wnd, UG_U8 id, UG_COLOR afc )
{
   UG_OBJECT* obj=NULL;
   UG_BUTTON* btn=NULL;

   obj = _UG_SearchObject( wnd, OBJ_TYPE_BUTTON, id );
   if ( obj == NULL ) return UG_RESULT_FAIL;

   btn = (UG_BUTTON*)(obj->data);
   btn->afc = afc;
   obj->state |= OBJ_STATE_UPDATE | OBJ_STATE_REDRAW;

   return UG_RESULT_OK;
}

UG_RESULT UG_ButtonSetAlternateBackColor( UG_WINDOW* wnd, UG_U8 id, UG_COLOR abc )
{
   UG_OBJECT* obj=NULL;
   UG_BUTTON* btn=NULL;

   obj = _UG_SearchObject( wnd, OBJ_TYPE_BUTTON, id );
   if ( obj == NULL ) return UG_RESULT_FAIL;

   btn = (UG_BUTTON*)(obj->data);
   btn->abc = abc;
   obj->state |= OBJ_STATE_UPDATE | OBJ_STATE_REDRAW;

   return UG_RESULT_OK;
}

UG_RESULT UG_ButtonSetText( UG_WINDOW* wnd, UG_U8 id, char* str )
{
   UG_OBJECT* obj=NULL;
   UG_BUTTON* btn=NULL;

   obj = _UG_SearchObject( wnd, OBJ_TYPE_BUTTON, id );
   if ( obj == NULL ) return UG_RESULT_FAIL;

   btn = (UG_BUTTON*)(obj->data);
   btn->str = str;
   obj->state |= OBJ_STATE_UPDATE | OBJ_STATE_REDRAW;

   return UG_RESULT_OK;
}

UG_RESULT UG_ButtonSetFont( UG_WINDOW* wnd, UG_U8 id, const UG_FONT* font )
{
   UG_OBJECT* obj=NULL;
   UG_BUTTON* btn=NULL;

   obj = _UG_SearchObject( wnd, OBJ_TYPE_BUTTON, id );
   if ( obj == NULL ) return UG_RESULT_FAIL;

   btn = (UG_BUTTON*)(obj->data);
   btn->font = font;
   obj->state |= OBJ_STATE_UPDATE | OBJ_STATE_REDRAW;

   return UG_RESULT_OK;
}

UG_RESULT UG_ButtonSetStyle( UG_WINDOW* wnd, UG_U8 id, UG_U8 style )
{
   UG_OBJECT* obj=NULL;
   UG_BUTTON* btn=NULL;

   obj = _UG_SearchObject( wnd, OBJ_TYPE_BUTTON, id );
   if ( obj == NULL ) return UG_RESULT_FAIL;

   btn = (UG_BUTTON*)(obj->data);

   /* Select color scheme */
   btn->style &= ~(BTN_STYLE_USE_ALTERNATE_COLORS | BTN_STYLE_TOGGLE_COLORS);
   btn->state |= BTN_STATE_ALWAYS_REDRAW;
   if ( style & BTN_STYLE_TOGGLE_COLORS )
   {
      btn->style |= BTN_STYLE_TOGGLE_COLORS;
   }
   else if ( style & BTN_STYLE_USE_ALTERNATE_COLORS )
   {
      btn->style |= BTN_STYLE_USE_ALTERNATE_COLORS;
   }
   else
   {
      btn->state &= ~BTN_STATE_ALWAYS_REDRAW;
   }

   /* 3D or 2D */
   if ( style & BTN_STYLE_3D )
   {
      btn->style |= BTN_STYLE_3D;
   }
   else
   {
      btn->style &= ~BTN_STYLE_3D;
   }
   obj->state |= OBJ_STATE_UPDATE | OBJ_STATE_REDRAW;

   return UG_RESULT_OK;
}

UG_COLOR UG_ButtonGetForeColor( UG_WINDOW* wnd, UG_U8 id )
{
   UG_OBJECT* obj=NULL;
   UG_BUTTON* btn=NULL;
   UG_COLOR c = C_BLACK;

   obj = _UG_SearchObject( wnd, OBJ_TYPE_BUTTON, id );
   if ( obj != NULL )
   {
      btn = (UG_BUTTON*)(obj->data);
      c = btn->fc;
   }
   return c;
}

UG_COLOR UG_ButtonGetBackColor( UG_WINDOW* wnd, UG_U8 id )
{
   UG_OBJECT* obj=NULL;
   UG_BUTTON* btn=NULL;
   UG_COLOR c = C_BLACK;

   obj = _UG_SearchObject( wnd, OBJ_TYPE_BUTTON, id );
   if ( obj != NULL )
   {
      btn = (UG_BUTTON*)(obj->data);
      c = btn->bc;
   }
   return c;
}

UG_COLOR UG_ButtonGetAlternateForeColor( UG_WINDOW* wnd, UG_U8 id )
{
   UG_OBJECT* obj=NULL;
   UG_BUTTON* btn=NULL;
   UG_COLOR c = C_BLACK;

   obj = _UG_SearchObject( wnd, OBJ_TYPE_BUTTON, id );
   if ( obj != NULL )
   {
      btn = (UG_BUTTON*)(obj->data);
      c = btn->afc;
   }
   return c;
}

UG_COLOR UG_ButtonGetAlternateBackColor( UG_WINDOW* wnd, UG_U8 id )
{
   UG_OBJECT* obj=NULL;
   UG_BUTTON* btn=NULL;
   UG_COLOR c = C_BLACK;

   obj = _UG_SearchObject( wnd, OBJ_TYPE_BUTTON, id );
   if ( obj != NULL )
   {
      btn = (UG_BUTTON*)(obj->data);
      c = btn->abc;
   }
   return c;
}

char* UG_ButtonGetText( UG_WINDOW* wnd, UG_U8 id )
{
   UG_OBJECT* obj=NULL;
   UG_BUTTON* btn=NULL;
   char* str = NULL;

   obj = _UG_SearchObject( wnd, OBJ_TYPE_BUTTON, id );
   if ( obj != NULL )
   {
      btn = (UG_BUTTON*)(obj->data);
      str = btn->str;
   }
   return str;
}

UG_FONT* UG_ButtonGetFont( UG_WINDOW* wnd, UG_U8 id )
{
   UG_OBJECT* obj=NULL;
   UG_BUTTON* btn=NULL;
   UG_FONT* font = NULL;

   obj = _UG_SearchObject( wnd, OBJ_TYPE_BUTTON, id );
   if ( obj != NULL )
   {
      btn = (UG_BUTTON*)(obj->data);
      font = (UG_FONT*)btn->font;
   }
   return font;
}

UG_U8 UG_ButtonGetStyle( UG_WINDOW* wnd, UG_U8 id )
{
   UG_OBJECT* obj=NULL;
   UG_BUTTON* btn=NULL;
   UG_U8 style = 0;

   obj = _UG_SearchObject( wnd, OBJ_TYPE_BUTTON, id );
   if ( obj != NULL )
   {
      btn = (UG_BUTTON*)(obj->data);
      style = btn->style;
   }
   return style;
}

void _UG_ButtonUpdate(UG_WINDOW* wnd, UG_OBJECT* obj)
{
   UG_BUTTON* btn;
   UG_AREA a;
   UG_TEXT txt;
   UG_U8 d;

   /* Get object-specific data */
   btn = (UG_BUTTON*)(obj->data);

   /* -------------------------------------------------- */
   /* Object touch section                               */
   /* -------------------------------------------------- */
   if ( (obj->touch_state & OBJ_TOUCH_STATE_CHANGED) )
   {
      /* Handle 'click' event */
      if ( obj->touch_state & OBJ_TOUCH_STATE_CLICK_ON_OBJECT )
      {
         obj->event = BTN_EVENT_CLICKED;
         obj->state |= OBJ_STATE_UPDATE;
      }
      /* Is the button pressed down? */
      if ( obj->touch_state & OBJ_TOUCH_STATE_PRESSED_ON_OBJECT )
      {
         btn->state |= BTN_STATE_PRESSED;
         obj->state |= OBJ_STATE_UPDATE;
      }
      /* Can we release the button? */
      else if ( btn->state & BTN_STATE_PRESSED )
      {
         btn->state &= ~BTN_STATE_PRESSED;
         obj->state |= OBJ_STATE_UPDATE;
      }
      obj->touch_state &= ~OBJ_TOUCH_STATE_CHANGED;
   }

   /* -------------------------------------------------- */
   /* Object update section                              */
   /* -------------------------------------------------- */
   if ( obj->state & OBJ_STATE_UPDATE )
   {
      if ( obj->state & OBJ_STATE_VISIBLE )
      {
         /* Full redraw necessary? */
         if ( (obj->state & OBJ_STATE_REDRAW) || (btn->state & BTN_STATE_ALWAYS_REDRAW) )
         {
            UG_WindowGetArea(wnd,&a);
            obj->a_abs.xs = obj->a_rel.xs + a.xs;
            obj->a_abs.ys = obj->a_rel.ys + a.ys;
            obj->a_abs.xe = obj->a_rel.xe + a.xs;
            obj->a_abs.ye = obj->a_rel.ye + a.ys;
            if ( obj->a_abs.ye >= wnd->ye ) return;
            if ( obj->a_abs.xe >= wnd->xe ) return;

            /* 3D or 2D style? */
            d = ( btn->style & BTN_STYLE_3D )? 3:1;

            txt.bc = btn->bc;
            txt.fc = btn->fc;

            if( btn->state & BTN_STATE_PRESSED )
            {
               /* "toggle" style? */
               if( btn->style & BTN_STYLE_TOGGLE_COLORS )
               {
                  /* Swap colors */
                  txt.bc = btn->fc;
                  txt.fc = btn->bc;
               }
               /* Use alternate colors? */
               else if ( btn->style & BTN_STYLE_USE_ALTERNATE_COLORS )
               {
                  txt.bc = btn->abc;
                  txt.fc = btn->afc;
               }
            }
            UG_FillFrame(obj->a_abs.xs+d, obj->a_abs.ys+d, obj->a_abs.xe-d, obj->a_abs.ye-d, txt.bc);

            /* Draw button text */
            txt.a.xs = obj->a_abs.xs+d;
            txt.a.ys = obj->a_abs.ys+d;
            txt.a.xe = obj->a_abs.xe-d;
            txt.a.ye = obj->a_abs.ye-d;
            txt.align = ALIGN_CENTER;
            txt.font = btn->font;
            txt.h_space = 2;
            txt.v_space = 2;
            txt.str = btn->str;
            _UG_PutText( &txt );
            obj->state &= ~OBJ_STATE_REDRAW;
         }
         /* Draw button frame */
         if ( btn->style & BTN_STYLE_3D )
         {  /* 3D */
            _UG_DrawObjectFrame(obj->a_abs.xs,obj->a_abs.ys,obj->a_abs.xe,obj->a_abs.ye, (btn->state&BTN_STATE_PRESSED)?(UG_COLOR*)pal_button_pressed:(UG_COLOR*)pal_button_released);
         }
         else
         {  /* 2D */
            UG_DrawFrame(obj->a_abs.xs,obj->a_abs.ys,obj->a_abs.xe,obj->a_abs.ye,(btn->state&BTN_STATE_PRESSED)?btn->abc:btn->afc);
         }
      }
      else
      {
         UG_FillFrame(obj->a_abs.xs, obj->a_abs.ys, obj->a_abs.xe, obj->a_abs.ye, wnd->bc);
      }
      obj->state &= ~OBJ_STATE_UPDATE;
   }
}

/* -------------------------------------------------------------------------------- */
/* -- TEXTBOX FUNCTIONS                                                          -- */
/* -------------------------------------------------------------------------------- */
UG_RESULT UG_TextboxCreate( UG_WINDOW* wnd, UG_TEXTBOX* txb, UG_U8 id, UG_S16 xs, UG_S16 ys, UG_S16 xe, UG_S16 ye )
{
   UG_OBJECT* obj;

   obj = _UG_GetFreeObject( wnd );
   if ( obj == NULL ) return UG_RESULT_FAIL;

   /* Initialize object-specific parameters */
   txb->str = NULL;
   txb->font = NULL;
   txb->style = 0; /* reserved */
   txb->fc = wnd->fc;
   txb->bc = wnd->bc;
   txb->align = ALIGN_CENTER;
   txb->h_space = 2;
   txb->v_space = 2;

   /* Initialize standard object parameters */
   obj->update = _UG_TextboxUpdate;
   obj->touch_state = OBJ_TOUCH_STATE_INIT;
   obj->type = OBJ_TYPE_TEXTBOX;
   obj->event = OBJ_EVENT_NONE;
   obj->a_rel.xs = xs;
   obj->a_rel.ys = ys;
   obj->a_rel.xe = xe;
   obj->a_rel.ye = ye;
   obj->a_abs.xs = -1;
   obj->a_abs.ys = -1;
   obj->a_abs.xe = -1;
   obj->a_abs.ye = -1;
   obj->id = id;
   obj->state |= OBJ_STATE_VISIBLE | OBJ_STATE_REDRAW | OBJ_STATE_VALID;
   obj->data = (void*)txb;

   /* Update function: Do your thing! */
   obj->state &= ~OBJ_STATE_FREE;

   return UG_RESULT_OK;
}

UG_RESULT UG_TextboxDelete( UG_WINDOW* wnd, UG_U8 id )
{
   return _UG_DeleteObject( wnd, OBJ_TYPE_TEXTBOX, id );
}

UG_RESULT UG_TextboxShow( UG_WINDOW* wnd, UG_U8 id )
{
   UG_OBJECT* obj=NULL;

   obj = _UG_SearchObject( wnd, OBJ_TYPE_TEXTBOX, id );
   if ( obj == NULL ) return UG_RESULT_FAIL;

   obj->state |= OBJ_STATE_VISIBLE;
   obj->state |= OBJ_STATE_UPDATE | OBJ_STATE_REDRAW;

   return UG_RESULT_OK;
}

UG_RESULT UG_TextboxHide( UG_WINDOW* wnd, UG_U8 id )
{
   UG_OBJECT* obj=NULL;

   obj = _UG_SearchObject( wnd, OBJ_TYPE_TEXTBOX, id );
   if ( obj == NULL ) return UG_RESULT_FAIL;

   obj->state &= ~OBJ_STATE_VISIBLE;
   obj->state |= OBJ_STATE_UPDATE;

   return UG_RESULT_OK;
}

UG_RESULT UG_TextboxSetForeColor( UG_WINDOW* wnd, UG_U8 id, UG_COLOR fc )
{
   UG_OBJECT* obj=NULL;
   UG_TEXTBOX* txb=NULL;

   obj = _UG_SearchObject( wnd, OBJ_TYPE_TEXTBOX, id );
   if ( obj == NULL ) return UG_RESULT_FAIL;

   txb = (UG_TEXTBOX*)(obj->data);
   txb->fc = fc;
   obj->state |= OBJ_STATE_UPDATE | OBJ_STATE_REDRAW;

   return UG_RESULT_OK;
}

UG_RESULT UG_TextboxSetBackColor( UG_WINDOW* wnd, UG_U8 id, UG_COLOR bc )
{
   UG_OBJECT* obj=NULL;
   UG_TEXTBOX* txb=NULL;

   obj = _UG_SearchObject( wnd, OBJ_TYPE_TEXTBOX, id );
   if ( obj == NULL ) return UG_RESULT_FAIL;

   txb = (UG_TEXTBOX*)(obj->data);
   txb->bc = bc;
   obj->state |= OBJ_STATE_UPDATE | OBJ_STATE_REDRAW;

   return UG_RESULT_OK;
}

UG_RESULT UG_TextboxSetText( UG_WINDOW* wnd, UG_U8 id, char* str )
{
   UG_OBJECT* obj=NULL;
   UG_TEXTBOX* txb=NULL;

   obj = _UG_SearchObject( wnd, OBJ_TYPE_TEXTBOX, id );
   if ( obj == NULL ) return UG_RESULT_FAIL;

   txb = (UG_TEXTBOX*)(obj->data);
   txb->str = str;
   obj->state |= OBJ_STATE_UPDATE | OBJ_STATE_REDRAW;

   return UG_RESULT_OK;
}

UG_RESULT UG_TextboxSetFont( UG_WINDOW* wnd, UG_U8 id, const UG_FONT* font )
{
   UG_OBJECT* obj=NULL;
   UG_TEXTBOX* txb=NULL;

   obj = _UG_SearchObject( wnd, OBJ_TYPE_TEXTBOX, id );
   if ( obj == NULL ) return UG_RESULT_FAIL;

   txb = (UG_TEXTBOX*)(obj->data);
   txb->font = font;
   obj->state |= OBJ_STATE_UPDATE | OBJ_STATE_REDRAW;

   return UG_RESULT_OK;
}

UG_RESULT UG_TextboxSetHSpace( UG_WINDOW* wnd, UG_U8 id, UG_S8 hs )
{
   UG_OBJECT* obj=NULL;
   UG_TEXTBOX* txb=NULL;

   obj = _UG_SearchObject( wnd, OBJ_TYPE_TEXTBOX, id );
   if ( obj == NULL ) return UG_RESULT_FAIL;

   txb = (UG_TEXTBOX*)(obj->data);
   txb->h_space = hs;
   obj->state |= OBJ_STATE_UPDATE | OBJ_STATE_REDRAW;

   return UG_RESULT_OK;
}

UG_RESULT UG_TextboxSetVSpace( UG_WINDOW* wnd, UG_U8 id, UG_S8 vs )
{
   UG_OBJECT* obj=NULL;
   UG_TEXTBOX* txb=NULL;

   obj = _UG_SearchObject( wnd, OBJ_TYPE_TEXTBOX, id );
   if ( obj == NULL ) return UG_RESULT_FAIL;

   txb = (UG_TEXTBOX*)(obj->data);
   txb->v_space = vs;
   obj->state |= OBJ_STATE_UPDATE | OBJ_STATE_REDRAW;

   return UG_RESULT_OK;
}

UG_RESULT UG_TextboxSetAlignment( UG_WINDOW* wnd, UG_U8 id, UG_U8 align )
{
   UG_OBJECT* obj=NULL;
   UG_TEXTBOX* txb=NULL;

   obj = _UG_SearchObject( wnd, OBJ_TYPE_TEXTBOX, id );
   if ( obj == NULL ) return UG_RESULT_FAIL;

   txb = (UG_TEXTBOX*)(obj->data);
   txb->align = align;
   obj->state |= OBJ_STATE_UPDATE | OBJ_STATE_REDRAW;

   return UG_RESULT_OK;
}

UG_COLOR UG_TextboxGetForeColor( UG_WINDOW* wnd, UG_U8 id )
{
   UG_OBJECT* obj=NULL;
   UG_TEXTBOX* txb=NULL;
   UG_COLOR c = C_BLACK;

   obj = _UG_SearchObject( wnd, OBJ_TYPE_TEXTBOX, id );
   if ( obj != NULL )
   {
      txb = (UG_TEXTBOX*)(obj->data);
      c = txb->fc;
   }
   return c;
}

UG_COLOR UG_TextboxGetBackColor( UG_WINDOW* wnd, UG_U8 id )
{
   UG_OBJECT* obj=NULL;
   UG_TEXTBOX* txb=NULL;
   UG_COLOR c = C_BLACK;

   obj = _UG_SearchObject( wnd, OBJ_TYPE_TEXTBOX, id );
   if ( obj != NULL )
   {
      txb = (UG_TEXTBOX*)(obj->data);
      c = txb->bc;
   }
   return c;
}

char* UG_TextboxGetText( UG_WINDOW* wnd, UG_U8 id )
{
   UG_OBJECT* obj=NULL;
   UG_TEXTBOX* txb=NULL;
   char* str = NULL;

   obj = _UG_SearchObject( wnd, OBJ_TYPE_TEXTBOX, id );
   if ( obj != NULL )
   {
      txb = (UG_TEXTBOX*)(obj->data);
      str = txb->str;
   }
   return str;
}

UG_FONT* UG_TextboxGetFont( UG_WINDOW* wnd, UG_U8 id )
{
   UG_OBJECT* obj=NULL;
   UG_TEXTBOX* txb=NULL;
   UG_FONT* font = NULL;

   obj = _UG_SearchObject( wnd, OBJ_TYPE_TEXTBOX, id );
   if ( obj != NULL )
   {
      txb = (UG_TEXTBOX*)(obj->data);
      font = (UG_FONT*)txb->font;
   }
   return font;
}

UG_S8 UG_TextboxGetHSpace( UG_WINDOW* wnd, UG_U8 id )
{
   UG_OBJECT* obj=NULL;
   UG_TEXTBOX* txb=NULL;
   UG_S8 hs = 0;

   obj = _UG_SearchObject( wnd, OBJ_TYPE_TEXTBOX, id );
   if ( obj != NULL )
   {
      txb = (UG_TEXTBOX*)(obj->data);
      hs = txb->h_space;
   }
   return hs;
}

UG_S8 UG_TextboxGetVSpace( UG_WINDOW* wnd, UG_U8 id )
{
   UG_OBJECT* obj=NULL;
   UG_TEXTBOX* txb=NULL;
   UG_S8 vs = 0;

   obj = _UG_SearchObject( wnd, OBJ_TYPE_TEXTBOX, id );
   if ( obj != NULL )
   {
      txb = (UG_TEXTBOX*)(obj->data);
      vs = txb->v_space;
   }
   return vs;
}

UG_U8 UG_TextboxGetAlignment( UG_WINDOW* wnd, UG_U8 id )
{
   UG_OBJECT* obj=NULL;
   UG_TEXTBOX* txb=NULL;
   UG_U8 align = 0;

   obj = _UG_SearchObject( wnd, OBJ_TYPE_TEXTBOX, id );
   if ( obj != NULL )
   {
      txb = (UG_TEXTBOX*)(obj->data);
      align = txb->align;
   }
   return align;
}

void _UG_TextboxUpdate(UG_WINDOW* wnd, UG_OBJECT* obj)
{
   UG_TEXTBOX* txb;
   UG_AREA a;
   UG_TEXT txt;

   /* Get object-specific data */
   txb = (UG_TEXTBOX*)(obj->data);

   /* -------------------------------------------------- */
   /* Object touch section                               */
   /* -------------------------------------------------- */

   /* Textbox doesn't support touch */

   /* -------------------------------------------------- */
   /* Object update section                              */
   /* -------------------------------------------------- */
   if ( obj->state & OBJ_STATE_UPDATE )
   {
      if ( obj->state & OBJ_STATE_VISIBLE )
      {
         /* Full redraw necessary? */
         if ( obj->state & OBJ_STATE_REDRAW )
         {
            UG_WindowGetArea(wnd,&a);
            obj->a_abs.xs = obj->a_rel.xs + a.xs;
            obj->a_abs.ys = obj->a_rel.ys + a.ys;
            obj->a_abs.xe = obj->a_rel.xe + a.xs;
            obj->a_abs.ye = obj->a_rel.ye + a.ys;
            if ( obj->a_abs.ye >= wnd->ye ) return;
            if ( obj->a_abs.xe >= wnd->xe ) return;

            txt.bc = txb->bc;
            txt.fc = txb->fc;

            UG_FillFrame(obj->a_abs.xs, obj->a_abs.ys, obj->a_abs.xe, obj->a_abs.ye, txt.bc);

            /* Draw Textbox text */
            txt.a.xs = obj->a_abs.xs;
            txt.a.ys = obj->a_abs.ys;
            txt.a.xe = obj->a_abs.xe;
            txt.a.ye = obj->a_abs.ye;
            txt.align = txb->align;
            txt.font = txb->font;
            txt.h_space = txb->h_space;
            txt.v_space = txb->v_space;
            txt.str = txb->str;
            _UG_PutText( &txt );
            obj->state &= ~OBJ_STATE_REDRAW;
         }
      }
      else
      {
         UG_FillFrame(obj->a_abs.xs, obj->a_abs.ys, obj->a_abs.xe, obj->a_abs.ye, wnd->bc);
      }
      obj->state &= ~OBJ_STATE_UPDATE;
   }
}

/* -------------------------------------------------------------------------------- */
/* -- IMAGE FUNCTIONS                                                            -- */
/* -------------------------------------------------------------------------------- */
UG_RESULT UG_ImageCreate( UG_WINDOW* wnd, UG_IMAGE* img, UG_U8 id, UG_S16 xs, UG_S16 ys, UG_S16 xe, UG_S16 ye )
{
   UG_OBJECT* obj;

   obj = _UG_GetFreeObject( wnd );
   if ( obj == NULL ) return UG_RESULT_FAIL;

   /* Initialize object-specific parameters */
   img->img = NULL;
   img->type = IMG_TYPE_BMP;

   /* Initialize standard object parameters */
   obj->update = _UG_ImageUpdate;
   obj->touch_state = OBJ_TOUCH_STATE_INIT;
   obj->type = OBJ_TYPE_IMAGE;
   obj->event = OBJ_EVENT_NONE;
   obj->a_rel.xs = xs;
   obj->a_rel.ys = ys;
   obj->a_rel.xe = xe;
   obj->a_rel.ye = ye;
   obj->a_abs.xs = -1;
   obj->a_abs.ys = -1;
   obj->a_abs.xe = -1;
   obj->a_abs.ye = -1;
   obj->id = id;
   obj->state |= OBJ_STATE_VISIBLE | OBJ_STATE_REDRAW | OBJ_STATE_VALID;
   obj->data = (void*)img;

   /* Update function: Do your thing! */
   obj->state &= ~OBJ_STATE_FREE;

   return UG_RESULT_OK;
}

UG_RESULT UG_ImageDelete( UG_WINDOW* wnd, UG_U8 id )
{
   return _UG_DeleteObject( wnd, OBJ_TYPE_IMAGE, id );
}

UG_RESULT UG_ImageShow( UG_WINDOW* wnd, UG_U8 id )
{
   UG_OBJECT* obj=NULL;

   obj = _UG_SearchObject( wnd, OBJ_TYPE_IMAGE, id );
   if ( obj == NULL ) return UG_RESULT_FAIL;

   obj->state |= OBJ_STATE_VISIBLE;
   obj->state |= OBJ_STATE_UPDATE | OBJ_STATE_REDRAW;

   return UG_RESULT_OK;
}

UG_RESULT UG_ImageHide( UG_WINDOW* wnd, UG_U8 id )
{
   UG_OBJECT* obj=NULL;

   obj = _UG_SearchObject( wnd, OBJ_TYPE_IMAGE, id );
   if ( obj == NULL ) return UG_RESULT_FAIL;

   obj->state &= ~OBJ_STATE_VISIBLE;
   obj->state |= OBJ_STATE_UPDATE;

   return UG_RESULT_OK;
}

UG_RESULT UG_ImageSetBMP( UG_WINDOW* wnd, UG_U8 id, const UG_BMP* bmp )
{
   UG_OBJECT* obj=NULL;
   UG_IMAGE* img=NULL;

   obj = _UG_SearchObject( wnd, OBJ_TYPE_IMAGE, id );
   if ( obj == NULL ) return UG_RESULT_FAIL;

   img = (UG_IMAGE*)(obj->data);
   img->img = (void*)bmp;
   img->type = IMG_TYPE_BMP;
   obj->state |= OBJ_STATE_UPDATE | OBJ_STATE_REDRAW;

   return UG_RESULT_OK;
}

void _UG_ImageUpdate(UG_WINDOW* wnd, UG_OBJECT* obj)
{
   UG_IMAGE* img;
   UG_AREA a;

   /* Get object-specific data */
   img = (UG_IMAGE*)(obj->data);

   /* -------------------------------------------------- */
   /* Object touch section                               */
   /* -------------------------------------------------- */

   /* Image doesn't support touch */

   /* -------------------------------------------------- */
   /* Object update section                              */
   /* -------------------------------------------------- */
   if ( obj->state & OBJ_STATE_UPDATE )
   {
      if ( obj->state & OBJ_STATE_VISIBLE )
      {
         /* Full redraw necessary? */
         if ( obj->state & OBJ_STATE_REDRAW )
         {
            UG_WindowGetArea(wnd,&a);
            /* ToDo: more/better image features */
            obj->a_abs.xs = obj->a_rel.xs + a.xs;
            obj->a_abs.ys = obj->a_rel.ys + a.ys;
            obj->a_abs.xe = obj->a_rel.xs + ((UG_BMP*)img->img)->width + a.xs;
            obj->a_abs.ye = obj->a_rel.ys + ((UG_BMP*)img->img)->height + a.ys;
            if ( obj->a_abs.ye >= wnd->ye ) return;
            if ( obj->a_abs.xe >= wnd->xe ) return;

            /* Draw Image */
            if ( (img->img != NULL) && (img->type & IMG_TYPE_BMP) )
            {
               UG_DrawBMP(obj->a_abs.xs,obj->a_abs.ys,(UG_BMP*)img->img);
            }

            obj->state &= ~OBJ_STATE_REDRAW;
         }
      }
      else
      {
         UG_FillFrame(obj->a_abs.xs, obj->a_abs.ys, obj->a_abs.xe, obj->a_abs.ye, wnd->bc);
      }
      obj->state &= ~OBJ_STATE_UPDATE;
   }
}
