# int newt_centered_window ( int $width , int $height [, string $title ] )

return_type: int

arguments:
  width:
    type: int
  height:
    type: int
  title:
    type: string
    default: null

%%

retval=newtCenteredWindow((long)width, (long)height, title_val);
