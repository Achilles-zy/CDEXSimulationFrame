import numpy as np


fo = open("point_source.mac", "w")

fo.write("/gps/particle e-\n")
fo.write("/gps/pos/type Point\n")
fo.write("/gps/ang/type iso\n")


energy = [2, 3, 4, 5, 7, 10, 15, 20]

for energy_idx in range(8):
    for x_idx in range(7):
        for y_idx in range(7):
            for z_idx in range(3):
                fo.write("/gps/energy "+str(energy[energy_idx])+" keV\n")
                fo.write("/gps/position "+str(-18+x_idx*6)+" "+str(-18+y_idx*6)+" "+str(-4+z_idx*4)+" mm\n")
                # fo.write("/analysis/setFileName Point_200_keV_x_"+str(x_idx)+"_y_"+str(y_idx)+"_z_"+str(z_idx)+"\n")
                fo.write("/run/beamOn 100\n")