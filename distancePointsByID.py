#!/usr/bin/env python3

from read_write_model import read_points3D_binary
import numpy as np


def point_distance(points3D, id1, id2):
    p1 = points3D[id1].xyz
    p2 = points3D[id2].xyz
    return np.linalg.norm(p1 - p2)



#
id_a = 113118#107854#105919
id_b = 116108#128847#124648



points3D = read_points3D_binary("sparse/0/points3D.bin")

dist = point_distance(points3D, id_a, id_b)
print(f"Original Distance between {id_a} and {id_b}: {dist:.6f}")



points3D = read_points3D_binary("model_scaled/points3D.bin")

dist = point_distance(points3D, id_a, id_b)
print(f"Distance between {id_a} and {id_b}: {dist:.6f}")
