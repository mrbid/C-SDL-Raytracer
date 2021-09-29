#ifndef ICON_H
#define ICON_H

static const struct {
  Uint32  width;
  Uint32  height;
  Uint32  bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  Uint8   pixel_data[16 * 16 * 4 + 1];
} icon_image = {
  16, 16, 4,
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\377\000\000\000\377\000\000\000\377\000"
  "\000\000\377\000\000\000\377\000\000\000\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\021\377\000\000\000\377\000\000\314\377\000\063\314\377\000\000\314"
  "\377\000\063\377\377\000\000\314\377\000\000\335\377\000\000\000\377\000\000\021\377\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\"\377\000\000\273\377\000\063\314\377\000f\377"
  "\377\063\231\377\377\000\231\377\377\063\231\377\377\063f\377\377\000f\314\377\000"
  "\000\335\377\000\000\314\377\000\000\"\377\000\000\000\000\000\000\000\000\000\000\000\000\021\000\000\377\000\000\252"
  "\377\000\063\377\377\063\063\377\377\063\231\314\377\063\314\377\377f\314\377\377"
  "f\231\377\377\063\231\377\377\000\063\377\377\000f\314\377\000\063\377\377\000\000\335"
  "\377\000\000\021\377\000\000\000\000\000\000\000\000\000\000\252\377\000\000\314\377\000\063\314\377\063\231"
  "\377\377f\314\377\377\231\377\377\377f\314\377\377\063\231\377\377f\314\377"
  "\377\000f\377\377\000\063\314\377\000\063\231\377\000\063\314\377\000\000\000\377\000\000\000\000"
  "\000\000\000\377\000\000\273\377\000\063\231\377\000\063\377\377\000\231\377\377f\231\377\377"
  "f\314\377\377f\314\377\377\063\231\314\377\000f\377\377\063f\314\377\000f\377\377"
  "\000\063\231\377\000\000f\377\000\000\252\377\000\000\000\377\000\000\000\377\000\000\252\377\000f\314"
  "\377\000f\377\377\000\063\314\377\000f\377\377\063\231\377\377\063f\377\377\063\231"
  "\377\377\063\231\314\377\000\063\377\377\000\063\231\377\000\063\314\377\000\063f\377"
  "\000\000\231\377\000\000\000\377\000\000\000\377\000\000w\377\000\063\314\377\000\063\314\377\000f\377"
  "\377\000f\377\377\000f\377\377\000\231\314\377\000f\377\377\000f\377\377\000\063\377\377"
  "\000\063\314\377\000\000U\377\000\063\231\377\000\000f\377\000\000\000\377\000\000\000\377\000\063\231"
  "\377\000\063\231\377\000\000\335\377\000\000\314\377\000\063\377\377\000f\314\377\000\063\377"
  "\377\000\063\314\377\000\063\377\377\000\000\314\377\000\063\231\377\000\000f\377\000\000U\377"
  "\000\000\210\377\000\000\000\377\000\000\000\377\000\000\252\377\000\000w\377\000\000\231\377\000\000\252"
  "\377\000\063\231\377\000\063\377\377\000\000\335\377\000\063\377\377\000\000\314\377\000\063"
  "\231\377\000\000f\377\000\000w\377\000\063f\377\000\000w\377\000\000\000\377\000\000\000\377\000\000\252"
  "\377\000\063\231\377\000\000w\377\000\063\314\377\000\000w\377\000\000\231\377\000\000\231\377"
  "\000\063\314\377\000\000f\377\000\000\252\377\000\000\210\377\000\063f\377\000\063\231\377\000\000"
  "\231\377\000\000\000\377\000\000\000\000\000\000\000\377\000\000w\377\000\063f\377\000\000U\377\000\000f\377"
  "\000\000f\377\000\000f\377\000\000f\377\000\000w\377\000\000U\377\000\000D\377\000\063f\377\000\000D\377"
  "\000\000\000\377\000\000\000\000\000\000\000\000\000\000\000\377\000\000\231\377\000\063f\377\000\000w\377\000\000f"
  "\377\000\000D\377\000\000w\377\000\000w\377\000\063f\377\000\063\231\377\000\063\231\377\000\000w"
  "\377\000\000\273\377\000\000\021\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\"\377\000\000\210\377"
  "\000\000f\377\000\063f\377\000f\231\377\000\063\231\377\000\063f\377\000\063\231\377\000\063\231"
  "\377\000\000f\377\000\000\252\377\000\000\"\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\021\377\000\000\000\377\000\000U\377\000\000f\377\000\000f\377\000\000D\377\000\000w\377\000\000"
  "\210\377\000\000\000\377\000\000\021\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000\000\000\377\000"
  "\000\000\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
};

