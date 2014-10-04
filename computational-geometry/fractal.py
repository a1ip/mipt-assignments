import numpy
import math
import Image, ImageDraw

def draw_point(draw, point):
    draw.ellipse([(point[0] - 3, point[1] - 3), (point[0] + 3, point[1] + 3)], fill = 0)

def draw_polygon(draw, polygon):
    draw.polygon(polygon, outline = 0)

def draw_polygons(draw, polygons):
    for polygon in polygons:
        draw_polygon(draw, polygon)

def apply_map(map, point):
    return (map[0] * point[0] + map[1] * point[1] + map[2], map[3] * point[0] + map[4] * point[1] + map[5])

def apply_maps(maps, polygons):
    result = []
    for map in maps:
        for polygon in polygons:
            mapped_polygon = []
            for point in polygon:
                mapped_polygon.append(apply_map(map, point))
            result.append(mapped_polygon)
    return result

if __name__ == "__main__":
    size = 9 * 9 * 9 * 9
    
    image = Image.new("RGB", (size, size), (255, 255, 255))
    draw = ImageDraw.Draw(image)
    
    maps = [
        #[1.0, 0.0, 0.0, 0.0, 1.0, 0.0],
        
        [1.0 / 3.0, 0.0, 0.0, 0.0, 1.0 / 3.0, 0.0],
        [1.0 / 3.0, 0.0, size * 1.0 / 3.0, 0.0, 1.0 / 3.0, 0.0],
        [1.0 / 3.0, 0.0, size * 2.0 / 3.0, 0.0, 1.0 / 3.0, 0.0],
        
        [1.0 / 3.0, 0.0, 0.0, 0.0, 1.0 / 3.0, size * 1.0 / 3.0],
        [1.0 / 3.0, 0.0, size * 2.0 / 3.0, 0.0, 1.0 / 3.0, size * 1.0 / 3.0],
        
        [1.0 / 3.0, 0.0, 0.0 / 3.0, 0.0, 1.0 / 3.0, size * 2.0 / 3.0],
        [1.0 / 3.0, 0.0, size * 1.0 / 3.0, 0.0, 1.0 / 3.0, size * 2.0 / 3.0],
        [1.0 / 3.0, 0.0, size * 2.0 / 3.0, 0.0, 1.0 / 3.0, size * 2.0 / 3.0]
    ]
    polygons = [
        [(0.0, 0.0), (0.0, size * 1.0), (size * 1.0, size * 1.0), (size * 1.0, 0.0)]
    ]
    
    for i in xrange(0, 7):
        polygons = apply_maps(maps, polygons)
    
    draw_polygons(draw, polygons)
    
    image.save("fractal.png")