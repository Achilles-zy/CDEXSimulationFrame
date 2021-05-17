# PENVetoSim

9/24

Macro cmds

/PEN/det/set/wiretype(string)

A1 A2

/PEN/det/set/confine(string)

Set your confine volume name to change file name. This cmd is only for changing file name, use /gps/pos/confine to actually change your confine volume.

But if necessary you can basically put any string in the file name with this cmd.



10/19

Merged the function of self veto simulation and active veto simulation

Added macro

/PEN/src/type SrcType(string)

current options: Wire/PENShell

Wire is for active-veto simulation

Src is uniformly distributed in the "Wire" volume

PENShell is for self-veto simulation

Src is uniformly distributed in the "PENShell" volume



10/21

/PEN/sim/set/mode Mode(string)

Current option: CDEX/LEGEND



10/23

/PEN/run/refresh IfRefresh(bool)

Set if refresh the output txt, works for LEGEND mode



/PEN/mat/set/PENpropertiesID ID(int)

current options:

0: LY=3000 ph/MeV, abs at 450 nm=15.52 mm, first result from Luis

1: LY=3500 ph/MeV, abs at 450 nm=40 mm

2: LY=6000 ph/MeV, abs at 450 nm=100 mm



10/27

Fixed bugs in .txt and .root output files.