static const struct {
  Uint32  width;
  Uint32  height;
  Uint32  bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  Uint8   pixel_data[16 * 16 * 4 + 1];
} icon_image1 = {
  16, 16, 4,
  "\000\000\000\000\000\000\000\000\000\000\000\000\023\023\023\005!!!E\035\035\035\205$\040\026\372=\033\033\377"
  "M\032\030\377&\023\017\277\016\002\001{\000\001\006\017\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\"\"\"\012\037\037\037\265++-\372,,,\377A/.\377\\((\377\244\061"
  "#\377\252+\026\377n\037\027\377\067\030\006\360\003\001\001I\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\022\022\022\012\037\037#\266//B\377\067\070\\\377WVZ\377J?B\377;\064\065\377"
  "\207-,\377\354\065#\377\344Q\031\377\231\067\025\377B\032\004\365\003\001\000I\000\000\000\000"
  "\000\000\000\000\002\002\002\005\026\026\027\265++Q\377C\066v\377[c\215\377\216\216\263\377l"
  "q\213\377QGL\377H\065\065\377\233D+\377\374v\"\377\361\222#\377\270T\002\377"
  "@\037\000\365\000\000\000E\000\000\000\000\004\004\004E!\040>\372\062,\222\377\071\070\234\377xx\266"
  "\377\317\317\353\377\267\256\322\377ei\201\377;\070H\377e\062%\377\345\201"
  "\"\377\377\275!\377\361\237\026\377\222Q\000\377\017\004\000\272\000\000\000\005\031\031\036"
  "\205\037\037q\377\064\062\277\377OD\300\377ok\312\377\261\261\332\377|\212\270"
  "\377RS\203\377\063\064[\377F(\060\377\276V\032\377\375\217\026\377\376\260\040"
  "\377\343\216\002\377M:\000\365\000\000\000\017\013\014\031\372#\"\233\377,,\315\377\065"
  "\063\314\377PG\314\377^^\302\377Q[\263\377\066\067\205\377-%g\377*'G\377u(\030"
  "\377\320Q\025\377\363`\004\377\373\243\000\377\236f\000\377\023\006\000{\007\007=\377\030"
  "\031\262\377\"#\341\377,+\345\377\064\062\332\377\065\064\313\377\064\065\267\377"
  "\063A\245\377+$w\377)\"h\377P\011\061\377\217!\006\377\323\035\024\377\357z\000\377"
  "\303\220\000\377-\033\000\177\007\024\067\377\026\025\252\377\025\030\342\377\"\031\351"
  "\377$*\327\377$$\324\377+$\312\377$&\256\377\"#\224\377\032\"v\377-\026\\\377"
  "\210\004\030\377\274\025\002\377\327B\000\377\274x\000\377%\024\000\177\004\006\025\372\024"
  "\003\216\377\004\026\333\377!\025\350\377\031\031\341\377\040!\325\377\033\"\311\377"
  "%#\305\377;*\250\377\031\030\213\377\034\025\213\377{\002/\377\253\000\000\377\272"
  "\010\000\377\206)\000\377\002\001\000{\001\010\010\200\004\000b\377\024\024\317\377\006\004\365\377"
  "\025\025\344\377\026\004\322\377\037\025\304\377\030\030\275\377%\032\260\377\026\025"
  "\235\377\016\002\225\377v\000b\377\236\000\002\377\226\000\000\377D\002\000\360\000\000\000\017\001"
  "\006\000\017\000\004\062\360\002\000\234\377\000\024\360\377\000\002\373\377\002\024\346\377\002\004"
  "\300\377\024\002\261\377\026\024\256\377\030\030\252\377\027\040\237\377r\002u\377\241"
  "\000\005\377v\000\000\377\015\000\000\177\000\000\000\000\000\000\000\000\000\001\011\177\000\004Z\377\000\000\272"
  "\377\000\000\361\377\000\002\367\377\000\000\302\377\002\000\235\377\000\000\226\377\032\026\237"
  "\377\060\026\260\377a\000\202\377\216\000'\377\066\004\002\360\000\006\000\017\000\000\000\000\000\000"
  "\000\000\000\001\000\012\000\000\014\266\004\000[\377\000\000\257\377\000\000\316\377\000\000\247\377\000\000"
  "n\377\000\000e\377\001\000\200\377\012\004\216\377<\001p\377&\004\035\365\003\001\001I\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\001\000\000\012\000\000\012\265\004\004:\372\001\000h\377\000\000g\377\000"
  "\000C\377\000\000\066\377\000\000@\377\011\027\070\372\016\016\027\272\001\001\001E\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\002\000\005\001\003\001E\010\010\014\205\004\004\010\372"
  "\000\004\022\377\000\004\017\377\000\005\006\277\003\003\002E\021\002\002\005\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000"
};

#endif
