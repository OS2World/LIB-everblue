#include "X11.h"

Status XQueryBestCursor(display, which_screen, width, height, width_return, height_return) 
      Display *display;
      Drawable which_screen;
      unsigned int width, height;
      unsigned int *width_return, *height_return;
{
   DBUG_ENTER("XQueryBestCursor");
   if(width < 32 && height < 32) 
      {
        *width_return = 16;
        *height_return = 16;
      }
   else
      {
        *width_return = 32;
        *height_return = 32;
      }
   DBUG_RETURN(True);
}

Status XQueryBestStipple(display, which_screen, width, height, width_return, height_return) 
      Display *display;
      Drawable which_screen;
      unsigned int width, height;
      unsigned int *width_return, *height_return;
{
   DBUG_ENTER("XQueryBestStipple");
   *width_return = width;
   *height_return = height;
   DBUG_RETURN(True);
}
Status XQueryBestTile(display, which_screen, width, height, width_return, height_return) 
      Display *display;
      Drawable which_screen;
      unsigned int width, height;
      unsigned int *width_return, *height_return;
{
   DBUG_ENTER("XQueryBestTile");
   *width_return = width;
   *height_return = height;
   DBUG_RETURN(True);
}

Status XQueryBestSize(display, class, which_screen, width, height, width_return, height_return) 
      Display *display;
      int class;
      Drawable which_screen;
      unsigned int width, height;
      unsigned int *width_return, *height_return;
{
   DBUG_ENTER("XQueryBestSize");
   switch(class) 
   {
   case TileShape:
      XQueryBestTile(display, which_screen, width, height, width_return, height_return);
      break;
   case StippleShape:
      XQueryBestStipple(display, which_screen, width, height, width_return, height_return);
      break;
   case CursorShape:
      XQueryBestCursor(display, which_screen, width, height, width_return, height_return);
      break;
   }
   DBUG_RETURN(True);
}

