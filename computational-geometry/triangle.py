import sys
import numpy
import Image, ImageDraw

def draw_scene(triangle, point):
    image = Image.new("RGB", (400, 400), (255, 255, 255))

    draw = ImageDraw.Draw(image)
    draw.point([point[0], point[1]], 0)
    draw.polygon([triangle[0][0], triangle[0][1], triangle[1][0], triangle[1][1], triangle[2][0], triangle[2][1]], outline = 128)
    del draw
    
    image.save("triangle.png")

def calculate_triangle_area(triangle):
    first_side_3d = numpy.append(triangle[1] - triangle[0], numpy.array([0.0]))
    second_side_3d = numpy.append(triangle[2] - triangle[0], numpy.array([0.0]))
    return abs(numpy.cross(first_side_3d, second_side_3d)[2])

def is_point_in_triangle(triangle, point):
    partition_area = 0.0
    partition_area += calculate_triangle_area((triangle[0], triangle[1], point))
    partition_area += calculate_triangle_area((triangle[1], triangle[2], point))
    partition_area += calculate_triangle_area((triangle[2], triangle[0], point))
    triangle_area = calculate_triangle_area(triangle)
    return (partition_area - triangle_area) < 0.000001

x = numpy.array([123.0, 324.0])
y = numpy.array([345.0, 11.0])
z = numpy.array([1.0, 100.0])
t = (x, y, z)
p = numpy.array([90.0, 70.0])

draw_scene(t, p)

print is_point_in_triangle(t, p)