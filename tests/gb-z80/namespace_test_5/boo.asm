        .macro "boo"
          jp myLabel\@
myLabel\@:.db 0
          jp my2ndLabel\@
my2ndLabel\@
          jp my3rdLabel\@
my3rdLabel\@:
        .endm
